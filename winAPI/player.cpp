#include "stdafx.h"
#include "player.h"

player::player()
{
}


player::~player()
{
}

HRESULT player::init(float x, float y)
{
	gameNode::init();
	//sound ____________________________________________________________________________________________
	soundInit();
	soundInit1();
	//inventory____________________________________________________________________________________________
	invenInit();

	//player____________________________________________________________________________________________
	this->setCurrentHP(DATABASE->getElementData("player")->currentHP);
	this->setMaxHP(DATABASE->getElementData("player")->maxHP);
	this->setSpeed(DATABASE->getElementData("player")->speed);
	this->setShootSpeed(DATABASE->getElementData("player")->shootSpeed);
	this->setCoin(DATABASE->getElementData("player")->coin);
	this->setsoundOpen(DATABASE->getElementData("player")->soundOpen);
	this->setinventoryOpen(DATABASE->getElementData("player")->inventoryOpen);

	_player.imgR =IMAGEMANAGER->addFrameImage("playerR", "player/playerRight.bmp", (100 * 20)*1.8, (100 * 25)*1.8, 20, 25, true, RGB(255, 0, 255));
	_player.imgL = IMAGEMANAGER->addFrameImage("playerL", "player/playerLeft.bmp", (100 * 20) * 1.8, (100 * 25) * 1.8, 20, 25, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addImage("bullet","player/bullet.bmp",50,50,true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("armGun", "player/armGun.bmp", 50*16*3, 50*5 *3,16,6, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("geddyR","player/geddyRight.bmp",100 *6 * 1.5 ,100 * 3 * 1.5,6,3,true,RGB(255,0,255));
	//IMAGEMANAGER->addFrameImage("geddyL", "player/geddyLeft.bmp", 100 * 6 * 1.5, 100 * 3 * 1.5, 6, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("geddy", "player/geddy.bmp", 100 * 6 * 1.4, 100 * 8 * 1.4, 6, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hp", "UI/hp.bmp",85,17, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("maxHP", "UI/maxHP.bmp", 85, 17, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBar", "UI/hpBar.bmp", 140, 50, true, RGB(255, 0, 255));

	// curve Point
	IMAGEMANAGER->addImage("curvePoint", "player/curvePoint.bmp",8,8, true, RGB(255, 0, 255));

	// coin icon
	IMAGEMANAGER->addImage("coinIcon", "UI/coinIcon.bmp",15*1.8,17 * 1.8, true, RGB(255, 0, 255));

	// player __________________________________________________________________________________________________________
	_player.x = x;
	_player.y = y;
	
	// database �� �س����� �� �ʱ�ȭ�ϸ� �ȵǴϱ� �ּ�ó���س���. 
	//= 0;
	//_player.coin = 0;
	//_player.speed = 4;
	
	_player.rc = RectMakeCenter(_player.x,_player.y, _player.imgR->getFrameWidth()/3, _player.imgR->getFrameHeight()/3);
	_player.angle = PI;

	_player.gravity = 30.0f;
	_player.groundgrv = 0; 
	_player.jump = false;
	_player.jumpCount = 0;
	_player.life = true;

	// �� �ൿ�� �� ����. 
	_player.fly = false;
	_player.fall = false;
	_player.turn = false;
	_player.flyTurn = false;

	_player.state = IDLE;
	_player.direction = RIGHT; 
	_player.shootState = NONESHOOT;

	_player.frameCount = 0;
	_player.currentX = 0;
	_player.shootFrameCount = 0;
	_player.shootCurrentY = 0;

	_player._fire = new missileM1;
	_player._fire->init(10, 350);

	// ���� �÷��̾�� ���, �Ե� �������� ��Ʈ�� �ʿ��ؼ� ����ü�� �����Ͽ���.
	// geddy __________________________________________________________________________________________________________
	geddy.x = 500;
	geddy.y = 200;
	geddy.radius = 25;
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, 50, 50);
	geddy.follow = false; // �÷��̾ ����ٴϳ�?
	geddy.bodyFrameX = 0;  // �Ե� �� ���� ������X.  
	geddy.bodyFrameY = 0; //y��
	geddy.gunFrameX = 0; // �� 16����
	geddy.gunFrameY = 0; //y��
	geddy.gunX = 0;
	geddy.gunY = 0;
	geddy.gunRadius = 10;
	geddy.gunRC = RectMakeCenter(50,50,20,20); 
	geddy.angle = PI; // ���������� ���������� �ޱۺ���. 
	geddy.speed = 12; // ������ ���󰡴� �ӵ�.  �ӵ��� ���ϼ��� castGravity���� �������Ѵ�.
	geddy.cast = false; //������?
	geddy.castGravity = 0; // ���� ������ų �߷�.
	geddy.geddyState = gNONE;
	geddy.frameCount = 0; //�Ե� �� ���� ������.  
	geddy.ground = false;
	geddy.gravity = 30.0f;
	geddy.groundgrv = 0;
	geddy.showCurve = false; // Ŀ�� �����ٶ� ���� ��.

	_hpBar = new progressBar;
	_hpBar->init(70,35);

	miniX = 0;
	miniY = 0; 

	//effectInit();

	//_mp3Player = new mp3Player;
	//_mp3Player->init();

    return S_OK;
}

void player::release(void)
{
	gameNode::release();
}

void player::update(void)
{
	gameNode::update();
	_player._fire->update();
	soundBtn->update();
	inventoryBtn->update();
	KEYANIMANAGER->update();
	//_mp3Player->update();

	// ���콺 �ִϸ��̼��� ���� ���.
	if(!geddy.follow) mouseGun = false;
	if (geddy.follow) mouseGun = true;
	if (!geddy.showCurve)GeddyThrow = false;
	if (geddy.showCurve)GeddyThrow = true;
	
	// �� ��ġ.
	geddy.gunX = geddy.x;
	geddy.gunY = geddy.y;
	// �÷��̾�, �Ե� ��Ʈ, �� ��Ʈ
	_player.rc = RectMakeCenter(_player.x, _player.y, _player.imgR->getFrameWidth() / 4, _player.imgR->getFrameHeight() / 4);
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, geddy.radius * 2, geddy.radius * 2);
	geddy.gunRC = RectMakeCenter(geddy.gunX, geddy.gunY, geddy.gunRadius * 2, geddy.gunRadius * 2);

	//player ___________________________________________________________________
	keyControl();
	jump();
	PixelCollision();
	frameFunc();

	//geddy _____________________________________________________________________
	geddyFunc(); //�Ե� �ȵ� ������, �ѽ�. 
	geddyPixelCollision();
	geddyFrameFunc(); // �Ե� �� ������. 
	CurveLineFunc(); // ������ Ŀ�� ���� �׸��� �Լ�
	geddyCastFunc(); // ������ ����,Ű��Ʈ��.

	//sound_inven_________________________________________________________________
	soundUpdate();
	soundUpdate1();

	invenUpdate();

	//effectUpdate();

	if (_player.currentHP > _player.maxHP)
	{
		_player.currentHP = _player.maxHP;
	}
	
	// �÷��̾ �Ųٷ� �־��ش�. �׷��� ����, �б� �� �Ǵ¼�. 
	//�÷��̾ ��ӹ޾Ƽ� �����Ҵ��� ��� ���鿡��
	_player.currentHP = DATABASE->getElementData("player")->currentHP;
	_player.maxHP = DATABASE->getElementData("player")->maxHP;
	_player.speed = DATABASE->getElementData("player")->speed;
	_player.shootSpeed = DATABASE->getElementData("player")->shootSpeed;
	_player.coin = DATABASE->getElementData("player")->coin;

	// hpBar ����ġ, �ִ�ġ.
	_hpBar->setGuage(_player.currentHP,_player.maxHP);

	//gunPixelCollision();
}

void player::render(void)
{
	//Rectangle(getMemDC(),_player.rc.left, _player.rc.top, _player.rc.right, _player.rc.bottom);
	//Rectangle(getMemDC(), geddy.rc.left, geddy.rc.top, geddy.rc.right, geddy.rc.bottom);
	//Rectangle(getMemDC(), geddy.gunRC.left, geddy.gunRC.top, geddy.gunRC.right, geddy.gunRC.bottom);
	//_mp3Player->render();

	_player._fire->render();

	//effectRender();

	if (_player.direction == RIGHT)
	{
		IMAGEMANAGER->findImage("playerR")->frameRender(getMemDC(), _player.rc.left - 70, _player.rc.top - 72,
			_player.currentX, _player.state);
	}
	if (_player.direction == LEFT)
	{
		IMAGEMANAGER->findImage("playerL")->frameRender(getMemDC(), _player.rc.left - 75, _player.rc.top - 72,
			_player.currentX, _player.state);
	}
	IMAGEMANAGER->findImage("geddy")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top - 60, geddy.bodyFrameX, geddy.bodyFrameY);

	IMAGEMANAGER->findImage("armGun")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top-40, geddy.gunFrameX, _player.shootCurrentY);

	//hp
	IMAGEMANAGER->findImage("hpBar")->render(getMemDC(), 20, 20);
	_hpBar->render();

	char str[256];

	
	SetTextAlign(getMemDC(), TA_LEFT);
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(255,255,255));
	HFONT myFont = CreateFont(28, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("System"));
	HFONT hpFont = CreateFont(8, 0, 0, 0, 2, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("System"));
	SelectObject(getMemDC(), hpFont);

	//currentHP
	sprintf(str, "%d", _player.currentHP);
	TextOut(getMemDC(), 70, 38, str, strlen(str));

	//maxHp
	sprintf(str, "%d", _player.maxHP);
	TextOut(getMemDC(), 127, 38, str, strlen(str));


	SelectObject(getMemDC(), myFont);
	//coin
	SetTextColor(getMemDC(), RGB(250,250,150));
	sprintf(str, "%d", _player.coin);
	TextOut(getMemDC(), 70, 84, str, strlen(str));

	IMAGEMANAGER->findImage("coinIcon")->render(getMemDC(),25,80);
	////shootArea
	//sprintf(str, "shootSpeed : %0.2f", _player.shootSpeed);
	//TextOut(getMemDC(), 10, 200, str, strlen(str));
	//
	////speed
	//sprintf(str, "speed : %0.2f", _player.speed);
	//TextOut(getMemDC(), 10, 250, str, strlen(str));
	//
	////x,y
	//sprintf(str, "x: %0.2f", _player.x);
	//TextOut(getMemDC(), 10, 300, str, strlen(str));
	//sprintf(str, "y: %0.2f", _player.y);
	//TextOut(getMemDC(), 130, 300, str, strlen(str));
	//
	//sprintf(str, "toVellieDoor %d", toVellieDoor);
	//TextOut(getMemDC(), 10, 350, str, strlen(str));
	//sprintf(str, "startDoor %d", startDoor);
	//TextOut(getMemDC(), 10, 400, str, strlen(str));


	// ������ Ŀ�� ������ �׸���.
	if (geddy.showCurve)
	{
		for (int i = 0; i < CURVE_CIRCLE_LINE; i++)
		{
			IMAGEMANAGER->findImage("curvePoint")->render(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top);
			//Ellipse(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top, curveLine[i].rc.right, curveLine[i].rc.bottom);
		}
	}
	
	soundBtn->render();
	inventoryBtn->render();

	//sound____inven____________________________________________________________________
	soundRender();
	soundRender1();

	invenRender();

	if (inventoryOpen)
	{
		//coin
		SetTextColor(getMemDC(), RGB(50,50,50));
		sprintf(str, "%d", _player.coin);
		TextOut(getMemDC(), 540, 280, str, strlen(str));
	}

	DeleteObject(myFont);
	DeleteObject(hpFont);
}

