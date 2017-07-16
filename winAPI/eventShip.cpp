#include "stdafx.h"
#include "eventShip.h"


eventShip::eventShip()
{
}


eventShip::~eventShip()
{
}


HRESULT eventShip::init(float x, float y)
{
	// 적 배 이미지
	_enemy.image = IMAGEMANAGER->addImage("ship", "Scene/event/ship.bmp", 300*2.5, 300*2.5, true, RGB(255, 0, 255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // 맞으면
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), 100);
	_enemy.Count = 0;
	_enemy.HP = 10;
	_enemy.AI = false;

	//적  돌.
	IMAGEMANAGER->addFrameImage("CannonBall", "Scene/event/CannonBall.bmp", 640, 656, 16, 16, true, RGB(255, 0, 255));
	//적 총구 기다림. 
	IMAGEMANAGER->addFrameImage("canonWait", "Scene/event/canonWait.bmp", 900* 1.4, 2400* 1.4, 6, 16, true, RGB(255, 0, 255));
	//적 총구 슛. 
	IMAGEMANAGER->addFrameImage("canonShoot", "Scene/event/canonShoot.bmp", 900 * 1.4, 2400 * 1.4, 6, 16, true, RGB(255, 0, 255));
	//적 빛 비추는 얘, 
	IMAGEMANAGER->addFrameImage("lightFind32", "Scene/event/lightFind32.bmp", 2560*1.8, 80*1.8, 32, 1, true, RGB(255, 0, 255));
	//적 빛 
	IMAGEMANAGER->addFrameImage("light64", "Scene/event/light64.bmp", 12800*3.8, 200*3.8, 64, 1, true, RGB(255, 0, 255));

	//대포 셋업
	_cannon.angle = PI / 2; 
	
	_cannon.cannon = 365; //선 길이. 
	_cannon.radius = 50;

	// 이 enemy의 중심좌표. 
	_cannon.center.x = _enemy.x; //중심점.  
	_cannon.center.y = _enemy.y;

	// 쏠 총구의 좌표.
	_cannon.shootCenter.x = 700;
	_cannon.shootCenter.y = 600;
	_cannon.shootAngle = PI;

	fire = false;

	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10,800);

	frameCount = 0; // 총알 날리기전 텀 프레임. 
	currentX = 0;

	anglefirst = true;
	angleReturn = false;

	playerToShipAngle = 0;
	lightFollowFrame = 0;

	cannonFrameNum = 8;
	lightFrameNum = 0;
	lightFrame64 = 0;

	shipShakeFrame = 0;
	shipShakeUp = true;
	shipShakeDown = false;

	gunState=gunIDLE;
	gunFrameCount=0;
	gunCurrentX=0; 
	gunCurrentY=0; 

	return S_OK;
}

void eventShip::release(void)
{

}

void eventShip::update(void)
{
	_enemy._bullet->update();

	// 불빛 돌고,각도 잡아 쏨.
	shipShoot(); 

	// 배 오르락 내리락. 
	shipShake();

	// 총구 가로 애니메이션. 
	gunFrameFunc();

}

void eventShip::render(void)
{
	IMAGEMANAGER->findImage("ship")->render(getMemDC(), _enemy.x - 330, _enemy.y - 350);
	
	HPEN MyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(getMemDC(), MyPen);

	//불빛 각도 위치 알려준다. 
	//LineMake(getMemDC(), _enemy.x, _enemy.y,
	//	_cannon.cannonEnd.x, _cannon.cannonEnd.y);
	//EllipseMakeCenter(getMemDC(), _enemy.x, _enemy.y,
	//	_cannon.radius, _cannon.radius);
	//
	//// 직접쏘는 총구 각도위치 알려준다.
	//LineMake(getMemDC(), _cannon.shootCenter.x, _cannon.shootCenter.y,
	//	_cannon.shootCenterEnd.x, _cannon.shootCenterEnd.y);
	//EllipseMakeCenter(getMemDC(), _cannon.shootCenter.x, _cannon.shootCenter.y,
	//	_cannon.radius, _cannon.radius);
	//
	//// 불빛렉트 그려준다. 
	//for (int i = 0; i < 4; i++)
	//{
	//	Ellipse(getMemDC(),lightRC[i].left, lightRC[i].top, lightRC[i].right, lightRC[i].bottom);
	//}

	IMAGEMANAGER->findImage("light64")->frameRender(getMemDC(), _enemy.x - 380, _enemy.y - 380, lightFrame64, 0);
	IMAGEMANAGER->findImage("lightFind32")->frameRender(getMemDC(), _enemy.x-65, _enemy.y-75, lightFrameNum, 0);

	_enemy._bullet->render();

	// 총구 애니메이션
	if (gunState == gunIDLE)
	{
		IMAGEMANAGER->findImage("canonWait")->frameRender(getMemDC(), _enemy.x + 105,
			_enemy.y - 100, gunCurrentX, gunCurrentY);
	}
	if (gunState == gunWAIT)
	{
		IMAGEMANAGER->findImage("canonWait")->frameRender(getMemDC(), _enemy.x + 105,
			_enemy.y - 100, gunCurrentX, gunCurrentY);
	}
	if (gunState == gunSHOOT)
	{
		IMAGEMANAGER->findImage("canonShoot")->frameRender(getMemDC(), _enemy.x + 105,
			_enemy.y - 100, gunCurrentX, gunCurrentY);
	}

	// 불빛 프레임 검사.
	//char str[128];
	//sprintf(str, "%d",lightFrameNum);
	//TextOut(getMemDC(), 670, 270, str, strlen(str));

	DeleteObject(MyPen);
}

