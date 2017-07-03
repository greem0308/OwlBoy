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

	//inventory____________________________________________________________________________________________
	invenInit();

	//player____________________________________________________________________________________________

	this->setHP(DATABASE->getElementData("player")->hp);
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
	IMAGEMANAGER->addImage("hpBar", "UI/hpBar.bmp", 140, 50, true, RGB(255, 0, 255));

	// player __________________________________________________________________________________________________________
	_player.x = x;
	_player.y = y;
	// database 에 해놓은걸 또 초기화하면 안되니까 주석처리해놨음. 
	//_player.hp = 0;
	//_player.coin = 0;
	_player.rc = RectMakeCenter(_player.x,_player.y, _player.imgR->getFrameWidth()/3, _player.imgR->getFrameHeight()/3);
	_player.angle = PI;
	_player.speed = 4;
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

	//ani->frameUpdate(TIMEMANAGER->getElapsedTime());

	// 총 위치.
	geddy.gunX = geddy.x;
	geddy.gunY = geddy.y;
	// 플레이어, 게디 렉트, 총 렉트
	_player.rc = RectMakeCenter(_player.x, _player.y, _player.imgR->getFrameWidth() / 4, _player.imgR->getFrameHeight() / 4);
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, geddy.radius * 2, geddy.radius * 2);
	geddy.gunRC = RectMakeCenter(geddy.gunX, geddy.gunY, geddy.gunRadius * 2, geddy.gunRadius * 2);

	//player ___________________________________________
	keyControl();
	jump();
	PixelCollision();
	frameFunc();

	//geddy ___________________________________________
	geddyFunc(); //게디 팔든 프레임, 총쏨. 
	geddyPixelCollision();
	geddyFrameFunc(); // 게디 몸 프레임. 
	CurveLineFunc(); // 던질때 커브 라인 그리는 함수
	geddyCastFunc(); // 던지는 조건,키컨트롤.


	//sound_inven_________________________________________________________________
	soundUpdate();
	invenUpdate();

}

void player::render(void)
{
	Rectangle(getMemDC(),_player.rc.left, _player.rc.top, _player.rc.right, _player.rc.bottom);
	Rectangle(getMemDC(), geddy.rc.left, geddy.rc.top, geddy.rc.right, geddy.rc.bottom);
	Rectangle(getMemDC(), geddy.gunRC.left, geddy.gunRC.top, geddy.gunRC.right, geddy.gunRC.bottom);

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

	char str[128];
	SetTextAlign(getMemDC(), TA_RIGHT);
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(20,10,0));
	HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("나눔바른고딕"));
	SelectObject(getMemDC(), myFont);

	//hp
	sprintf(str, "%d", _player.hp);
	TextOut(getMemDC(), 50, 150, str, strlen(str));

	//coin
	sprintf(str, "%d", _player.coin);
	TextOut(getMemDC(), 100, 150, str, strlen(str));

	//x,y
	sprintf(str, "%0.2f", _player.x);
	TextOut(getMemDC(), 100, 250, str, strlen(str));
	sprintf(str, "%0.2f", _player.y);
	TextOut(getMemDC(), 250, 250, str, strlen(str));

	// 던질때 커브 원형들 그리기.
	if (geddy.showCurve)
	{
		for (int i = 0; i < CURVE_CIRCLE_LINE; i++)
		{
			Ellipse(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top, curveLine[i].rc.right, curveLine[i].rc.bottom);
		}
	}
	_player._fire->render();

	//hp
	IMAGEMANAGER->findImage("hpBar")->render(getMemDC(),20,20);
	IMAGEMANAGER->findImage("hp")->render(getMemDC(),70,35);
	soundBtn->render();
	inventoryBtn->render();

	//sound____inven____________________________________________________________________
	soundRender();
	invenRender();
}

