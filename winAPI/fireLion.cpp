#include "stdafx.h"
#include "fireLion.h"


fireLion::fireLion()
{
}


fireLion::~fireLion()
{
}

HRESULT fireLion::init(float x, float y)
{
	_enemy.image = IMAGEMANAGER->addFrameImage("fireLionRight", "dungeon/fireLionRight.bmp", 
		1344*1.5,1456 * 1.5, 12,13, true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("fireLionLeft", "dungeon/fireLionLeft.bmp", 1344 * 1.5, 1456 * 1.5, 12, 13, true,RGB(255, 0, 255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // 맞으면
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), _enemy.image->getFrameHeight());
	_enemy.Count = 0;
	_enemy.HP = 100000; // 총으로 죽지 않으므로.
	_enemy.AI = false;
	_enemy.direction = ERIGHT;
	_enemy.angle = PI / 2;

	IMAGEMANAGER->addFrameImage("fire16", "dungeon/fire16.bmp",1170 * 1.5,1248*1.5, 15,16, true, RGB(255, 0, 255));

	moveFrameCount = 0;
	moveRight = true;
	moveLeft = false;

	frameCount = 0;
	currentX = 0;

	flState = FL_IDLE;

	// 얘가 쓰진 않지만 안해주면 터지네.
	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10, 300);
	_enemy._bullet2 = new bulletM2;
	_enemy._bullet2->init(10, 300);

	lionAngle = PI;
	attack = false;
	attackFrameCount = 0;

	fireFrameCount=0;
	fireCurrentX=0;
	_enemy.stoned = false;
	return S_OK;
}

void fireLion::release(void)
{
}

void fireLion::update(void)
{
	int distance = getDistance(_enemy.x, _enemy.y, otus->_player.x, otus->_player.y);

	if (flState != FL_DIE)
	{
	//범위에 아직 플레이어가 들어오지 않았다면,
	if (distance > 100)
	{
		if (moveRight)
		{
		  	_enemy.direction = ERIGHT;
			_enemy.x += 1.5;
		}
		moveFrameCount++;
		if (moveFrameCount > 180)
		{
			moveRight = false;
			moveLeft = true;
			flState = FL_TURN;
		}
		if (moveLeft)
		{
			_enemy.x -= 1.5;
			_enemy.direction = ELEFT;
		}
		if (moveFrameCount > 360)
		{
			moveRight = true;
			moveLeft = false;
			moveFrameCount = 0;
			flState = FL_TURN;
		}
		flState = FL_WALK;
	}

	
		// 만약 사격거리에 들어오면, 
		if (distance <= 300)
		{
			// 플레이어와의 각도를 잡아서... 그 각도로 
			lionAngle = getAngle(_enemy.x, _enemy.y, otus->_player.x, otus->_player.y);
			attack = true;
		}

		if (attack)
		{
			// 만약 그 각도면, ,, 
			flState = FL_0A;
			// 그 각도의 상태로  그 각도의 불꽃 프레임으로 쏴야함.. 
			hitRC = RectMakeCenter(_enemy.x, _enemy.y - 100, 100, 50);
			attackFrameCount++;
			if (attackFrameCount > 60) // 1초만 때린다. 
			{
				attackFrameCount = 0;
				attack = false;
			}
		}
	}
	
	frameFunc();
	fireFrameFunc();

	// 물에 맞으면 
	if (_enemy.stoned)
	{
		flState = FL_DIE;
	}

	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth()/2, _enemy.image->getFrameHeight()/3);
}

void fireLion::render(void)
{
	if (_enemy.direction == ERIGHT)
	{
		IMAGEMANAGER->findImage("fireLionRight")->frameRender(getMemDC(), _enemy.x - 70, _enemy.y - 100, currentX, flState);
	}
	if (_enemy.direction == ELEFT)
	{
		IMAGEMANAGER->findImage("fireLionLeft")->frameRender(getMemDC(), _enemy.x - 70, _enemy.y - 100, currentX, flState);
	}

	if (attack)
	{
		//Rectangle(getMemDC(), hitRC.left, hitRC.top, hitRC.right, hitRC.bottom);
		IMAGEMANAGER->findImage("fire16")->frameRender(getMemDC(), _enemy.x-70, _enemy.y - 150, fireCurrentX,0);
	}
}


void fireLion::frameFunc(void)
{
	frameCount++;
	switch (flState)
	{
	case FL_NONE:
		break;

	case FL_TURN:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 4)
			{
				currentX = 0;
			}
		}
		break;

	case FL_WALK:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 11)
			{
				currentX = 0;
			}
		}
		break;

	case FL_IDLE:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

    // 각도 /////////////////////////////////////////////////////
	case FL_0A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

	case FL_20A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

	case Fl_40A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

	case FL_60A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

	case FL_80A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;

	case FL_90A:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 2)
			{
				currentX = 0;
			}
		}
		break;
	case FL_HURT:
		currentX = 0;
		break;
	case FL_DIE:
		currentX = 0;
		break;
	case FL_HURTDIE:
		currentX = 0;
		break;
	default:
		break;
	}
}


void fireLion::fireFrameFunc()
{
	fireFrameCount++;
	if (fireFrameCount > 15)
	{
		fireFrameCount = 0;
		fireCurrentX++;
		if (fireCurrentX > 14)
		{
			fireCurrentX = 0;
		}
	}
}