void player::keyControl(void)
{
	// ������ ������, //////////////////////////////////////////////////////////////
	
	//////////////////
	if (!_player.fly)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			if (blueCollision())
			{
				_player.speed = 0;
			}
			_player.direction = LEFT;
			_player.state = RUN;
			_player.fall = false;
			_player.x -= _player.speed;
			miniX -= _player.speed;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT)) _player.state = IDLE;

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			if (greenCollision())
			{
				_player.speed = 0;
			}
			_player.direction = RIGHT;
			_player.state = RUN;
			_player.fall = false;
			_player.x += _player.speed;
			miniX += _player.speed;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT)) _player.state = IDLE;

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			if (yellowCollision())
			{
				_player.speed = 0;
			}
			_player.jump = true;
			_player.state = JUMP;
			_player.jumpCount++;
			if (_player.jumpCount <= 1)
			{
				_player.gravity = 30.0f;
			}
			else if (_player.jumpCount >= 2)
			{
				_player.fly = true;
				_player.state = FLYIDLE;
				_player.jump = false;
			}
			if (_player.fall == true)
			{
				_player.fly = true;
				_player.jump = false;
				_player.fall = false;
			}
		}
	} // !fly

	// ���� �⺻���� �϶� ////////////////////////////////////////////////////////////////////////////////
	if (_player.fly)
	{
		if (_player.state != FLYHOLD)
		{
			//_player.ground = true;
			if (KEYMANAGER->isStayKeyDown(VK_UP))
			{
				if (yellowCollision())
				{
					_player.speed = 0;
				}
				_player.y -= _player.speed;
				miniY -= _player.speed;
				_player.jump = false;
			}
			if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_player.y += _player.speed;
				miniY += _player.speed;
				_player.jump = false;
			}

			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				_player.direction = LEFT;	
				_player.fall = false;
				if (blueCollision())
				{
					_player.speed = 0;
				}
				_player.x -= _player.speed;
				miniX -= _player.speed;
				_player.state = FLYIDLE;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT)) _player.state = FLYIDLE;

			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				if (greenCollision())
				{
					_player.speed = 0;
				}
				_player.direction = RIGHT;
				_player.fall = false;
				_player.x += _player.speed;
				miniX += _player.speed;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT)) _player.state = FLYIDLE;

			if (!geddy.follow)
			{
				if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
				{
					_player.fall = true;
					_player.jump = true;
				}
			}

			if (KEYMANAGER->isStayKeyDown(VK_SPACE))
			{
				_player.state = TURN;
			}

			if (KEYMANAGER->isStayKeyDown('R'))
			{
				_player.state = ROLL;

				if (_player.direction == RIGHT)
				{
					if (greenCollision())
					{
						_player.speed = 0;
					}
					else
					{
						_player.x += 5;
						miniX += 5;
					}
				}
				if (_player.direction == LEFT)
				{
					if (blueCollision())
					{
						_player.speed = 0;
					}
					else
					{
						_player.x -= 5;
						miniX -= 5;
					}
				}
			}
		}

		// �� ��� ������ ////////////////////////////////////////////////////////////////////////////////
		else if (_player.state == FLYHOLD)
		{
			//_player.ground = true;
			if (KEYMANAGER->isStayKeyDown(VK_UP))
			{
				if (yellowCollision())
				{
					_player.speed = 0;
				}
				_player.y -= _player.speed;
				miniY -= _player.speed;
				_player.jump = false;
			}
			if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_player.y += _player.speed;
				miniY += _player.speed;
				_player.jump = false;
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				if (blueCollision())
				{
					_player.speed = 0;
				}
				_player.direction = LEFT;
				_player.fall = false;
				_player.x -= _player.speed;
				miniX -= _player.speed;
				_player.state = FLYHOLD;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				if (greenCollision())
				{
					_player.speed = 0;
				}
				_player.direction = RIGHT;
				_player.fall = false;
				_player.x += _player.speed;
				miniX += _player.speed;
			}
		}
	}

	//geddy___________________________________________________
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		geddy.follow = true;
	}
	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		geddy.follow = false;
	}

	if (geddy.follow)
	{
		if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
		{
			se1 = true;
			_player._fire->fire(geddy.x + cosf(_player.angle), geddy.y - sinf(_player.angle), _player.angle, 0, geddy.gunFrameX,_player.shootSpeed);
			_player.shootState = SHOOT;
			geddy.geddyState = gSHOOT;
		}
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			geddy.cast = true;
			geddy.showCurve = false;
			geddy.follow = false;
			geddy.castGravity = 0;
			_player.state = FLYIDLE;
		}
	}

}