void eventShip::shipShoot()
{
	// 끝점.  각도 * 길이 * 중심점. 
	_cannon.cannonEnd.x = cosf(_cannon.angle) * _cannon.cannon + _enemy.x;
	_cannon.cannonEnd.y = -sinf(_cannon.angle) * _cannon.cannon + _enemy.y;

	// 총구 끝점.
	_cannon.shootCenterEnd.x = cosf(_cannon.shootAngle) * _cannon.cannon + _cannon.shootCenter.x;
	_cannon.shootCenterEnd.y = -sinf(_cannon.shootAngle) * _cannon.cannon + _cannon.shootCenter.y;

	// 직접 쏠 총구 위치 지정. 
	_cannon.shootCenter.x = _enemy.x + 200;
	_cannon.shootCenter.y = _enemy.y;

	//플레이어와 진짜 쏘는 총구와의 각도를 구함. 
	_cannon.shootAngle = getAngle(_cannon.shootCenter.x, _cannon.shootCenter.y, otus->_player.x, otus->_player.y);

	// 불빛 찾는...
	lightFrameNum = int(_cannon.angle / PI_16); // 32방향 
	lightFrame64 = int(_cannon.angle / PI_32); // 64방향
	gunCurrentY = int(_cannon.shootAngle / PI_8); // 16방향

	if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;

	// 불빛 각도 제한.  // 다 만든 후 인자로 빼서 변화를 줄 수 있게 할 예정.
	if (anglefirst)_cannon.angle += 0.01f;
	if (_cannon.angle > PI + PI / 4)
	{
		anglefirst = false;
		angleReturn = true;
	}
	if (angleReturn) _cannon.angle -= 0.01f;
	if (_cannon.angle < PI / 2)
	{
		anglefirst = true;
		angleReturn = false;
	}

	// 플레이어와 불빛 렉트가 충돌하면,
	RECT rcTemp;
	for (int i = 0; i < 4; i++)
	{
		if (IntersectRect(&rcTemp, &lightRC[i], &otus->_player.rc))
		{
			//플레이어를 따라가게해야함. 그러려면 플레이어와의 각도를 구해서 그 각도로 불빛이 이동. 
			playerToShipAngle = getAngle(otus->_player.x, otus->_player.y, _enemy.x, _enemy.y);

			if (_cannon.angle < playerToShipAngle)
			{
				angleReturn = true;
				anglefirst = false;
			}
			if (_cannon.angle > playerToShipAngle)
			{
				angleReturn = false;
				anglefirst = true;
			}
			fire = true;
			break;
		}
	}

	if (fire)
	{
		gunState = gunWAIT;
		frameCount++;
		if (frameCount >= 100)
		{
			gunState = gunSHOOT;
			cannonFrameNum = int(_cannon.shootAngle / PI_8); // 총구 프레임. 
			if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;
			if (_cannon.shootAngle >= PI * 2) _cannon.shootAngle -= PI * 2;
			_enemy._bullet->fire(_cannon.shootCenter.x, _cannon.shootCenter.y, _cannon.shootAngle, 10.0f);
			frameCount = 0;
			fire = false;
		}
	}

	// 불빛 렉트들 그려주기. 
	lightRC[0] = RectMakeCenter(_cannon.cannonEnd.x, _cannon.cannonEnd.y, 110, 110);
	lightRC[1] = RectMakeCenter(cosf(_cannon.angle) * 260 + _enemy.x,
		-sinf(_cannon.angle) * 260 + _enemy.y, 90, 90);
	lightRC[2] = RectMakeCenter(cosf(_cannon.angle) * 180 + _enemy.x,
		-sinf(_cannon.angle) * 180 + _enemy.y, 60, 60);
	lightRC[3] = RectMakeCenter(cosf(_cannon.angle) * 120 + _enemy.x,
		-sinf(_cannon.angle) * 120 + _enemy.y, 50, 50);
}

void eventShip::shipShake()
{
	if (shipShakeUp && !shipShakeDown)
	{
		_enemy.y -= 0.25f;
	}
	if (shipShakeDown && !shipShakeUp)
	{
		_enemy.y += 0.25f;
	}

	shipShakeFrame++;
	if (shipShakeFrame > 220)
	{
		shipShakeUp = false;
		shipShakeDown = true;
	}
	if (shipShakeFrame < 220)
	{
		shipShakeUp = true;
		shipShakeDown = false;
	}
	if (shipShakeFrame > 440)
	{
		shipShakeFrame = 0;
	}
}

void eventShip::gunFrameFunc(void)
{
	switch (gunState)
	{
	case gunIDLE:
		gunCurrentX = 0;
		break;

	case gunWAIT:
		gunFrameCount++;
		if (gunFrameCount > 5)
		{
			gunFrameCount = 0;
			gunCurrentX++;
			if (gunCurrentX > 5)
			{
				gunCurrentX = 0;
			}
		}
		break;

	case gunSHOOT:
		gunFrameCount++;
		if (gunFrameCount > 15)
		{
			gunFrameCount = 0;
			gunCurrentX++;
			if (gunCurrentX > 5)
			{
				gunCurrentX = 0;
				gunState = gunIDLE;
			}
		}
		break;
	default:
		break;
	}
}


