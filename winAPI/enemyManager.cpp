#include "stdafx.h"
#include "enemyManager.h"
#include "enemy.h"

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init(void)
{
	gameNode::init();


	return S_OK;
}

void enemyManager::release(void)
{
	gameNode::release();
}

void enemyManager::update(void)
{
	gameNode::update();

	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion)
	{
		(*_viMinion)->update();
	}
}

void enemyManager::render(void)
{
	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion)
	{
		(*_viMinion)->render();
	}
}

void enemyManager::removeMinion(int arrNum)
{
	SAFE_DELETE(_vMinion[arrNum]);
	_vMinion.erase(_vMinion.begin() + arrNum);
}

void enemyManager::setShooter(float x, float y)
{
	shooter* _shooter = new shooter;
	_shooter->init(x, y);
	_shooter->setPlayer(otus);
	_vMinion.push_back(_shooter);
}

void enemyManager::setEventShip(float x, float y)
{
	eventShip* _eventShip = new eventShip;
	_eventShip->init(x, y);
	_eventShip->setPlayer(otus);
	_vMinion.push_back(_eventShip);
}