// �Ʒ��� ���. �浹üũ 
void player::PixelCollision(void)
{
	if (!_player.jump)   // �������� �ʾ�����  
	{
		_player.ground = false; // ���ΰ� ��¥. 
	}
	if (_player.life)
	{
		// Pink_pixel_________________________________________________________________________________________________

		// i = x - ũ������; i< x + ũ���� ����; i++
		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)
		{
			// ���� �÷��̾� y��+ũ��/2 �Ʒ� �ȼ��� ������ ���̰� && ���� �ƴϸ�
			if (GetPixel(getPixel(), i, _player.y + 50 / 2 + 5 )== RGB(255, 0, 255) && !_player.ground)
			{
				_player.ground = true; // ���̶�� �˷���. 
				_player.y += _player.groundgrv; // �÷��̾�y += �� �׷���Ƽ 
				miniY += _player.groundgrv;

				_player.groundgrv = 0;
				_player.jumpCount = 0;
				if (_player.fly)
				{
					_player.fly = false;
					_player.state = IDLE;
				}
			//	if (_player.jump)
			//	{
			//		_player.jump = false;
			//		_player.state = IDLE;
			//	}
				if (_player.fall)
				{
					_player.fall = false;
					_player.state = IDLE;
				}
			}
		}
		//���� 
		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i) 
		{
			if (GetPixel(getPixel(), i, _player.y + 50 / 2 + 5) == RGB(255, 0, 255) && _player.gravity <= 0)
			{
				_player.y += _player.gravity;
				miniY += _player.gravity;

				_player.jump = false;
				_player.gravity = 20;

				break;
			}
		}

		//// BLUE_pixel____________________________________________________________________________________________________
		//for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)
		//{
		//	// ���� �÷��̾� y��+ũ��/2 �Ʒ� �ȼ��� ������ ���̰� && ���� �ƴϸ�
		//	if (GetPixel(getPixelBlue(), i, _player.y + 50 / 2 + 5) == RGB(0, 0, 255) && !_player.ground)
		//	{
		//		_player.ground = true; // ���̶�� �˷���. 
		//		_player.y += _player.groundgrv; // �÷��̾�y += �� �׷���Ƽ 

		//		_player.groundgrv = 0;
		//		_player.jumpCount = 0;
		//		if (_player.fly)
		//		{
		//			_player.fly = false;
		//			_player.state = IDLE;
		//		}
		//		//if (_player.jump)
		//		//{
		//		//	_player.jump = false;
		//		//	_player.state = IDLE;
		//		//}
		//		if (_player.fall)
		//		{
		//			_player.fall = false;
		//			_player.state = IDLE;
		//		}
		//	}
		//}

		//����. player����+5 �κ��� �Ķ����̸�,   
		//for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)//�÷��̾� ���� �翷 �˻�
		//{
		//	if (GetPixel(getPixelBlue(), i, _player.y + 50 / 2 + 5) == RGB(0, 0, 255) && _player.gravity <= 0)
		//	{
		//		_player.y += _player.gravity;
		//		_player.jump = false;
		//		_player.gravity = 20;
		//		break;
		//	}
		//}

		//if (blueCollision())
		//{
		//	//_player.x -= _player.speed;
		//}

		//for (int i = _player.x-25; i < _player.x + 25; ++i)
		//{
					//}
	}// if life
} // func

