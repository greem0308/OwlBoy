#include "StdAfx.h"
#include "actionManager.h"
#include "action.h"

actionManager::actionManager()
{
}


actionManager::~actionManager()
{
}

HRESULT actionManager::init()
{
	return S_OK;
}

void actionManager::release(void)
{
	for (_viAction = _vAction.begin(); _viAction != _vAction.end();)
	{
		if (_vAction.size() != 0)
		{
			(*_viAction)->release();
			SAFE_DELETE(*_viAction);
			_viAction = _vAction.erase(_viAction);
		}
		else ++_viAction;
	}

	_vAction.clear();
}

void actionManager::update(void)
{
	for (int i = 0; i < _vAction.size(); i++)
	{
		_vAction[i]->update();
		if (!_vAction[i]->getIsMoving())
		{
			_vAction[i]->release();
			SAFE_DELETE(_vAction[i]);
			_vAction.erase(_vAction.begin() + i);
			break;
		}
	}
}

void actionManager::render(void)
{

}

void actionManager::moveTo(image* img, float endX, float endY, float time)
{
	action* _ac = new action;
	_ac->init();
	_ac->moveTo(img,endX,endY,time);
	_vAction.push_back(_ac);
}

// callback
void actionManager::moveTo(image* img, float endX, float endY, float time, void* cbFuntion)
{
	action* _ac = new action;
	_ac->init();
	_ac->moveTo(img, endX, endY, time, (CALLBACK_FUNCTION)cbFuntion);
	_vAction.push_back(_ac);
}

//rewind
void actionManager::moveTo(image* img, float endX, float endY, float time, bool rewind)
{
	action* _ac = new action;
	_ac->init();
	_ac->moveTo(img, endX, endY, time, rewind);
	_vAction.push_back(_ac);
}

// Bezier 
void actionManager::moveToBezier(image* img, float endX, float endY, float time)
{
	action* _ac = new action;
	_ac->init();
	_ac->moveToBezier(img,endX,endY,time);
	_vAction.push_back(_ac);
}

// Bezier make
void actionManager::moveToBezier(image* img, float twoX, float twoY, float threeX, float threeY, float endX, float endY, float time)
{
	action* _ac = new action;
	_ac->init();
	_ac->moveToBezier(img,twoX,twoY,threeX, threeY, endX, endY, time);
	_vAction.push_back(_ac);
}

