#include "BT.h"
#include "NodeType.h"
#include <iostream>
using namespace std;

void BT::print(int offset) {
	if (this->type == NodeType::ACTION) {
		cout << string(offset, ' ').c_str() << "action(" << this->label.c_str() << ")" << endl;
	}
	else {
		string type = "";
		if (this->type == NodeType::SELECTOR) {
			type = "SELECTOR";
		}
		else if (this->type == NodeType::SEQUENCE) {
			type = "SEQUENCE";
		}
		cout << string(offset, ' ').c_str() << type.c_str() << "(" << this->label.c_str() << ")" << endl;

		for (BT* bt : this->children) {
			bt->print(offset + 2);
		}
	}
}

BT::BT(string label, IActor* actor, int idAction) {
	this->label = label;
	this->type = NodeType::ACTION;
	this->actor = actor;
	this->idAction = idAction;
}

BT::BT(string label, NodeType type) {
	this->label = label;
	this->type = type;
}

void BT::addChild(BT* child) {
	this->children.push_back(child);
}

ValueBT BT::eval() {
	ValueBT value = ValueBT::SUCCESS;

	if (this->type == NodeType::ACTION) {
		return this->actor->action(this->idAction);
	}
	else if (this->type == NodeType::SELECTOR) {
		for (BT* child : this->children) {
			value = child->eval();
			if (value != ValueBT::FAIL) {
				break;
			}
		}
	}
	else if (this->type == NodeType::SEQUENCE) {
		for (BT* child : this->children) {
			value = child->eval();
			if (value != ValueBT::SUCCESS) {
				break;
			}
		}
	}
	else {
		throw new exception("Node type not implemented!");
	}

	return value;
}

void BT::print() {
	this->print(0);
}