// ���� ��� �浹üũ 
bool player::blueCollision(void)
{
	//player.x ����+5 �� �Ķ����̸�, 
	if (GetPixel(getPixelBlue(), _player.x - 25, _player.y) == RGB(0, 0, 255))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ������ ��� �浹üũ 
bool player::greenCollision(void)
{
	if (GetPixel(getPixelGreen(), _player.x + 25, _player.y) == RGB(0, 255, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ���� ��� �浹üũ 
bool player::yellowCollision(void)
{
	if (GetPixel(getPixelYellow(), _player.x , _player.y - 25) == RGB(255,255,0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void player::jump(void)
{
	if (_player.jump)
	{
		_player.gravity -= 2.8f;
		_player.y -= _player.gravity;
		miniY -= _player.gravity;

		if (_player.gravity < -20) _player.gravity = -20;
	}
	//���������� �ʰ� �������µ� �ƴϸ�, ��������. 
	if (!_player.ground && !_player.jump && !_player.fly)
	{
		if (_player.state != RUN)
		{
			_player.fall = true;
		}
		_player.groundgrv -= 1.4f;
		_player.y -= _player.groundgrv;
		miniY -= _player.groundgrv;
		if (_player.groundgrv < -20) _player.groundgrv = -20;
	}
	if (_player.fall) _player.state = FALL;
}


void player::frameFunc(void)
{
	_player.frameCount++;
	switch (_player.state)
	{
		// player _______________________________________
	case IDLE:
		if (_player.frameCount > 12)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 19)
			{
				_player.currentX = 0;
			}
		}
		break;
	case RUN:
		if (_player.frameCount > 4)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 10)
			{
				_player.currentX = 0;
			}
		}
		break;
	case JUMP:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 2)
			{
				_player.currentX = 0;
			}
		}
		break;
	case TURN:
		if (_player.frameCount > 3)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 5)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FALL:
		if (_player.frameCount > 6)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 4)
			{
				_player.currentX = 0;
			}
		}
		break;
	case ROLL:
		if (_player.frameCount > 3)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 5)
			{
				_player.currentX = 0;
			}
		}
		break;

		// fly _______________________________________
	case FLYIDLE:
		if (_player.frameCount > 6)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 10)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYRUN:
		if (_player.frameCount > 6)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 8)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYTURN:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 3)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYFALL:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 5)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYROLL:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 5)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYHOLD:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 10)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYEAT:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX == 1)
			{
				se11 = true;
			}
			if (_player.currentX > 14)
			{
				_player.currentX = 0;
			}
		}
		break;
	case FLYDRINK:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 13)
			{
				_player.currentX = 0;
			}
		}
		break;

		// whole _______________________________________
	case HURT:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 11)
			{
				_player.currentX = 0;
			}
		}
		break;
	case DIE:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 8)
			{
				_player.currentX = 0;
			}
		}
		break;
	case SLEEP:
		if (_player.frameCount > 10)
		{
			_player.frameCount = 0;
			_player.currentX++;
			if (_player.currentX > 14)
			{
				_player.currentX = 0;
			}
		}
		break;
	default:
		break;
	}
}


void player::geddyFunc(void)
{
	if (geddy.follow)
	{
		_player.state = FLYHOLD;
		geddy.x = _player.x;
		geddy.y = _player.y + 70;
		geddy.ground = false;
	}
	// mouse angle
	_player.angle = getAngle(_player.x, _player.y, _ptMouse.x, _ptMouse.y);

	if (geddy.follow)
	{
		if (_player.angle >= PI * 2) _player.angle -= PI * 2;
		geddy.gunFrameX = int(_player.angle / PI_8); // �� �̹��� ������. 


		// �ѵ� �� ������Y 
		if (_player.shootState == SHOOT)
		{
			_player.shootFrameCount++;
			if (_player.shootFrameCount > 3)
			{
				_player.shootFrameCount = 0;
				_player.shootCurrentY++;
				if (_player.shootCurrentY > 5)
				{
					_player.shootCurrentY = 0;
					_player.shootState = NONESHOOT;
				}
			}
		}

		// ������ �߰�. 
		if (-PI / 8 < _player.angle && _player.angle <= PI / 8)
		{
			geddy.bodyFrameY = 3;
		}
		// ������ ��
		if (PI / 8 < _player.angle && _player.angle <= PI / 2)
		{
			geddy.bodyFrameY = 4;
		}
		// ������ �Ʒ�
		if (PI + PI / 2 < _player.angle && _player.angle <= PI + (PI / 2) + (PI / 4))
		{
			geddy.bodyFrameY = 5;
		}
		// ���� �߰�. 
		if (PI - PI / 8 < _player.angle && _player.angle <= PI + PI / 8)
		{
			geddy.bodyFrameY = 0;
		}
		// ���� ��. 
		if (PI / 2 < _player.angle && _player.angle <= PI - PI / 8)
		{
			geddy.bodyFrameY = 1;
		}
		// ���� �Ʒ�. 
		if (PI + PI / 8 < _player.angle && _player.angle <= PI + PI / 2)
		{
			geddy.bodyFrameY = 2;
		}
	}// follow 
}

void player::geddyPixelCollision(void)
{
	//���������� �ʰ� �������µ� �ƴϸ�, ��������. 
	if (!geddy.ground && !geddy.follow)
	{
		geddy.geddyState = gFALL;
		geddy.groundgrv -= 1.4f;
		geddy.y -= geddy.groundgrv;
		if (geddy.groundgrv < -20) geddy.groundgrv = -20;
	}

	////geddy__________________________________________________________________________________________________
	// i = x - ũ������; i< x + ũ���� ����; i++
	for (int i = geddy.x - geddy.radius; i < geddy.x + geddy.radius; ++i)
	{
		// ���� �÷��̾� y��+ũ��/2 �Ʒ� �ȼ��� ������ ���̰� && ���� �ƴϸ�
		if (GetPixel(getPixel(), i, geddy.y + geddy.radius ) == RGB(255, 0, 255) && !geddy.ground)
		{
			geddy.ground = true; // ���̶�� �˷���. 
			geddy.y += geddy.groundgrv; // �÷��̾�y += �� �׷���Ƽ 
			geddy.groundgrv = 0;
			geddy.geddyState = gIDLE;
		}
	}
	//���� 
	for (int i = geddy.x - geddy.radius; i < geddy.x + geddy.radius; ++i)
	{
		if (GetPixel(getPixel(), i, geddy.y + geddy.radius) == RGB(255, 0, 255) && geddy.gravity <= 0)
		{
			geddy.y += geddy.gravity;
			geddy.gravity = 20;
			break;
		}
	}
}

