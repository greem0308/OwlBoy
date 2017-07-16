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
	// �� �� �̹���
	_enemy.image = IMAGEMANAGER->addImage("ship", "Scene/event/ship.bmp", 300*2.5, 300*2.5, true, RGB(255, 0, 255));
	_enemy.x = _enemy.fireX = x;
	_enemy.y = _enemy.fireY = y;
	_enemy.hitCheck = false; // ������
	_enemy.life = true;
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth(), 100);
	_enemy.Count = 0;
	_enemy.HP = 10;
	_enemy.AI = false;

	//��  ��.
	IMAGEMANAGER->addFrameImage("CannonBall", "Scene/event/CannonBall.bmp", 640, 656, 16, 16, true, RGB(255, 0, 255));
	//�� �ѱ� ��ٸ�. 
	IMAGEMANAGER->addFrameImage("canonWait", "Scene/event/canonWait.bmp", 900* 1.4, 2400* 1.4, 6, 16, true, RGB(255, 0, 255));
	//�� �ѱ� ��. 
	IMAGEMANAGER->addFrameImage("canonShoot", "Scene/event/canonShoot.bmp", 900 * 1.4, 2400 * 1.4, 6, 16, true, RGB(255, 0, 255));
	//�� �� ���ߴ� ��, 
	IMAGEMANAGER->addFrameImage("lightFind32", "Scene/event/lightFind32.bmp", 2560*1.8, 80*1.8, 32, 1, true, RGB(255, 0, 255));
	//�� �� 
	IMAGEMANAGER->addFrameImage("light64", "Scene/event/light64.bmp", 12800*3.8, 200*3.8, 64, 1, true, RGB(255, 0, 255));

	//���� �¾�
	_cannon.angle = PI / 2; 
	
	_cannon.cannon = 365; //�� ����. 
	_cannon.radius = 50;

	// �� enemy�� �߽���ǥ. 
	_cannon.center.x = _enemy.x; //�߽���.  
	_cannon.center.y = _enemy.y;

	// �� �ѱ��� ��ǥ.
	_cannon.shootCenter.x = 700;
	_cannon.shootCenter.y = 600;
	_cannon.shootAngle = PI;

	fire = false;

	_enemy._bullet = new bulletM1;
	_enemy._bullet->init(10,800);

	frameCount = 0; // �Ѿ� �������� �� ������. 
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

	// �Һ� ����,���� ��� ��.
	shipShoot(); 

	// �� ������ ������. 
	shipShake();

	// �ѱ� ���� �ִϸ��̼�. 
	gunFrameFunc();

}

void eventShip::render(void)
{
	IMAGEMANAGER->findImage("ship")->render(getMemDC(), _enemy.x - 330, _enemy.y - 350);
	
	HPEN MyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(getMemDC(), MyPen);

	//�Һ� ���� ��ġ �˷��ش�. 
	//LineMake(getMemDC(), _enemy.x, _enemy.y,
	//	_cannon.cannonEnd.x, _cannon.cannonEnd.y);
	//EllipseMakeCenter(getMemDC(), _enemy.x, _enemy.y,
	//	_cannon.radius, _cannon.radius);
	//
	//// ������� �ѱ� ������ġ �˷��ش�.
	//LineMake(getMemDC(), _cannon.shootCenter.x, _cannon.shootCenter.y,
	//	_cannon.shootCenterEnd.x, _cannon.shootCenterEnd.y);
	//EllipseMakeCenter(getMemDC(), _cannon.shootCenter.x, _cannon.shootCenter.y,
	//	_cannon.radius, _cannon.radius);
	//
	//// �Һ���Ʈ �׷��ش�. 
	//for (int i = 0; i < 4; i++)
	//{
	//	Ellipse(getMemDC(),lightRC[i].left, lightRC[i].top, lightRC[i].right, lightRC[i].bottom);
	//}

	IMAGEMANAGER->findImage("light64")->frameRender(getMemDC(), _enemy.x - 380, _enemy.y - 380, lightFrame64, 0);
	IMAGEMANAGER->findImage("lightFind32")->frameRender(getMemDC(), _enemy.x-65, _enemy.y-75, lightFrameNum, 0);

	_enemy._bullet->render();

	// �ѱ� �ִϸ��̼�
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

	// �Һ� ������ �˻�.
	//char str[128];
	//sprintf(str, "%d",lightFrameNum);
	//TextOut(getMemDC(), 670, 270, str, strlen(str));

	DeleteObject(MyPen);
}

void eventShip::shipShoot()
{
	// ����.  ���� * ���� * �߽���. 
	_cannon.cannonEnd.x = cosf(_cannon.angle) * _cannon.cannon + _enemy.x;
	_cannon.cannonEnd.y = -sinf(_cannon.angle) * _cannon.cannon + _enemy.y;

	// �ѱ� ����.
	_cannon.shootCenterEnd.x = cosf(_cannon.shootAngle) * _cannon.cannon + _cannon.shootCenter.x;
	_cannon.shootCenterEnd.y = -sinf(_cannon.shootAngle) * _cannon.cannon + _cannon.shootCenter.y;

	// ���� �� �ѱ� ��ġ ����. 
	_cannon.shootCenter.x = _enemy.x + 200;
	_cannon.shootCenter.y = _enemy.y;

	//�÷��̾�� ��¥ ��� �ѱ����� ������ ����. 
	_cannon.shootAngle = getAngle(_cannon.shootCenter.x, _cannon.shootCenter.y, otus->_player.x, otus->_player.y);

	// �Һ� ã��...
	lightFrameNum = int(_cannon.angle / PI_16); // 32���� 
	lightFrame64 = int(_cannon.angle / PI_32); // 64����
	gunCurrentY = int(_cannon.shootAngle / PI_8); // 16����

	if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;

	// �Һ� ���� ����.  // �� ���� �� ���ڷ� ���� ��ȭ�� �� �� �ְ� �� ����.
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

	// �÷��̾�� �Һ� ��Ʈ�� �浹�ϸ�,
	RECT rcTemp;
	for (int i = 0; i < 4; i++)
	{
		if (IntersectRect(&rcTemp, &lightRC[i], &otus->_player.rc))
		{
			//�÷��̾ ���󰡰��ؾ���. �׷����� �÷��̾���� ������ ���ؼ� �� ������ �Һ��� �̵�. 
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
			cannonFrameNum = int(_cannon.shootAngle / PI_8); // �ѱ� ������. 
			if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;
			if (_cannon.shootAngle >= PI * 2) _cannon.shootAngle -= PI * 2;
			_enemy._bullet->fire(_cannon.shootCenter.x, _cannon.shootCenter.y, _cannon.shootAngle, 10.0f);
			frameCount = 0;
			fire = false;
		}
	}

	// �Һ� ��Ʈ�� �׷��ֱ�. 
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


