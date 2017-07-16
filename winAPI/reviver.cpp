#include "stdafx.h"
#include "reviver.h"


reviver::reviver()
{
}


reviver::~reviver()
{
}

HRESULT reviver::init(float x, float y, int length)
{
	_enemy.image = IMAGEMANAGER->addFrameImage("reviver", "dungeon/reviver.bmp", 1254*1.5,72 * 1.5, 19, 1, true,RGB(255,0,255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // 맞으면
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), _enemy.image->getFrameHeight());
	_enemy.Count = 0;
	_enemy.HP = 100000;
	_enemy.AI = false;
	_enemy.direction = ERIGHT;
	_enemy.angle = 0;

	_cannon.Length = length;

	_cannon.radius = 50; // 중심원 그려서 확인할때 필요.
	_cannon.angle = PI;

	// 얘가 쓰진 않지만 안해주면 터지네.
	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10,300);
	_enemy._bullet2 = new bulletM2;
	_enemy._bullet2->init(10,300);

	frameCount = 0;
	currentX = 0;

	stopFrameCount=0;
	stopCurrentX=0;

	stop = false;

	return S_OK;
}

void reviver::release(void)
{
}

void reviver::update(void)
{
	// 끝점.  각도 * 길이 * 중심점. 
	_cannon.cannonEnd.x = cosf(_cannon.angle) * _cannon.Length + _enemy.x;
	_cannon.cannonEnd.y = -sinf(_cannon.angle) * _cannon.Length + _enemy.y;

	if (!stop)
	{
		_cannon.angle += 0.01f;
	}

	stopFrameCount++;
	if (stopFrameCount > 120)
	{
		stop = true;
	}
	if (stopFrameCount > 210)
	{
		stop = false;
		stopFrameCount = 0;
	}

	frameFunc();

	_enemy.rc = RectMakeCenter(_cannon.cannonEnd.x, _cannon.cannonEnd.y, _enemy.image->getFrameWidth()/2, _enemy.image->getFrameHeight()/2);
}

void reviver::render(void)
{
	IMAGEMANAGER->findImage("reviver")->frameRender(getMemDC(), _cannon.cannonEnd.x-50, _cannon.cannonEnd.y-50, currentX,0);

	HPEN MyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(getMemDC(), MyPen);

	//각도 위치 알려준다. 
	LineMake(getMemDC(), _enemy.x, _enemy.y,
		_cannon.cannonEnd.x, _cannon.cannonEnd.y);
	EllipseMakeCenter(getMemDC(), _enemy.x, _enemy.y,
		_cannon.radius, _cannon.radius);

	char str[256];
	sprintf(str, "%0.2f", _enemy.angle);
	TextOut(getMemDC(),570, 38, str, strlen(str));

	DeleteObject(MyPen);
}

void reviver::frameFunc()
{
	frameCount++;
	if (frameCount > 15)
	{
		frameCount = 0;
		currentX++;
		if (currentX > 18)
		{
			currentX = 0;
		}
	}
}