void player::geddyFrameFunc(void)
{
	switch (geddy.geddyState)
	{
	case gSHOOT:
		geddy.frameCount++;
		if (geddy.frameCount > 4)
		{
			geddy.frameCount = 0;
			geddy.bodyFrameX++;
			if (geddy.bodyFrameX > 5)
			{
				geddy.bodyFrameX = 0;
				geddy.geddyState = gNONE;
			}
		}
		break;
	case gIDLE:
		geddy.bodyFrameY = 6;
		geddy.bodyFrameX = 5;
		break;
	case gFALL:
		geddy.bodyFrameY = 7;
		geddy.frameCount++;
		if (geddy.frameCount > 4)
		{
			geddy.frameCount = 0;
			geddy.bodyFrameX++;
			if (geddy.bodyFrameX > 2)
			{
				geddy.bodyFrameX = 0;
			}
		}
		break;
	case gNONE:
		break;
	default:
		break;
	}
}

void player::geddyCastFunc()
{
	if (geddy.cast)
	{
		geddy.castGravity += 0.5f;
		geddy.gravity = 0;
		geddy.groundgrv = 0;
		geddy.x += cosf(geddy.angle) *  geddy.speed;
		geddy.y += -sinf(geddy.angle) * geddy.speed + geddy.castGravity;
	}
	if (geddy.ground)
	{
		geddy.cast = false;
		geddy.castGravity = 0;
	}
}

