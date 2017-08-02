#include "stdafx.h"
#include "etcObjectManager.h"
#include "etcObject.h"


etcObjectManager::etcObjectManager()
{
}


etcObjectManager::~etcObjectManager()
{
}

HRESULT etcObjectManager::init(void)
{
	gameNode::init();


	return S_OK;
}

void etcObjectManager::release(void)
{
	gameNode::release();
}

void etcObjectManager::update(void)
{
	gameNode::update();

	for (_viObj = _vObj.begin(); _viObj != _vObj.end(); ++_viObj)
	{
		(*_viObj)->update();
	}
}

void etcObjectManager::render(void)
{
	for (_viObj = _vObj.begin(); _viObj != _vObj.end(); ++_viObj)
	{
		(*_viObj)->render();
	}
}

void etcObjectManager::removeObj(int arrNum)
{
	SAFE_DELETE(_vObj[arrNum]);
	_vObj.erase(_vObj.begin() + arrNum);
}

