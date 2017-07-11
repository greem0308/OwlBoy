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