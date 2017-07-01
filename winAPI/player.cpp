#include "stdafx.h"
#include "player.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init(void)
{
	gameNode::init();

	this->setHP(DATABASE->getElementData("player")->hp);
	this->setCoin(DATABASE->getElementData("player")->coin);

	_player.imgR =IMAGEMANAGER->addFrameImage("playerR", "player/playerRight.bmp", (100 * 20)*2, (100 * 25)*2, 20, 25, true, RGB(255, 0, 255));
	_player.imgL = IMAGEMANAGER->addFrameImage("playerL", "player/playerLeft.bmp", (100 * 20) * 2, (100 * 25) * 2, 20, 25, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addImage("bullet","player/bullet.bmp",50,50,true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("armGun", "player/armGun.bmp", 50*16*3, 50*5 *3,16,6, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("geddyR","player/geddyRight.bmp",100 *6 * 1.5 ,100 * 3 * 1.5,6,3,true,RGB(255,0,255));
	//IMAGEMANAGER->addFrameImage("geddyL", "player/geddyLeft.bmp", 100 * 6 * 1.5, 100 * 3 * 1.5, 6, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("geddy", "player/geddy.bmp", 100 * 6 * 1.5, 100 * 6 * 1.5, 6, 6, true, RGB(255, 0, 255));

	// player __________________________________________________________________________________________________________
	_player.x = 300;
	_player.y = 100;
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
	geddy.x = 600;
	geddy.y = 100;
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

	geddy.ground = false;
	geddy.gravity = 30.0f;
	geddy.groundgrv = 0;

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
	//ani->frameUpdate(TIMEMANAGER->getElapsedTime());

	// 총 위치.
	geddy.gunX = geddy.x;
	geddy.gunY = geddy.y;
	// 플레이어, 게디 렉트, 총 렉트
	_player.rc = RectMakeCenter(_player.x, _player.y, _player.imgR->getFrameWidth() / 4, _player.imgR->getFrameHeight() / 3);
	geddy.rc = RectMakeCenter(geddy.x, geddy.y, geddy.radius*2, geddy.radius*2);
	geddy.gunRC = RectMakeCenter(geddy.x, geddy.y, geddy.gunRadius*2, geddy.gunRadius*2);

	keyControl();
	jump();
	PixelCollision();
	frameFunc();

	geddyFunc();
	geddyPixelCollision();
}

void player::render(void)
{
	Rectangle(getMemDC(),_player.rc.left, _player.rc.top, _player.rc.right, _player.rc.bottom);
	Rectangle(getMemDC(), geddy.rc.left, geddy.rc.top, geddy.rc.right, geddy.rc.bottom);
	Rectangle(getMemDC(), geddy.gunRC.left, geddy.gunRC.top, geddy.gunRC.right, geddy.gunRC.bottom);

	if (_player.direction == RIGHT)
	{
		IMAGEMANAGER->findImage("playerR")->frameRender(getMemDC(), _player.rc.left - 70, _player.rc.top - 70,
			_player.currentX, _player.state);
		//IMAGEMANAGER->findImage("geddyR")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top - 57, 0, 0);
		
	}
	if (_player.direction == LEFT)
	{
		IMAGEMANAGER->findImage("playerL")->frameRender(getMemDC(), _player.rc.left - 75, _player.rc.top - 70,
			_player.currentX, _player.state);
		//IMAGEMANAGER->findImage("geddyL")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top - 57, 0, 0);
	}
	
	IMAGEMANAGER->findImage("geddy")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top - 57, geddy.bodyFrameX, geddy.bodyFrameY);

	IMAGEMANAGER->findImage("armGun")->frameRender(getMemDC(), geddy.rc.left - 50, geddy.rc.top-40, geddy.gunFrameX, _player.shootCurrentY);

	char str[128];
	SetTextAlign(getMemDC(), TA_RIGHT);
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(20,10,0));
	HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("나눔바른고딕"));
	SelectObject(getMemDC(), myFont);

	sprintf(str, "%d", _player.hp);
	TextOut(getMemDC(), 50, 50, str, strlen(str));

	sprintf(str, "%d", _player.coin);
	TextOut(getMemDC(), 150, 50, str, strlen(str));

	_player._fire->render();
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
		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON) || KEYMANAGER->isOnceKeyDown('E'))
		{
			_player.fall = true;
			_player.jump = true;
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
		// i = x - 크기절반; i< x + 크기의 절반; i++
		for (int i = _player.x - 50 / 2; i < _player.x + 50 / 2; ++i)
		{
			// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
			if (GetPixel(getPixel(), i, _player.y + 50 / 2 + 5 )== RGB(255, 0, 255) && !_player.ground)
			{
				_player.ground = true; // 땅이라고 알려줌. 
				_player.y += _player.groundgrv; // 플레이어y += 땅 그래피티 

				// 플레이어y += 땅 그래피티 
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

	}// if life
} // func


void player::jump(void)
{
	if (_player.jump)
	{
		_player.gravity -= 1.4f;
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
		geddy.groundgrv -= 1.4f;
		geddy.y -= geddy.groundgrv;
		if (geddy.groundgrv < -20) geddy.groundgrv = -20;
	}

	////geddy__________________________________________________________________________________________________
	// i = x - 크기절반; i< x + 크기의 절반; i++
	for (int i = geddy.x - 50/2; i < geddy.x + 50/2; ++i)
	{
		// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
		if (GetPixel(getPixel(), i, geddy.y + 50/3 ) == RGB(255, 0, 255) && !geddy.ground)
		{
			geddy.ground = true; // 땅이라고 알려줌. 
			geddy.y += geddy.groundgrv; // 플레이어y += 땅 그래피티 
			geddy.groundgrv = 0;
		}
	}
	//바텀 
	for (int i = geddy.x - 50/2; i < geddy.x + 50 /2; ++i)
	{
		if (GetPixel(getPixel(), i, geddy.y + 50/3 ) == RGB(255, 0, 255) && geddy.gravity <= 0)
		{
			geddy.y += geddy.gravity;
			geddy.gravity = 20;
			break;
		}
	}
}