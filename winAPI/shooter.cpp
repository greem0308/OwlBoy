#include "stdafx.h"
#include "shooter.h"


shooter::shooter()
{
}


shooter::~shooter()
{
}

HRESULT shooter::init(float x, float y)
{
	_enemy.image = IMAGEMANAGER->addFrameImage("shooter", "trinitas/enemy3_shooter.bmp", 150 * 3, 150 * 3, 3, 3, true);
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // ¸ÂÀ¸¸é
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), 100);
	_enemy.Count = 0;
	_enemy.HP = 10;
	_enemy.AI = false;
	_enemy.action = ENEMY_MOVE;
	
	Start = false;
	Count = 0;
	angle = PI;

	frameCount = 0;
	currentX = 0;

	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10, 300);
	stop = false;
	return S_OK;
}

void shooter::release(void)
{
	SAFE_DELETE(_enemy._bullet);
}

void shooter::update(void)
{
	_enemy._bullet->update();

	RECT rcTemp;

	if (!_enemy.hitCheck)
	{
		//PostQuitMessage(0);
		_enemy.x -= 1.5;
		_enemy.action = ENEMY_MOVE;
	}

	if (_enemy.x  < 680)
	{
		//stop = false;
		_enemy.action = ENEMY_ATTACK;

		_enemy.Count++;
		if (_enemy.Count % 70 == 0)
		{
			_enemy._bullet->fire(_enemy.x, _enemy.y, PI , 5);
		}
	}

	if (_enemy.hitCheck)
	{
		hitCount++;
		if (hitCount % 20 == 0)
		{
			//PostQuitMessage(0);
			_enemy.HP -= 2;
			_enemy.action = ENEMY_DIE;
			_enemy.hitCheck = false;
		}
	}

	if (_enemy.HP <= 0)
	{
		_enemy.HP = 0;
		_enemy.life = false;
	}

	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, 70, 100);
	_enemy.HPbar = RectMake(_enemy.x - 40, _enemy.y - 70, _enemy.HP * 8, 10);
}

void shooter::render(void)
{
	_enemy._bullet->render();
	IMAGEMANAGER->findImage("shooter")->frameRender(getMemDC(), _enemy.x - 90, _enemy.y - 90, currentX, _enemy.action);

	frameCount++;

	switch (_enemy.action)
	{
	case ENEMY_MOVE:
		if (frameCount > 30)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}

		break;
	case ENEMY_ATTACK:
		if (frameCount > 30)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 1)
			{
				currentX = 0;
			}
		}
		break;
	case ENEMY_DIE:
		if (frameCount > 30)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 1)
			{
				currentX = 0;
			}
		}
		break;
	default:
		break;
	}
}
