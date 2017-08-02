#include "stdafx.h"
#include "itemManager.h"
#include "item.h"


itemManager::itemManager()
{
}


itemManager::~itemManager()
{
}

HRESULT itemManager::init(void)
{
	gameNode::init();


	return S_OK;
}

void itemManager::release(void)
{
	gameNode::release();
}

void itemManager::update(void)
{
	gameNode::update();

	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		(*_viItem)->update();
	}
}

void itemManager::render(void)
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		(*_viItem)->render();
	}
}

void itemManager::removeItem(int arrNum)
{
	SAFE_DELETE(_vItem[arrNum]);
	_vItem.erase(_vItem.begin() + arrNum);
}

void itemManager::setFruit0(float x, float y)
{
	fruit0* _fruit0 = new fruit0;
	_fruit0->init(x, y);
	_fruit0->setPlayer(otus);
	_vItem.push_back(_fruit0);
}

void itemManager::setFruit1(float x, float y)
{
	fruit1* _fruit1 = new fruit1;
	_fruit1->init(x, y);
	_fruit1->setPlayer(otus);
	_vItem.push_back(_fruit1);
}

void itemManager::setFruit2(float x, float y)
{
	fruit2* _fruit2 = new fruit2;
	_fruit2->init(x, y);
	_fruit2->setPlayer(otus);
	_vItem.push_back(_fruit2);
}

void itemManager::setFruit3(float x, float y)
{
	fruit3* _fruit3 = new fruit3;
	_fruit3->init(x, y);
	_fruit3->setPlayer(otus);
	_vItem.push_back(_fruit3);
}

void itemManager::setGlassObject(float x, float y)
{
	glassObject* _glassObject = new glassObject;
	_glassObject->init(x, y);
	_glassObject->setPlayer(otus);
	_vItem.push_back(_glassObject);
}

void itemManager::setGlassObject2(float x, float y)
{
	glassObject2* _glassObject2 = new glassObject2;
	_glassObject2->init(x, y);
	_glassObject2->setPlayer(otus);
	_vItem.push_back(_glassObject2);
}