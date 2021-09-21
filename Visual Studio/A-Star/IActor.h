#pragma once

#include "ValueBT.h"

class IActor {
public:
	virtual ValueBT action(int idAction) = 0;
};