#include "BT.h"
#include "NodeType.h"

BT::BT(IActor* actor, int idAction) {
	this->type = NodeType::ACTION;
	this->actor = actor;
	this->idAction = idAction;
}

BT::BT(NodeType type) {
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