void player::keyControl(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_player.direction = LEFT;
		_player.state = RUN;
		_player.fall = false;
		_player.x -= _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT)) _player.state = IDLE;

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_player.direction = RIGHT;
		_player.state = RUN;	
		_player.fall = false;
		_player.x += _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT)) _player.state = IDLE;

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
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
			_player.jump = false;
		}
		if (_player.fall == true)
		{
			_player.fly = true;
			_player.jump = false;
			_player.fall = false;
		}
	}

	// 날고 있을때,
	if (_player.fly)
	{
		_player.state = FLYIDLE;
		_player.ground = true;
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			_player.y -= _player.speed;
			_player.jump = false;
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_player.y += _player.speed;
			_player.jump = false;
		}
		if (!geddy.follow)
		{
			if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON) || KEYMANAGER->isOnceKeyDown('E'))
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
			
			if (_player.direction == RIGHT) _player.x += 5;
			if (_player.direction == LEFT) _player.x -= 5;
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
}

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

				_player.groundgrv = 0;
				_player.jumpCount = 0;
				if (_player.fly)
				{
					_player.fly = false;
					_player.state = IDLE;
				}
				if (_player.jump)
				{
					_player.jump = false;
					_player.state = IDLE;
				}
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
				_player.jump = false;
				_player.gravity = 20;

				break;
			}
		}


		// BLUE_pixel____________________________________________________________________________________________________

		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)
		{
			// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
			if (GetPixel(getPixelBlue(), i, _player.y + 50 / 2 + 5) == RGB(0, 0, 255) && !_player.ground)
			{
				_player.ground = true; // 땅이라고 알려줌. 
				_player.y += _player.groundgrv; // 플레이어y += 땅 그래피티 

				_player.groundgrv = 0;
				_player.jumpCount = 0;
				if (_player.fly)
				{
					_player.fly = false;
					_player.state = IDLE;
				}
				if (_player.jump)
				{
					_player.jump = false;
					_player.state = IDLE;
				}
				if (_player.fall)
				{
					_player.fall = false;
					_player.state = IDLE;
				}
			}
		}

		//바텀. player바텀+5 부분이 파랑색이면,   
		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)//플레이어 범위 양옆 검사
		{
			if (GetPixel(getPixelBlue(), i, _player.y + 50 / 2 + 5) == RGB(0, 0, 255) && _player.gravity <= 0)
			{
				_player.y += _player.gravity;
				_player.jump = false;
				_player.gravity = 20;
				break;
			}
		}

		//for (int i = _player.x-25; i < _player.x + 25; ++i)
		//{
			//player.x 오른쪽+5 가 파랑색이면, 
			if (GetPixel(getPixelBlue(), _player.x + 25, _player.y) == RGB(0, 0, 255))
			{
			}
		//}
	}// if life
} // func


void player::jump(void)
{
	if (_player.jump)
	{
		_player.gravity -= 2.0f;
		_player.y -= _player.gravity;
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
			if (_player.currentX > 9)
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

void player::removeMissile(int arrNum)
{
	if (_player._fire)
	{
		_player._fire->removeMissile(arrNum);
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

		if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
		{
			_player._fire->fire(geddy.x + cosf(_player.angle) * 35, geddy.y - sinf(_player.angle) * 35, _player.angle, geddy.gunFrameX);
			_player.shootState = SHOOT;
			geddy.geddyState = gSHOOT;
		}


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
	if (geddy.follow)
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			geddy.cast = true;
			geddy.showCurve = false;
			geddy.follow = false;
			geddy.castGravity = 0;
		}
	}
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
void player::soundInit()
{
	// 사운드창 열었냐?
	//_soundOpen = false; 
	soundOpen = false;

	//사운드창 배경.
	IMAGEMANAGER->addImage("soundOption","UI/soundOption.bmp",WINSIZEX,WINSIZEY,true,RGB(255,0,255));

	//버튼 
	IMAGEMANAGER->addFrameImage("soundBtn", "UI/soundBtn.bmp", 133/1.4, 74 / 1.4, 1, 2, true, RGB(255, 0, 255));

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

	NowPlayList = "0";
}
void player::soundUpdate()
{
	if (soundOpen)
	{
		if (KEYMANAGER->isOnceKeyDown('T'))
		{
			if (!SOUNDMANAGER->isPlay("0"))
			{
				SOUNDMANAGER->play("0");
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			DATABASE->getElementData("player")->soundOpen = false;
		}
	}
	soundOpen = DATABASE->getElementData("player")->soundOpen;
}

void player::soundRender()
{
	if (soundOpen)
	{
		//_soundOpen = true;

		IMAGEMANAGER->findImage("soundOption")->render(getMemDC());
	}
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
	IMAGEMANAGER->addImage("inventoryOption", "UI/inventoryOption.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
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
}

void player::invenRender()
{
	if (inventoryOpen)
	{
		IMAGEMANAGER->findImage("inventoryOption")->render(getMemDC());
	}
}

void player::cbInventoryBtn()
{
	DATABASE->getElementData("player")->inventoryOpen = true;
}