//���� ����� 
void player::CurveLineFunc()
{
	if (geddy.follow)
	{
		if (KEYMANAGER->isStayKeyDown(MK_RBUTTON))
		{
			geddy.showCurve = true;
			geddy.angle = getAngle(geddy.x, geddy.y, _ptMouse.x, _ptMouse.y);
			curveLine[0].x = geddy.x;
			curveLine[0].y = geddy.y;
			curveLine[0].rc = RectMakeCenter(curveLine[0].x, curveLine[0].y, CURVE_CIRCLE_SIZE/3, CURVE_CIRCLE_SIZE/3);
			curveLine[0].angle = geddy.angle;

			for (int i = 0, j = 1; j < CURVE_CIRCLE_LINE; i++, j++)
			{
           if (j == 1) geddy.castGravity = 0.05f;
			else if (j == 2) geddy.castGravity = 2.0f;
			else if (j == 3) geddy.castGravity = 4.0f;
			else if (j == 4) geddy.castGravity = 6.0f;
			else if (j == 5) geddy.castGravity = 8.0f;
			else if (j == 6) geddy.castGravity = 12.0f;
			else if (j == 7) geddy.castGravity = 17.0f;
			else if (j == 8) geddy.castGravity = 22.0f;
			else if (j == 9) geddy.castGravity = 30.0f;

				curveLine[j].angle = curveLine[i].angle;
				curveLine[j].x = cosf(curveLine[i].angle) * CURVE_CIRCLE_SIZE ;
				curveLine[j].y = -sinf(curveLine[i].angle) * CURVE_CIRCLE_SIZE + geddy.castGravity;

				curveLine[j].x = curveLine[j].x + curveLine[i].x;
				curveLine[j].y = curveLine[j].y + curveLine[i].y;

				curveLine[j].rc = RectMakeCenter(curveLine[j].x, curveLine[j].y, CURVE_CIRCLE_SIZE/3, CURVE_CIRCLE_SIZE/3);
			}
		}// Rbutton
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound _______________________________________________________________________________________________________
// ����� _______________________________________________________________________________________________________
void player::soundInit()
{
	// ����â ������?
	soundOpen = false;

	// ����� ______________________________________________________________________________________________����� 
	//����â ���.
	IMAGEMANAGER->addImage("soundOption","UI/soundOption1.bmp",WINSIZEX,WINSIZEY,true,RGB(255,0,255));

	//��ư 
	IMAGEMANAGER->addFrameImage("soundBtn", "UI/soundBtn.bmp", 133/1.4, 74 / 1.4, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("soundBar", "UI/soundBar.bmp",440,44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("barHandle", "UI/barHandle.bmp", 8,44, true, RGB(255, 0, 255));

	soundBtn = new button;
	soundBtn->init("soundBtn", 1210, 680, PointMake(0, 1),
		PointMake(0, 0), cbSoundBtn);

	// �����. 
	SOUNDMANAGER->addSound("0", "sound/startScene.flac", true, true);
	SOUNDMANAGER->addSound("1", "sound/otusHouseScene.flac", true, true);
	SOUNDMANAGER->addSound("2", "sound/vellieScene.flac", true, true);
	SOUNDMANAGER->addSound("3", "sound/Buccanary's Shop.flac", true, true);
	SOUNDMANAGER->addSound("4", "sound/eventScene.flac", true, true);
	SOUNDMANAGER->addSound("5", "sound/dunGeonScene.flac", true, true);
	SOUNDMANAGER->addSound("6", "sound/Bomboman.flac", true, true);
	SOUNDMANAGER->addSound("7", "sound/bossScene.flac", true, true);
	SOUNDMANAGER->addSound("8", "sound/08eventBridge.flac", true, true);
	SOUNDMANAGER->addSound("9", "sound/09_lab.flac", true, true);
	
	NowPlayList = "0";

	//testBGM

	// �̰� �ΰ��� �����ͺ��̽����ΰͰ���. 
	PlayList = 0;
	playNum = 0;
	volume = 0.5f;
	//if (!SOUNDMANAGER->isPlay(NowPlayList)) SOUNDMANAGER->play(NowPlayList, volume);

	px = 5;
	py = 410;
	
	MouseCheck = false;
	musicStart = false;
	musicStartFrame = 0;
}

void player::soundUpdate()
{
	// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	// ����� &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& �����
	if (!SOUNDMANAGER->isPlay(NowPlayList)) SOUNDMANAGER->play(NowPlayList);
	
	PlayList = playNum;

	if (playNum == 0)
	{
		//SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 1)
	{
		SOUNDMANAGER->stop("0");
		//SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 2)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		//SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 3)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		//SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 4)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		//SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 5)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		//SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 6)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		//SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 7)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		//SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}
	if (playNum == 8)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		//SOUNDMANAGER->stop("8");
		SOUNDMANAGER->stop("9");
	}	
	if (playNum == 9)
	{
		SOUNDMANAGER->stop("0");
		SOUNDMANAGER->stop("1");
		SOUNDMANAGER->stop("2");
		SOUNDMANAGER->stop("3");
		SOUNDMANAGER->stop("4");
		SOUNDMANAGER->stop("5");
		SOUNDMANAGER->stop("6");
		SOUNDMANAGER->stop("7");
		SOUNDMANAGER->stop("8");
		//SOUNDMANAGER->stop("9");
	}

	// ������ �޾Ƽ� ��Ʈ�������� �־��ش�. 
	switch (PlayList)
	{
	case 0:
		NowPlayList = "0";
		break;

	case 1:
		NowPlayList = "1";
		break;

	case 2:
		NowPlayList = "2";
		break;

	case 3:
		NowPlayList = "3";
		break;

	case 4:
		NowPlayList = "4";
		break;

	case 5:
		NowPlayList = "5";
		break;

	case 6:
		NowPlayList = "6";
		break;

	case 7:
		NowPlayList = "7";
		break;

	case 8:
		NowPlayList = "8";
		break;

	case 9:
		NowPlayList = "9";
		break;
	}

	if (soundOpen)
	{
		py = 300;
		b[0].rc = RectMake(px, py, 8, 44);

		barRC = RectMake(617, py, px, 44);
		barRC2 = RectMake(617, py, 440, 44);

		// ���α׷������� ��Ʈ�� ������, �׷��ϱ� ���� ��ư�� ������ ���� ���ν��� �Ǵ°���. 
		if (PtInRect(&barRC2, _ptMouse))
		{
			if (_leftButtonDown)
			{
				px = _ptMouse.x - 617;
			}
		}

		//�� ������ ���̸� -0.5~0.5�̰�, �̸� ���� �¿� �����Ѵ�. 
		if (KEYMANAGER->isOnceKeyUp(MK_LBUTTON))
		{
			MouseCheck = true;
			volume = (px / 440.0f) - 0.5f;
			SOUNDMANAGER->setVolume(NowPlayList, volume);

			MouseCheck = false;
		}
		/// sound btn&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& ��ư ���
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			DATABASE->getElementData("player")->soundOpen = false;
		}
	     	} //soundOpen

		soundOpen = DATABASE->getElementData("player")->soundOpen;
}

void player::soundRender()
{
	HBRUSH brushSound = CreateSolidBrush(RGB(120,0,180));
	HBRUSH brushSound1 = CreateSolidBrush(RGB(130,78,218));
	HBRUSH brushSound2 = CreateSolidBrush(RGB(180,150,218));
	HPEN penSound  = CreatePen(PS_SOLID, 1, RGB(120, 0, 180));
	HPEN penSound1 = CreatePen(PS_SOLID, 1, RGB(130, 78, 218));

	if (soundOpen)
	{
	   IMAGEMANAGER->findImage("soundOption")->render(getMemDC());
	}

	if (soundOpen)
	{
		// ����� �� ____________________________________________________________________________________________�����
			//Rectangle(getMemDC(), barRC2.left, barRC2.top, barRC2.right, barRC2.bottom);
			SelectObject(getMemDC(), brushSound);
			SelectObject(getMemDC(), penSound); 
			Rectangle(getMemDC(), barRC.left, barRC.top, barRC.right, barRC.bottom); // ����° 
			SelectObject(getMemDC(), brushSound1);
			Rectangle(getMemDC(), barRC.left, barRC.top+5, barRC.right, barRC.bottom-15); // �ι���
			SelectObject(getMemDC(), brushSound2);
			SelectObject(getMemDC(), penSound1);
			Rectangle(getMemDC(), barRC.left, barRC.top + 10, barRC.right, barRC.bottom - 25); // �帰��
			//Rectangle(getMemDC(), b[i].rc.left, b[i].rc.top, b[i].rc.right, b[i].rc.bottom);
			//IMAGEMANAGER->findImage("soundBar")->render(getMemDC(), barRC2.left, barRC2.top);
			IMAGEMANAGER->findImage("barHandle")->render(getMemDC(), barRC.right, barRC.top);
	}
	DeleteObject(brushSound);
	DeleteObject(brushSound1);
	DeleteObject(brushSound2);
	DeleteObject(penSound);

	//char str[128];
	//SetTextColor(getMemDC(), RGB(0,0,0));
	//HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY/��/��B"));
	//SelectObject(getMemDC(), myFont);
	//
	////sprintf(str, "%f", volume);
	////TextOut(getMemDC(), 500, 200, str, strlen(str));
	////sprintf(str, "%d", PlayList);
	////TextOut(getMemDC(), 500, 250, str, strlen(str));
	//
	//DeleteObject(myFont);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Sound _______________________________________________________________________________________________________
//// ȿ����_______________________________________________________________________________________________________
void player::soundInit1()
{
	se1 = false;
	se2 = false;
	se3 = false;
	se4 = false;
	se5 = false;; // �̺�Ʈ �긴��. 
	se6 = false;; // ����. 

	se7 = false; // ���. 
	se8 = false; // ������. 
	se9 = false; // �� ����.
	se10 = false; // �� ��. 

	se11 = false; // ���� �Ѿ�. 
	se12 = false; // ������ ������.
	
	_soundIndex = 0;

	char strSoundKey[128];
	char strSoundName[128];

	for (int i = 0; i < 12; i++)
	{
		ZeroMemory(strSoundKey, sizeof(strSoundKey));
		ZeroMemory(strSoundName, sizeof(strSoundName));

		sprintf(strSoundKey, "bgm/sound_%d", i);
		sprintf(strSoundName, "bgm/sound_%d.mp3", i); // �����̸��� sound_%d ���� �Ǿ� ����. 

		SOUNDMANAGER->addSound(strSoundKey, strSoundName, false, false); 
		_vTitle.push_back(strSoundName);
	}
}

void player::play(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "bgm/sound_%d", _soundIndex);
	SOUNDMANAGER->play(strSoundKey);
}

void player::stop(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "bgm/sound_%d", _soundIndex);
	SOUNDMANAGER->stop(strSoundKey);
	//this->setCurrentButton(this->getCurrentButton(BUTTONKIND_PLAY));
}

void player::pause(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "bgm/sound_%d", _soundIndex);
	if (!SOUNDMANAGER->isPause(strSoundKey))
	{
		SOUNDMANAGER->pause(strSoundKey);
		//this->setCurrentButton(this->getCurrentButton(BUTTONKIND_PLAY));
	}
}

void player::setSoundIndex(int index)
{
	if (index > 12) _soundIndex = 0;
	else _soundIndex = index;
}


void player::soundUpdate1()
{
	// �ѼҸ� 
	if (se1)
	{
		_soundIndex = 0;
		play();
		se1 = false;
	}

	// ����
	if (se2)
	{
		_soundIndex = 1;
		play();
		se2 = false;
	}

	// ������
	if (se3)
	{
		_soundIndex = 2;
		play();
		se3 = false;
	}

	// �� ��� 
	if (se4)
	{
		_soundIndex = 3;
		play();
		se4 = false;
	}

	// �̺�Ʈ �긴��. 
	if (se5)
	{
		_soundIndex = 4;
		play();
		se5 = false;
	}
	// ����. 
	if (se6)
	{
		_soundIndex = 5;
		play();
		se6 = false;
	}
	// // ���. 
	if (se7)
	{
		_soundIndex = 6;
		play();
		se7 = false;
	}
	// ������
	if (se8)
	{
		_soundIndex = 7;
		play();
		se8 = false;
	}
	// �� ����
	if (se9)
	{
		_soundIndex = 8;
		play();
		se9 = false;
	}
	// �� ��
	if (se10)
	{
		_soundIndex = 9;
		play();
		se10 = false;
	}
	// ���� �Ѿ�
	if (se11)
	{
		_soundIndex = 10;
		play();
		se11 = false;
	}
	// ��������.
	if (se12)
	{
		_soundIndex = 11;
		play();
		se12 = false;
	}

	//if (KEYMANAGER->isOnceKeyDown('5'))
	//{
	//	se1 = true;
	//}
	//if (_soundIndex > 15) _soundIndex = 0;
}

void player::soundRender1()
{
	
}


void player::cbSoundBtn()
{
	DATABASE->getElementData("player")->soundOpen = true;
}

// Inventory _______________________________________________________________________________________________________
void player::invenInit()
{
	inventoryOpen = false;

	IMAGEMANAGER->addFrameImage("inventoryBtn", "UI/inventoryBtn.bmp", 133 / 1.4, 74 / 1.4, 1, 2, true, RGB(255, 0, 255));
	inventoryBtn = new button;
	inventoryBtn->init("inventoryBtn", 1100, 680, PointMake(0, 1),
		PointMake(0, 0), cbInventoryBtn);

	// �κ��丮 â
	IMAGEMANAGER->addImage("inventoryOption", "UI/inventoryOption.bmp", 1280,720, true, RGB(255, 0, 255));
	//�����۵�
	IMAGEMANAGER->addImage("item1", "UI/item1_shoot.bmp",50,48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item2", "UI/item2_hp.bmp",   50,48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item3", "UI/item3_speed.bmp",50,48, true, RGB(255, 0, 255));

	//������ ���� ��
	IMAGEMANAGER->addImage("item1Text", "UI/inventoryItem1Text.bmp",WINSIZEX,WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item2Text", "UI/inventoryItem2Text.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item3Text", "UI/inventoryItem3Text.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	inven[0].wearRC = RectMake(365,375,50,50);
	inven[0].notWearRC = RectMake(450,375,50,50);
	inven[1].wearRC = RectMake(365, 455, 50, 50);
	inven[1].notWearRC = RectMake(450, 455, 50, 50);
	inven[2].wearRC = RectMake(365, 530, 50, 50);
	inven[2].notWearRC = RectMake(450, 530, 50, 50);
	
	for (int i = 0; i < 3; i++)
	{
		inven[i].notWear = false;
		inven[i].wear = false;
		inven[i].item = false;
	}

	itemTextCount = 0;
}

void player::invenUpdate()
{
	if (inventoryOpen)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			DATABASE->getElementData("player")->inventoryOpen = false;
		}
	}
	inventoryOpen = DATABASE->getElementData("player")->inventoryOpen;

	//��ݰŸ��� 200���� ũ�ٸ�, 
	if (_player.shootSpeed >5.0f)
	{
		//�κ��丮 ������κ��� ���� Ʈ��ȴ�. 
		inven[0].item = true;
		inven[0].wear = true;
	}

	if (_player.maxHP > 50)
	{
		inven[1].item = true;
		inven[1].wear = true;
	}
	if (_player.speed > 3.0)
	{
		inven[2].item = true;
		inven[2].wear = true;
	}

	//���� ������ ��Ʈ�� Ŭ���ϸ�, �� �����ɷ� �ǰ�, �÷��̾�� �ٽ� �ٲ��. 
	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&inven[i].wearRC, _ptMouse) && inven[i].item == true)
		{
			if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
			{
				if (i == 0)
				{
					inven[0].wear = false;
					inven[0].notWear = true;
					DATABASE->getElementData("player")->shootSpeed = 5.0f;
					itemTextCount = 0;
				}
				if (i == 1)
				{
					inven[1].wear = false;
					inven[1].notWear = true;
					DATABASE->getElementData("player")->maxHP = 50;
					itemTextCount = 1;
				}
				if (i == 2)
				{
					inven[2].wear = false;
					inven[2].notWear = true;
					DATABASE->getElementData("player")->speed = 3.0;
					itemTextCount = 2;
				}
			}
		}
	}

	//���� �������� ��Ʈ�� Ŭ���ϸ�, �����ɷ� �ǰ�, �÷��̾�� �ٽ� �ٲ��. 
	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&inven[i].notWearRC, _ptMouse) && inven[i].item == true)
		{
			if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
			{
				if (i == 0)
				{
					inven[0].notWear = false;
					inven[0].wear = true;
					DATABASE->getElementData("player")->shootSpeed = 8.0f;

				}
				if (i == 1)
				{
					inven[1].notWear = false;
					inven[1].wear = true;
					DATABASE->getElementData("player")->maxHP = 100;
				}
				if (i == 2)
				{
					inven[2].notWear = false;
					inven[2].wear = true;
					DATABASE->getElementData("player")->speed = 5.0f;
				}
			}
		}
	}
}

