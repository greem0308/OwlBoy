#include "stdafx.h"
#include "effectManager.h"


effectManager::effectManager()
{
}


effectManager::~effectManager()
{
}

HRESULT effectManager::init(void)
{
	gameNode::init();

	return S_OK;
}

void effectManager::release(void)
{
	gameNode::release();
}

void effectManager::update(void)
{
	gameNode::update();

	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion)
	{
		(*_viMinion)->update();
	}

}

void effectManager::render(void)
{
	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion)
	{
		(*_viMinion)->render();
	}
}

void effectManager::setPlayerEffect(float x, float y)
{
	effect* _playerEffect = new playerEffect;
	_playerEffect->init(x, y);
	_vMinion.push_back(_playerEffect);
}

void effectManager::removeMinion(int arrNum)
{
	SAFE_DELETE(_vMinion[arrNum]);
	_vMinion.erase(_vMinion.begin() + arrNum);
}