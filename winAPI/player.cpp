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
	
	// database 에 해놓은걸 또 초기화하면 안되니까 주석처리해놨음. 
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

	// 각 행동별 불 변수. 
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

	// 총은 플레이어에서 쏘고, 게디 잡을때의 렉트가 필요해서 구조체를 생성하였음.
	// geddy __________________________________________________________________________________________________________
	geddy.x = 500;
	geddy.y = 200;
	geddy.radius = 25;
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, 50, 50);
	geddy.follow = false; // 플레이어를 따라다니냐?
	geddy.bodyFrameX = 0;  // 게디 몸 방향 프레임X.  
	geddy.bodyFrameY = 0; //y축
	geddy.gunFrameX = 0; // 총 16방향
	geddy.gunFrameY = 0; //y축
	geddy.gunX = 0;
	geddy.gunY = 0;
	geddy.gunRadius = 10;
	geddy.gunRC = RectMakeCenter(50,50,20,20); 
	geddy.angle = PI; // 포물선으로 던지기위한 앵글변수. 
	geddy.speed = 12; // 던지면 날라가는 속도.  속도를 높일수록 castGravity값도 높여야한다.
	geddy.cast = false; //던졌냐?
	geddy.castGravity = 0; // 점점 증가시킬 중력.
	geddy.geddyState = gNONE;
	geddy.frameCount = 0; //게디 몸 관련 프레임.  
	geddy.ground = false;
	geddy.gravity = 30.0f;
	geddy.groundgrv = 0;
	geddy.showCurve = false; // 커브 보여줄때 구분 불.

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

	// 마우스 애니메이션을 위한 기능.
	if(!geddy.follow) mouseGun = false;
	if (geddy.follow) mouseGun = true;
	if (!geddy.showCurve)GeddyThrow = false;
	if (geddy.showCurve)GeddyThrow = true;
	
	// 총 위치.
	geddy.gunX = geddy.x;
	geddy.gunY = geddy.y;
	// 플레이어, 게디 렉트, 총 렉트
	_player.rc = RectMakeCenter(_player.x, _player.y, _player.imgR->getFrameWidth() / 4, _player.imgR->getFrameHeight() / 4);
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, geddy.radius * 2, geddy.radius * 2);
	geddy.gunRC = RectMakeCenter(geddy.gunX, geddy.gunY, geddy.gunRadius * 2, geddy.gunRadius * 2);

	//player ___________________________________________________________________
	keyControl();
	jump();
	PixelCollision();
	frameFunc();

	//geddy _____________________________________________________________________
	geddyFunc(); //게디 팔든 프레임, 총쏨. 
	geddyPixelCollision();
	geddyFrameFunc(); // 게디 몸 프레임. 
	CurveLineFunc(); // 던질때 커브 라인 그리는 함수
	geddyCastFunc(); // 던지는 조건,키컨트롤.

	//sound_inven_________________________________________________________________
	soundUpdate();
	soundUpdate1();

	invenUpdate();

	//effectUpdate();

	if (_player.currentHP > _player.maxHP)
	{
		_player.currentHP = _player.maxHP;
	}
	
	// 플레이어에 거꾸로 넣어준다. 그러면 쓰고, 읽기 다 되는셈. 
	//플레이어를 상속받아서 동적할당한 모든 씬들에서
	_player.currentHP = DATABASE->getElementData("player")->currentHP;
	_player.maxHP = DATABASE->getElementData("player")->maxHP;
	_player.speed = DATABASE->getElementData("player")->speed;
	_player.shootSpeed = DATABASE->getElementData("player")->shootSpeed;
	_player.coin = DATABASE->getElementData("player")->coin;

	// hpBar 현재치, 최대치.
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


	// 던질때 커브 원형들 그리기.
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
	// 땅에서 걸을때, //////////////////////////////////////////////////////////////
	
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

	// 나는 기본상태 일때 ////////////////////////////////////////////////////////////////////////////////
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

		// 뭘 들고 있을때 ////////////////////////////////////////////////////////////////////////////////
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

