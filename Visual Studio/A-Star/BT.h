#pragma once

#include "NodeType.h"
#include "IActor.h"
#include <vector>
using namespace std;

class BT {
private:
	NodeType type;

	IActor* actor;
	int idAction;

	vector<BT*> children;

public:
	BT(IActor* actor, int idAction);
	BT(NodeType type);
	void addChild(BT* child);
	ValueBT eval();
};

