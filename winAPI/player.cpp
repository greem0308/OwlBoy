#include "stdafx.h"
#include "player.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init(void)
{
	gameNode::init();

	this->setHP(DATABASE->getElementData("player")->hp);

	_player.x = WINSIZEX/2;
	_player.y = WINSIZEY / 2;
	_player.rc = RectMakeCenter(_player.x,_player.y,50,50);
	_player.angle = PI;
	_player.speed = 4;
	_player.gravity = 30.0f;
	_player.groundgrv = 0; 
	_player.jump = false;
	_player.jumpCount = 0;
	//_player.hp = 0;
	//_player.coin = 0;
	_player.bullet = new missileM1;
	_player.bullet->init(10,300);

	return S_OK;
}

void player::release(void)
{
	gameNode::release();
}

void player::update(void)
{
	gameNode::update();
	_player.rc = RectMakeCenter(_player.x, _player.y, 50, 50);
	keyControl();
}

void player::render(void)
{
	Rectangle(getMemDC(),_player.rc.left, _player.rc.top, _player.rc.right, _player.rc.bottom);
}

void player::keyControl(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_player.x -= _player.speed;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_player.x += _player.speed;
	}
}