void player::invenRender()
{
	if (inventoryOpen)
	{
		IMAGEMANAGER->findImage("inventoryOption")->render(getMemDC());

		//for (int i = 0; i < 3; i++)
		//{
		//	Rectangle(getMemDC(), inven[i].wearRC.left, inven[i].wearRC.top, inven[i].wearRC.right, inven[i].wearRC.bottom);
		//	Rectangle(getMemDC(), inven[i].notWearRC.left, inven[i].notWearRC.top, inven[i].notWearRC.right, inven//[i].notWearRC.bottom);
		//}
				//���, ���������� Ʈ��, �������� �޽� �̸�, 
		if (inven[0].item && inven[0].notWear && !inven[0].wear)
		{
			IMAGEMANAGER->findImage("item1")->render(getMemDC(), inven[0].notWearRC.left, inven[0].notWearRC.top);
		}
		if (inven[1].item && inven[1].notWear && !inven[1].wear)
		{
			IMAGEMANAGER->findImage("item2")->render(getMemDC(), inven[1].notWearRC.left, inven[1].notWearRC.top);
		}
		if (inven[2].item && inven[2].notWear && !inven[2].wear)
		{
			IMAGEMANAGER->findImage("item3")->render(getMemDC(), inven[2].notWearRC.left, inven[2].notWearRC.top);
		}

		// ���� ��� 
		if (inven[0].item && inven[0].wear && !inven[0].notWear)
		{
			IMAGEMANAGER->findImage("item1")->render(getMemDC(), inven[0].wearRC.left, inven[0].wearRC.top);
		}
		if (inven[1].item && inven[1].wear && !inven[1].notWear)
		{
			IMAGEMANAGER->findImage("item2")->render(getMemDC(), inven[1].wearRC.left, inven[1].wearRC.top);
		}
		if (inven[2].item && inven[2].wear && !inven[2].notWear)
		{
			IMAGEMANAGER->findImage("item3")->render(getMemDC(), inven[2].wearRC.left, inven[2].wearRC.top);
		}

		// ����� 
		if (itemTextCount == 0 && inven[0].item)
		{
			IMAGEMANAGER->findImage("item1Text")->render(getMemDC(), 0, 0);
		}
		if (itemTextCount == 1 && inven[1].item)
		{
			IMAGEMANAGER->findImage("item2Text")->render(getMemDC(), 0, 0);
		}
		if (itemTextCount == 2 && inven[2].item)
		{
			IMAGEMANAGER->findImage("item3Text")->render(getMemDC(), 0, 0);
		}
	}//inventoryOpen
}

