/*/
 *  ファイル名		：	BehaviourTree.h
 *  概要			：	ビヘイビアツリーで扱うクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/13
/*/
#pragma once

//	判定結果
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

//	成否結果(Condition用)
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

//	すべてのノードの基底クラス
class Node
{
public:
	Node() = default;
	virtual ~Node() = default;

	//	呼び出し
	virtual NodeStatus Invoke() = 0;

	//	RUNNING状態のノードを解放
	virtual void ReleaseRunning() {}

};

//	挙動制御用ノードの基底クラス
class ControlNode : public Node
{
public:
	ControlNode();
	virtual ~ControlNode() = default;

	void AddChild(Node* child);

	std::vector<std::unique_ptr<Node>>& GetChildren() { return m_children; }

private:
	//	子ノードを管理する
	std::vector<std::unique_ptr<Node>> m_children;

};

//	優先度順に子ノードを探索し、FAILUREが出るまで実行
class Sequence final : public ControlNode
{
public:
	Sequence();
	~Sequence() = default;

	NodeStatus Invoke() override final;

	void ReleaseRunning() override final;

};

//	優先度順に子ノードを探索し、1つだけ実行
class Selector final : public ControlNode
{
public:
	Selector();
	~Selector() = default;

	NodeStatus Invoke() override final;

	void ReleaseRunning() override final;

private:
	//	RUNNING状態のノードを保持
	Node* m_running;

};

//	実行するアクションを管理
class Action final : public Node
{
public:
	Action(std::function<NodeStatus()> action);
	~Action() = default;

	NodeStatus Invoke() override final;

private:
	//	実行するアクション
	std::function<NodeStatus()> m_action;

};

//	条件を設定し、その成否を返す
class Condition final : public Node
{
public:
	Condition(std::function<NodeJudge()> condition);
	~Condition() = default;

	NodeStatus Invoke() override final;

	void  AddChild(Action* child);

private:
	//	条件を判定
	std::function<NodeJudge()> m_condition;
	//	Selectorで使う際のために
	//	Actionノードを保持できるようにしておく
	std::unique_ptr<Action> m_children;

};
