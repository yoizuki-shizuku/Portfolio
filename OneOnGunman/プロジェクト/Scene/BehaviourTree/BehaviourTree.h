/*/
 *  �t�@�C����		�F	BehaviourTree.h
 *  �T�v			�F	�r�w�C�r�A�c���[�ň����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/13
/*/
#pragma once

//	���茋��
class NodeStatus
{
public:
	NodeStatus() : m_status(-1) {}
	~NodeStatus() = default;

	NodeStatus(const int& status) { m_status = status; }
	NodeStatus(const NodeStatus& rhs) { m_status = rhs.m_status; }
	void operator=(const NodeStatus& rhs) { m_status = rhs.m_status; }

	bool operator==(const NodeStatus& rhs) { return m_status == rhs.m_status; }
	bool operator!=(const NodeStatus& rhs) { return !(*this == rhs); }

	static const NodeStatus INVALID;
	static const NodeStatus SUCCESS;
	static const NodeStatus FAILURE;
	static const NodeStatus RUNNING;

protected:
	int m_status;

};

//	���ی���(Condition�p)
class NodeJudge : public NodeStatus
{
public:
	NodeJudge() : NodeStatus(-1) {}
	virtual ~NodeJudge() = default;

	NodeJudge(const int status) { m_status = status; }
	NodeJudge(const NodeJudge& rhs) { m_status = rhs.m_status; }
	void operator=(const NodeJudge& rhs) { m_status = rhs.m_status; }

	static const NodeJudge INVALID;
	static const NodeJudge SUCCESS;
	static const NodeJudge FAILURE;

};

//	���ׂẴm�[�h�̊��N���X
class Node
{
public:
	Node() = default;
	virtual ~Node() = default;

	//	�Ăяo��
	virtual NodeStatus Invoke() = 0;

	//	RUNNING��Ԃ̃m�[�h�����
	virtual void ReleaseRunning() {}

};

//	��������p�m�[�h�̊��N���X
class ControlNode : public Node
{
public:
	ControlNode();
	virtual ~ControlNode() = default;

	void AddChild(Node* child);

	std::vector<std::unique_ptr<Node>>& GetChildren() { return m_children; }

private:
	//	�q�m�[�h���Ǘ�����
	std::vector<std::unique_ptr<Node>> m_children;

};

//	�D��x���Ɏq�m�[�h��T�����AFAILURE���o��܂Ŏ��s
class Sequence final : public ControlNode
{
public:
	Sequence();
	~Sequence() = default;

	NodeStatus Invoke() override final;

	void ReleaseRunning() override final;

};

//	�D��x���Ɏq�m�[�h��T�����A1�������s
class Selector final : public ControlNode
{
public:
	Selector();
	~Selector() = default;

	NodeStatus Invoke() override final;

	void ReleaseRunning() override final;

private:
	//	RUNNING��Ԃ̃m�[�h��ێ�
	Node* m_running;

};

//	���s����A�N�V�������Ǘ�
class Action final : public Node
{
public:
	Action(std::function<NodeStatus()> action);
	~Action() = default;

	NodeStatus Invoke() override final;

private:
	//	���s����A�N�V����
	std::function<NodeStatus()> m_action;

};

//	������ݒ肵�A���̐��ۂ�Ԃ�
class Condition final : public Node
{
public:
	Condition(std::function<NodeJudge()> condition);
	~Condition() = default;

	NodeStatus Invoke() override final;

	void  AddChild(Action* child);

private:
	//	�����𔻒�
	std::function<NodeJudge()> m_condition;
	//	Selector�Ŏg���ۂ̂��߂�
	//	Action�m�[�h��ێ��ł���悤�ɂ��Ă���
	std::unique_ptr<Action> m_children;

};