// 아래쪽 담당. 충돌체크 
void player::PixelCollision(void)
{
	if (!_player.jump)   // 점프하지 않았으면  
	{
		_player.ground = false; // 땅인건 가짜. 
	}
	if (_player.life)
	{
		// Pink_pixel_________________________________________________________________________________________________

		// i = x - 크기절반; i< x + 크기의 절반; i++
		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)
		{
			// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
			if (GetPixel(getPixel(), i, _player.y + 50 / 2 + 5 )== RGB(255, 0, 255) && !_player.ground)
			{
				_player.ground = true; // 땅이라고 알려줌. 
				_player.y += _player.groundgrv; // 플레이어y += 땅 그래피티 
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
		//바텀 
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
		//	// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
		//	if (GetPixel(getPixelBlue(), i, _player.y + 50 / 2 + 5) == RGB(0, 0, 255) && !_player.ground)
		//	{
		//		_player.ground = true; // 땅이라고 알려줌. 
		//		_player.y += _player.groundgrv; // 플레이어y += 땅 그래피티 

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

		//바텀. player바텀+5 부분이 파랑색이면,   
		//for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)//플레이어 범위 양옆 검사
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

// 왼쪽 담당 충돌체크 
bool player::blueCollision(void)
{
	//player.x 왼쪽+5 가 파랑색이면, 
	if (GetPixel(getPixelBlue(), _player.x - 25, _player.y) == RGB(0, 0, 255))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 오른쪽 담당 충돌체크 
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

// 위쪽 담당 충돌체크 
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
	//땅에있지도 않고 점프상태도 아니면, 떨어진다. 
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
		geddy.gunFrameX = int(_player.angle / PI_8); // 총 이미지 프레임. 


		// 총든 팔 프레임Y 
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

		// 오른쪽 중간. 
		if (-PI / 8 < _player.angle && _player.angle <= PI / 8)
		{
			geddy.bodyFrameY = 3;
		}
		// 오른쪽 위
		if (PI / 8 < _player.angle && _player.angle <= PI / 2)
		{
			geddy.bodyFrameY = 4;
		}
		// 오른쪽 아래
		if (PI + PI / 2 < _player.angle && _player.angle <= PI + (PI / 2) + (PI / 4))
		{
			geddy.bodyFrameY = 5;
		}
		// 왼쪽 중간. 
		if (PI - PI / 8 < _player.angle && _player.angle <= PI + PI / 8)
		{
			geddy.bodyFrameY = 0;
		}
		// 왼쪽 위. 
		if (PI / 2 < _player.angle && _player.angle <= PI - PI / 8)
		{
			geddy.bodyFrameY = 1;
		}
		// 왼쪽 아래. 
		if (PI + PI / 8 < _player.angle && _player.angle <= PI + PI / 2)
		{
			geddy.bodyFrameY = 2;
		}
	}// follow 
}

void player::geddyPixelCollision(void)
{
	//땅에있지도 않고 점프상태도 아니면, 떨어진다. 
	if (!geddy.ground && !geddy.follow)
	{
		geddy.geddyState = gFALL;
		geddy.groundgrv -= 1.4f;
		geddy.y -= geddy.groundgrv;
		if (geddy.groundgrv < -20) geddy.groundgrv = -20;
	}

	////geddy__________________________________________________________________________________________________
	// i = x - 크기절반; i< x + 크기의 절반; i++
	for (int i = geddy.x - geddy.radius; i < geddy.x + geddy.radius; ++i)
	{
		// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
		if (GetPixel(getPixel(), i, geddy.y + geddy.radius ) == RGB(255, 0, 255) && !geddy.ground)
		{
			geddy.ground = true; // 땅이라고 알려줌. 
			geddy.y += geddy.groundgrv; // 플레이어y += 땅 그래피티 
			geddy.groundgrv = 0;
			geddy.geddyState = gIDLE;
		}
	}
	//바텀 
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

//궤적 만들기 
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
// 배경음 _______________________________________________________________________________________________________
void player::soundInit()
{
	// 사운드창 열었냐?
	soundOpen = false;

	// 배경음 ______________________________________________________________________________________________배경음 
	//사운드창 배경.
	IMAGEMANAGER->addImage("soundOption","UI/soundOption1.bmp",WINSIZEX,WINSIZEY,true,RGB(255,0,255));

	//버튼 
	IMAGEMANAGER->addFrameImage("soundBtn", "UI/soundBtn.bmp", 133/1.4, 74 / 1.4, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("soundBar", "UI/soundBar.bmp",440,44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("barHandle", "UI/barHandle.bmp", 8,44, true, RGB(255, 0, 255));

	soundBtn = new button;
	soundBtn->init("soundBtn", 1210, 680, PointMake(0, 1),
		PointMake(0, 0), cbSoundBtn);

	// 배경음. 
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

	// 이거 두개가 데이터베이스용인것같아. 
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
	// 배경음 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 배경음
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

	// 정수값 받아서 스트링값으로 넣어준다. 
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

		// 프로그래스바의 렉트를 누르면, 그러니까 볼륨 버튼을 누르면 이제 바인식이 되는거지. 
		if (PtInRect(&barRC2, _ptMouse))
		{
			if (_leftButtonDown)
			{
				px = _ptMouse.x - 617;
			}
		}

		//바 정해진 길이를 -0.5~0.5이고, 이를 볼륨 셋에 대입한다. 
		if (KEYMANAGER->isOnceKeyUp(MK_LBUTTON))
		{
			MouseCheck = true;
			volume = (px / 440.0f) - 0.5f;
			SOUNDMANAGER->setVolume(NowPlayList, volume);

			MouseCheck = false;
		}
		/// sound btn&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 버튼 디비
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
		// 배경음 바 ____________________________________________________________________________________________배경음
			//Rectangle(getMemDC(), barRC2.left, barRC2.top, barRC2.right, barRC2.bottom);
			SelectObject(getMemDC(), brushSound);
			SelectObject(getMemDC(), penSound); 
			Rectangle(getMemDC(), barRC.left, barRC.top, barRC.right, barRC.bottom); // 세번째 
			SelectObject(getMemDC(), brushSound1);
			Rectangle(getMemDC(), barRC.left, barRC.top+5, barRC.right, barRC.bottom-15); // 두번쨰
			SelectObject(getMemDC(), brushSound2);
			SelectObject(getMemDC(), penSound1);
			Rectangle(getMemDC(), barRC.left, barRC.top + 10, barRC.right, barRC.bottom - 25); // 흐린거
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
	//HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY/궁/서B"));
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
//// 효과음_______________________________________________________________________________________________________
void player::soundInit1()
{
	se1 = false;
	se2 = false;
	se3 = false;
	se4 = false;
	se5 = false;; // 이벤트 브릿지. 
	se6 = false;; // 대포. 

	se7 = false; // 블록. 
	se8 = false; // 문열림. 
	se9 = false; // 적 폭발.
	se10 = false; // 물 줌. 

	se11 = false; // 보스 총알. 
	se12 = false; // 해적선 터질때.
	
	_soundIndex = 0;

	char strSoundKey[128];
	char strSoundName[128];

	for (int i = 0; i < 12; i++)
	{
		ZeroMemory(strSoundKey, sizeof(strSoundKey));
		ZeroMemory(strSoundName, sizeof(strSoundName));

		sprintf(strSoundKey, "bgm/sound_%d", i);
		sprintf(strSoundName, "bgm/sound_%d.mp3", i); // 파일이름이 sound_%d 으로 되어 있음. 

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
	// 총소리 
	if (se1)
	{
		_soundIndex = 0;
		play();
		se1 = false;
	}

	// 날떄
	if (se2)
	{
		_soundIndex = 1;
		play();
		se2 = false;
	}

	// 먹을떄
	if (se3)
	{
		_soundIndex = 2;
		play();
		se3 = false;
	}

	// 링 통과 
	if (se4)
	{
		_soundIndex = 3;
		play();
		se4 = false;
	}

	// 이벤트 브릿지. 
	if (se5)
	{
		_soundIndex = 4;
		play();
		se5 = false;
	}
	// 대포. 
	if (se6)
	{
		_soundIndex = 5;
		play();
		se6 = false;
	}
	// // 블록. 
	if (se7)
	{
		_soundIndex = 6;
		play();
		se7 = false;
	}
	// 문열림
	if (se8)
	{
		_soundIndex = 7;
		play();
		se8 = false;
	}
	// 적 폭발
	if (se9)
	{
		_soundIndex = 8;
		play();
		se9 = false;
	}
	// 물 줌
	if (se10)
	{
		_soundIndex = 9;
		play();
		se10 = false;
	}
	// 보스 총알
	if (se11)
	{
		_soundIndex = 10;
		play();
		se11 = false;
	}
	// 보스터짐.
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

	// 인벤토리 창
	IMAGEMANAGER->addImage("inventoryOption", "UI/inventoryOption.bmp", 1280,720, true, RGB(255, 0, 255));
	//아이템들
	IMAGEMANAGER->addImage("item1", "UI/item1_shoot.bmp",50,48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item2", "UI/item2_hp.bmp",   50,48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("item3", "UI/item3_speed.bmp",50,48, true, RGB(255, 0, 255));

	//아이템 설명 글
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

	//사격거리가 200보다 크다면, 
	if (_player.shootSpeed >5.0f)
	{
		//인벤토리 미착용부분의 불이 트루된다. 
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

	//만약 입은쪽 렉트를 클릭하면, 안 입은걸로 되고, 플레이어값도 다시 바뀐다. 
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

	//만약 안입은쪽 렉트를 클릭하면, 입은걸로 되고, 플레이어값도 다시 바뀐다. 
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
				//샀고, 안입은불이 트루, 입은불이 펄스 이면, 
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

		// 입은 경우 
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

		// 설명글 
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
//			//if (otherHitCount % 2 == 0) // 피가 한번만 닳게끔. 
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
		//player총알 왼쪽+5 가 파랑색이면, 
		if (GetPixel(getPixelBlue(), _player._fire->getVBullet()[i].x - 10, _player._fire->getVBullet()[i].y) == RGB(0, 0, 255))
		{
			removeMissile(i);
		}
		// 플레이어 총알 오른쪽이 초록색이면,
		if (GetPixel(getPixelGreen(), _player._fire->getVBullet()[i].x + 10, _player._fire->getVBullet()[i].y) == RGB(0, 255,0))
		{
			removeMissile(i);
		}
		// 플레이어 총알 위쪽이 노랑색이면,
		if (GetPixel(getPixelYellow(), _player._fire->getVBullet()[i].x, _player._fire->getVBullet()[i].y-10) == RGB(255,255, 0))
		{
			removeMissile(i);
		}
		// 플레이어 총알 아래쪽이 핑크색이면,
		if (GetPixel(getPixel(), _player._fire->getVBullet()[i].x, _player._fire->getVBullet()[i].y - 10) == RGB(255, 0,255))
		{
			removeMissile(i);
		}
	}
}
