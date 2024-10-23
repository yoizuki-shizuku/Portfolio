/*/
 *  �t�@�C����		�F	BehaviourTree.cpp
 *  �T�v			�F	�r�w�C�r�A�c���[�ň����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/13
/*/
#include "pch.h"
#include "BehaviourTree.h"

const NodeStatus NodeStatus::INVALID = -1;
const NodeStatus NodeStatus::SUCCESS = 0;
const NodeStatus NodeStatus::FAILURE = 1;
const NodeStatus NodeStatus::RUNNING = 2;

const NodeJudge NodeJudge::INVALID = -1;
const NodeJudge NodeJudge::SUCCESS = 0;
const NodeJudge NodeJudge::FAILURE = 1;


#pragma region ControlNode

ControlNode::ControlNode() :
	Node(),
	m_children{}
{
}

void ControlNode::AddChild(Node* child)
{
	std::unique_ptr<Node> addChild;
	addChild.reset(child);
	
	m_children.push_back(std::move(addChild));
}

#pragma endregion

#pragma region Sequence

Sequence::Sequence() :
	ControlNode()
{
}

NodeStatus Sequence::Invoke()
{
	for (auto& node : GetChildren())
	{
		NodeStatus state = node->Invoke();

		if (state != NodeStatus::SUCCESS)
		{
			return state;
		}
	}

	return NodeStatus::SUCCESS;
}

void Sequence::ReleaseRunning()
{
	for (auto& node : GetChildren())
	{
		node->ReleaseRunning();
	}
}

#pragma endregion

#pragma region Selector

Selector::Selector() :
	ControlNode(),
	m_running(nullptr)
{
}

NodeStatus Selector::Invoke()
{
	if (m_running != nullptr)
	{
		NodeStatus state = m_running->Invoke();
		
		if (state == NodeStatus::RUNNING)
		{
			return NodeStatus::SUCCESS;
		}

		m_running = nullptr;
		return state;
	}

	for (std::vector<std::unique_ptr<Node>>::iterator it = GetChildren().begin(); it != GetChildren().end(); it++)
	{
		NodeStatus state = it->get()->Invoke();

		if (state == NodeStatus::RUNNING)
		{
			m_running = it->get();
			return state;
		}

		if (state == NodeStatus::SUCCESS)
		{
			m_running = nullptr;
			return state;
		}
	}

	m_running = nullptr;
	return NodeStatus::FAILURE;
}

void Selector::ReleaseRunning()
{
	m_running = nullptr;
	for (auto& node : GetChildren())
	{
		node->ReleaseRunning();
	}
}

#pragma endregion

#pragma region Action

Action::Action(std::function<NodeStatus()> action) :
	m_action(action)
{
}

NodeStatus Action::Invoke()
{
	return m_action();
}

#pragma endregion

#pragma region Condition

Condition::Condition(std::function<NodeJudge()> condition) :
	m_condition(condition),
	m_children(nullptr)
{
}

NodeStatus Condition::Invoke()
{
	//	����
	NodeJudge judge = m_condition();

	//	FAILURE�Ȃ炻�̂܂܌��ʂ�Ԃ�
	if (judge == NodeJudge::FAILURE) return judge;

	//	�q�����Ȃ��Ȃ画�茋�ʂ݂̂�Ԃ�
	if (!m_children) return judge;

	//	�q��Action�����s
	return m_children->Invoke();
}

void Condition::AddChild(Action* child)
{
	m_children.reset(child);
}

#pragma endregion

