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
	_enemy.image = IMAGEMANAGER->addFrameImage("shooterRight", "dungeon/shooterRight.bmp",600*1.5,350 * 1.6, 6, 5, true);
	IMAGEMANAGER->addFrameImage("shooterLeft", "dungeon/shooterLeft.bmp", 600 * 1.6, 350 * 1.6, 6, 5, true);
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
	
	shooterState = SHOOTER_IDLE;

	Start = false;
	Count = 0;
	angle = PI;

	frameCount = 0;
	currentX = 0;

	_enemy._bullet2 = new bulletM2;
	_enemy._bullet2->init(10, 700);
	stop = false;

	waitFrameCount = 0;

	moveFrameCount = 0;
	moveRight = true;
	moveLeft = false;

	attack = false;
	attackFrameCount = 0;

	waitbool = true;

	fx0Init();
	return S_OK;
}

void shooter::release(void)
{
	SAFE_DELETE(_enemy._bullet2);
}

void shooter::update(void)
{
	fx0Update();
	_enemy._bullet2->update();
	
	RECT rcTemp;

	_enemy.angle = getAngle(_enemy.x,_enemy.y, otus->_player.x, otus->_player.y);

	// 플레이어가 일정거리 내에 들어오면 공격한다.
	int distance = getDistance(_enemy.x, _enemy.y, otus->_player.x, otus->_player.y);
	if (distance < 400 && !_enemy.hitCheck)
	{
		if (_enemy.x > otus->_player.x)
		{
			_enemy.direction = ELEFT;
		}
		else { _enemy.direction = ERIGHT; }

		attack = true;
		if (waitbool)
		{
			shooterState = SHOOTER_WAIT;
		}
	}

	// 공격 
	if (attack)
	{
		waitFrameCount++;
		if (waitFrameCount > 150)
		{
			waitbool = false;
			shooterState = SHOOTER_ATTACK;
			_enemy._bullet2->fire(_enemy.x, _enemy.y, _enemy.angle, 7.0f);
			waitFrameCount = 0;   
			attack = false;
		}
		attackFrameCount++; // 공격 모션이 한번만 들어오고 나가서 작동이 안되서 해놓은 장치. 
		if (attackFrameCount > 90)
		{
			waitbool = true;
			attackFrameCount = 0;
		}
	}

	//맞는 상태가 아니고 범위에 아직 플레이어가 들어오지 않았다면,
	if (!_enemy.hitCheck && distance > 400)
	{
		if (moveRight)
		{
			_enemy.direction = ERIGHT;
			_enemy.x += 1.5;
		}
		moveFrameCount++;
		if (moveFrameCount > 120)
		{
			moveRight = false;
			moveLeft = true;
		}
		if (moveLeft)
		{
			_enemy.x -= 1.5;
			_enemy.direction = ELEFT;
		}
		if (moveFrameCount > 240)
		{
			moveRight = true;
			moveLeft = false;
			moveFrameCount = 0;
		}
		shooterState = SHOOTER_MOVE;
	}

	//만약 player한테 맞으면,
	if (_enemy.hitCheck)
	{
		hitCount++;
		if (hitCount % 20 == 0)
		{
			//PostQuitMessage(0);
			_enemy.HP -= 1;
			shooterState = SHOOTER_DIE;
			otus->se7 = true;
			_enemy.hitCheck = false;
		}
	}

	if (_enemy.HP <= 0)
	{
		_enemy.HP = 0;
		shooterState = SHOOTER_DIE;
		fx0.fire = true;
	}
	
	if (fx0.fire)
	{
		fx0.frameCount++;
		if (fx0.frameCount > 8)
		{
			fx0.frameCount = 0;
			fx0.currentX++;
			if (fx0.currentX > 14)
			{
				fx0.currentX = 0;
				fx0.fire = false;
				_enemy.life = false;
			}
		}
	}
	
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth()/3, _enemy.image->getFrameHeight()/2);
}

void shooter::render(void)
{
	_enemy._bullet2->render();

	if (_enemy.direction == ERIGHT)
	{
		IMAGEMANAGER->findImage("shooterRight")->frameRender(getMemDC(), _enemy.x - 80, _enemy.y - 70, currentX, shooterState);
	}
	if (_enemy.direction == ELEFT)
	{
		IMAGEMANAGER->findImage("shooterLeft")->frameRender(getMemDC(), _enemy.x - 80, _enemy.y - 70, currentX, shooterState);
	}

	fx0Render();

	frameCount++;
	switch (shooterState)
	{
	case SHOOTER_IDLE:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 5)
			{
				currentX = 0;
			}
		}

		break;
	case SHOOTER_MOVE:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 5)
			{
				currentX = 0;
			}
		}
		break;
	case SHOOTER_WAIT:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 1)
			{
				currentX = 0;
			}
		}
		break;
	case SHOOTER_ATTACK:
		if (frameCount > 15)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 5)
			{
				currentX = 0;
			}
		}
		break;
	case SHOOTER_DIE:
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



void shooter::fx0Init(void)
{
	IMAGEMANAGER->addFrameImage("explosionEffect1","effect/explosionEffect1.bmp",1872,128,18,1,true,RGB(255,0,255));
	
	fx0.frameCount = 0;
	fx0.currentX = 0;
	fx0.fire = false;
}

void shooter::fx0Update(void)
{
	if (fx0.fire)
	{
		fx0.frameCount++;
		if (fx0.frameCount > 8)
		{
			fx0.frameCount = 0;
			fx0.currentX++;
			if (fx0.currentX > 17)
			{
				fx0.currentX = 0;
				fx0.fire = false;
				_enemy.life = false;
			}
		}
	}

}

void shooter::fx0Render(void)
{
	if (fx0.fire)
	{
		IMAGEMANAGER->findImage("explosionEffect1")->frameRender(getMemDC(), _enemy.x-65, _enemy.y-70, fx0.currentX, 0);
	}
}