void player::cbInventoryBtn()
{
	DATABASE->getElementData("player")->inventoryOpen = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////effect _________________________________________________________________________________________________________effect
//struct tagPlayerEffect
//{
//	RECT rc;
//	float x, y, radius;
//	int frameCount, currentX;
//	bool life;
//	bool fire;
//};
//tagPlayerEffect pEffect[EFFECT_MAX];
//
//void player::effectInit(void)
//{
//	IMAGEMANAGER->addFrameImage("playerBullet2", "player/playerBullet2.bmp", 1800, 200, 9, 1, true, RGB(255, 0, 255));
//
//	for (int i = 0; i < EFFECT_MAX; i++)
//	{
//		pEffect[i].x = 0;
//		pEffect[i].y = 0;
//		pEffect[i].fire = false;
//		pEffect[i].life = false;
//		pEffect[i].frameCount = 0;
//		pEffect[i].currentX = 0;
//		pEffect[i].rc = { 100,100,10,10 };
//		pEffect[i].radius = 50;
//		pEffect[i].lifeFrame = 0;
//	}
//
//	/*otherHit=false;
//	otherHitCount=0;
//}*/
//}
//
//void player::effectUpdate(void)
//{
//	//frame
//	for (int i = 0; i < EFFECT_MAX; i++)
//	{
//		if (!pEffect[i].fire)continue;
//		else {
//			pEffect[i].lifeFrame++;
//			if (pEffect[i].lifeFrame <= 60)
//			{
//				pEffect[i].rc = RectMakeCenter(pEffect[i].x, pEffect[i].y, pEffect[i].radius, pEffect[i].radius);
//			}
//			if (pEffect[i].lifeFrame > 60)
//			{
//				pEffect[i].lifeFrame = 0;
//
//				pEffect[i].life = false;
//				break;
//			}
//		}
//	}
//
//	if (otherHit)
//	{
//		//otherHitCount++;
//			//if (otherHitCount % 2 == 0) // �ǰ� �ѹ��� ��Բ�. 
//			//{
//			//	PostQuitMessage(0);
//				//effectFire();
//				//otherHitCount = 0;
//			//}
//		
//		otherHit = false;
//	}
//}
//
//void player::effectRender(void)
//{
//	for (int i = 0; i < EFFECT_MAX; i++)
//	{
//		if (!pEffect[i].fire)continue;
//		IMAGEMANAGER->findImage("playerBullet2")->frameRender(getMemDC(), pEffect[i].rc.left, 
//		pEffect[i].rc.right, pEffect[i].currentX,0);
//		//Rectangle(getMemDC(),pEffect[i].rc.left, pEffect[i].rc.top, pEffect[i].rc.right, pEffect[i].rc.bottom);
//		//break;
//	}
//}
//
//void player::effectFire(void)
//{
//	// fire
//	for (int i = 0; i < EFFECT_MAX; i++)
//	{
//		if (pEffect[i].fire)continue;
//		pEffect[i].fire = true;
//		pEffect[i].rc = RectMakeCenter(pEffect[i].x, pEffect[i].y, pEffect[i].radius, pEffect[i].radius);
//		break;
//	}
//}

void player::removeMissile(int arrNum)
{
	//if (_player._fire)
	//{
	//	//frame
	//	if (pEffect[arrNum].fire)
	//	{
	//			pEffect[arrNum].rc = RectMakeCenter(_player._fire->getVBullet()[arrNum].x, 
	//				_player._fire->getVBullet()[arrNum].y, pEffect[arrNum].radius,
	//				pEffect[arrNum].radius);
	//			Rectangle(getMemDC(), pEffect[arrNum].rc.left, pEffect[arrNum].rc.top, 
	//				pEffect[arrNum].rc.right, pEffect[arrNum].rc.bottom);
	//			pEffect[arrNum].life = false;
	//		
	//	}
	//	
	//	
	//}
	_player._fire->removeMissile(arrNum);
}


void player::gunPixelCollision(void)
{
	for (int i = 0; i < _player._fire->getVBullet().size(); i++)
	{
		//player�Ѿ� ����+5 �� �Ķ����̸�, 
		if (GetPixel(getPixelBlue(), _player._fire->getVBullet()[i].x - 10, _player._fire->getVBullet()[i].y) == RGB(0, 0, 255))
		{
			removeMissile(i);
		}
		// �÷��̾� �Ѿ� �������� �ʷϻ��̸�,
		if (GetPixel(getPixelGreen(), _player._fire->getVBullet()[i].x + 10, _player._fire->getVBullet()[i].y) == RGB(0, 255,0))
		{
			removeMissile(i);
		}
		// �÷��̾� �Ѿ� ������ ������̸�,
		if (GetPixel(getPixelYellow(), _player._fire->getVBullet()[i].x, _player._fire->getVBullet()[i].y-10) == RGB(255,255, 0))
		{
			removeMissile(i);
		}
		// �÷��̾� �Ѿ� �Ʒ����� ��ũ���̸�,
		if (GetPixel(getPixel(), _player._fire->getVBullet()[i].x, _player._fire->getVBullet()[i].y - 10) == RGB(255, 0,255))
		{
			removeMissile(i);
		}
	}
}
