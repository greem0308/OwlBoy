#include "stdafx.h"
#include "follower.h"


follower::follower()
{
}


follower::~follower()
{
}

HRESULT follower::init(float x, float y)
{
	_enemy.image = IMAGEMANAGER->addFrameImage("followerRight", "dungeon/followerRight.bmp", 891,280, 11,4, true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("followerLeft", "dungeon/followerLeft.bmp", 891, 280, 11, 4, true,RGB(255,0,255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // ¸ÂÀ¸¸é
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), _enemy.image->getFrameHeight());
	_enemy.Count = 0;
	_enemy.HP = 10;
	_enemy.AI = false;
	_enemy.direction = ERIGHT;
	_enemy.angle = PI / 2;
	return S_OK;
}

void follower::release(void)
{
}

void follower::update(void)
{

}

void follower::render(void)
{
}