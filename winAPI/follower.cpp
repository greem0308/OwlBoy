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
	_enemy.image = IMAGEMANAGER->addFrameImage("followerRight", "dungeon/followerRight.bmp", 891*1.5,280 * 1.5, 11,4, true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("followerLeft", "dungeon/followerLeft.bmp", 891 * 1.5, 280 * 1.5, 11, 4, true,RGB(255,0,255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // 맞으면
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), _enemy.image->getFrameHeight());
	_enemy.Count = 0;
	_enemy.HP = 10;
	_enemy.AI = false;
	_enemy.direction = ERIGHT;
	_enemy.angle = PI / 2;

	// 얘가 쓰진 않지만 안해주면 터지네.
	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10, 300);
	_enemy._bullet2 = new bulletM2;
	_enemy._bullet2->init(10, 300);

	frameCount = 0;
	currentX = 0;
	fstate = FOLLOWER_SLEEP;

	wakeup = false;
	follow = false;
	hurt = false;

	speedX = 0;
	speedY = 0;

	wakeupFrameCount = 0;
	wakeupCurrentX=0;

	hitCount=0;

	dieFrameCount = 0;
	dieCurrentX = 0;

	hurtFrameCount = 0;
	hurtCurrentX = 0;

	fx0Init();

	return S_OK;
}

void follower::release(void)
{
}

void follower::update(void)
{
	fx0Update();

	int distance = getDistance(_enemy.x,_enemy.y,otus->_player.x,otus->_player.y);

	// 공격 범위 안에 있지않다면,
	if (distance > 300 && !wakeup)
	{
		fstate = FOLLOWER_SLEEP;
	}

	// 공격범위 내에 있다면 
	if (distance <= 300 )
	{
		wakeup = true;
	}

	if (wakeup && !follow && !_enemy.hitCheck)
	{
		wakeupFrameCount++;
		if (wakeupFrameCount > 0 && wakeupFrameCount < 100)
		{
			fstate = FOLLOWER_WAKEUP;
			speedY += 0.04f;
			_enemy.y += speedY;
		}
		if (wakeupFrameCount >= 100)
		{
			follow = true;
			wakeupFrameCount = 0;
		}
	}

	if (hurt)
	{
		fstate = FOLLOWER_DIE;
		hurtFrameCount++;
		if (hurtFrameCount > 30)
		{
			hurtFrameCount = 0;
			hurt = false;
		}
	}
	else if (follow && !_enemy.hitCheck)
	{
		
		fstate = FOLLOWER_FOLLOW;
		if (_enemy.x < otus->_player.x)
		{
			_enemy.x += 1.0f;
		}
		if (_enemy.x >= otus->_player.x)
		{
			_enemy.x -= 1.0f;
		}
		if (_enemy.y < otus->_player.y)
		{
			_enemy.y += 1.0f;
		}
		if (_enemy.y >= otus->_player.y)
		{
			_enemy.y -= 1.0f;
		}
	}

	//만약 player한테 맞으면,
	if (_enemy.hitCheck)
	{
		hitCount++;
		if (hitCount % 20 == 0)
		{
			//PostQuitMessage(0);
			_enemy.HP -= 1;
			fstate = FOLLOWER_DIE;
			hurt = true;
			otus->se7 = true;
			_enemy.hitCheck = false;
		}
	}

	if (_enemy.HP <= 0)
	{
		_enemy.HP = 0;
		fstate = FOLLOWER_DIE;
		dieFrameCount++;
		if (dieFrameCount > 50)
		{
			fx0.fire = true;
		}
	}
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth() / 3, _enemy.image->getFrameHeight() / 2);

	frameFunc();
}

void follower::render(void)
{
	//Rectangle(getMemDC(),_enemy.rc.left, _enemy.rc.top, _enemy.rc.right, _enemy.rc.bottom);
	if (_enemy.direction == ERIGHT)
	{
		IMAGEMANAGER->findImage("followerRight")->frameRender(getMemDC(), _enemy.x - 65, _enemy.y - 50, currentX, fstate);
	}
	if (_enemy.direction == ELEFT)
	{
		IMAGEMANAGER->findImage("followerLeft")->frameRender(getMemDC(), _enemy.x - 65, _enemy.y - 50, currentX, fstate);
	}

	fx0Render();
}

void follower::frameFunc(void)
{
	switch (fstate)
	{
	case FOLLOWER_FOLLOW:
		frameCount++;
		if (frameCount > 8)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 5)
			{
				currentX = 0;
			}
		}
		break;
	case FOLLOWER_SLEEP:
		frameCount++;
		if (frameCount > 10)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 1)
			{
				currentX = 0;
			}
		}
		break;
	case FOLLOWER_WAKEUP:
		frameCount++;
		if (frameCount > 9)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 10)
			{
				currentX = 0;
			}
		}
		break;

	case FOLLOWER_DIE:
		frameCount++;
		if (frameCount > 10)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 4)
			{
				currentX = 0;
			}
		}
		break;
	default:
		break;
	}
}


void follower::fx0Init(void)
{
	IMAGEMANAGER->addFrameImage("fx_0", "effect/fx_0.bmp",1000,100,10, 1, true, RGB(255, 0, 255));

	fx0.frameCount = 0;
	fx0.currentX = 0;
	fx0.fire = false;
}

void follower::fx0Update(void)
{
	if (fx0.fire)
	{
		fx0.frameCount++;
		if (fx0.frameCount > 8)
		{
			fx0.frameCount = 0;
			fx0.currentX++;
			if (fx0.currentX > 9)
			{
				fx0.currentX = 0;
				fx0.fire = false;

				_enemy.life = false;
			}
		}
	}

}

void follower::fx0Render(void)
{
	if (fx0.fire)
	{
		IMAGEMANAGER->findImage("fx_0")->frameRender(getMemDC(), _enemy.x - 50, _enemy.y - 60, fx0.currentX, 0);
	}
}
