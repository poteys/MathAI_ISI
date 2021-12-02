#pragma once

#include "NodeType.h"
#include "IActor.h"
#include <vector>
#include <string>
using namespace std;

class BT {
private:
	string label;
	NodeType type;

	IActor* actor;
	int idAction;

	vector<BT*> children;
	void print(int offset);

public:
	BT(string label, IActor* actor, int idAction);
	BT(string label, NodeType type);
	void addChild(BT* child);
	ValueBT eval();

	void print();
};

