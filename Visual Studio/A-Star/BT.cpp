#include "BT.h"
#include "NodeType.h"
#include <iostream>
#include <exception>
using namespace std;

#define ASSERT(predicat, errorMessage) if (!(predicat)) { throw exception(errorMessage); }

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
		else if (this->type == NodeType::INVERTER) {
			type = "INVERTER";
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
		ASSERT(this->children.size() > 0, "SELECTOR node must have at least one child");

		for (BT* child : this->children) {
			value = child->eval();
			if (value != ValueBT::FAIL) {
				break;
			}
		}
	}
	else if (this->type == NodeType::SEQUENCE) {
		ASSERT(this->children.size() > 0, "SEQUENCE node must have at least one child");

		for (BT* child : this->children) {
			value = child->eval();
			if (value != ValueBT::SUCCESS) {
				break;
			}
		}
	}
	else  if (this->type == NodeType::INVERTER) {
		ASSERT(this->children.size() == 1, "INVERTER node must have one child");

		value = this->children[0]->eval();
		if (value == ValueBT::FAIL) { value = ValueBT::SUCCESS; }
		else if (value == ValueBT::SUCCESS) { value = ValueBT::FAIL; }
	}
	else {
		throw new exception("Node type not implemented!");
	}

	return value;
}

void BT::print() {
	this->print(0);
}