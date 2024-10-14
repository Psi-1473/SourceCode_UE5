#include "pch.h"
#include "BaseObject.h"

BaseObject::BaseObject()
{
}

BaseObject::~BaseObject()
{
	delete objectInfo;
	objectInfo = nullptr;
}
