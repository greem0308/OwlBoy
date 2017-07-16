#include "stdafx.h"
#include "VellieScene.h"


VellieScene::VellieScene()
{
}


VellieScene::~VellieScene()
{
}

HRESULT VellieScene::init(void)
{
	gameNode::init();
	
	//하늘 
	// 점프할때 뒤에 흰배경 안보이게
	IMAGEMANAGER->addImage("backSkyBG", "vellie/skyBG.bmp", 3840, 2880, true, RGB(255, 0, 255));

	skyBG = IMAGEMANAGER->addImage("skyBG", "vellie/skyBG.bmp", 1280, 720 * 2, true, RGB(255, 0, 255));
	skyLoopFront = IMAGEMANAGER->addImage("skyLoopFront", "vellie/cloudLoopFront.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopMiddle = IMAGEMANAGER->addImage("skyLoopMiddle", "vellie/cloudLoopMiddle.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopBack = IMAGEMANAGER->addImage("skyLoopBack", "vellie/cloudLoopBack.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));

	// vellie 배경.
	_background = IMAGEMANAGER->addImage("bg","vellie/velli.bmp",3840,2880,true,RGB(255,0,255));
	_backgroundBehind = IMAGEMANAGER->addImage("villeBehindBG", "vellie/villeBehindBG.bmp", 3840, 2880, true, RGB(255, 0, 255)); 
	_backgroundCheck = IMAGEMANAGER->addImage("villePixelPink", "vellie/villePixelPink.bmp", 3840, 2880,false);

	IMAGEMANAGER->addImage("miniMap", "UI/miniMap.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("alphaOption", "UI/alpha_option.bmp", 1280, 720, true, RGB(255, 0, 255));

	// 마을 구름1,2 
	IMAGEMANAGER->addImage("cloud1", "vellie/cloud1.bmp",3840, 2880, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("cloud2", "vellie/cloud2.bmp",3840, 2880, true, RGB(255, 0, 255));

	//mimimap vellie
	IMAGEMANAGER->addImage("minimapBG", "UI/minimapBG.bmp", 3840 / 20, 2880 / 20, true, RGB(255, 0, 255));

	cameraX = 0;
	cameraY = 0;
	
	skyLoopFront_offsetX=0;
	skyLoopMiddle_offsetX=0;
	skyLoopBack_offsetX=0;

	cloud1_offsetX = 0;
	cloud2_offsetX = 0;
	cloudX=0;
	cloudY=0;

	doorPosInit();

	mapRC = RectMake(1060, 500, 3840/20,2880/20);

	itemShowFrame = 0;

	_player = new player;
	_player->init(2200, 1645);
	_player->miniX = 2200;
	_player->miniY = 1645;

	_im = new itemManager;
	_im->init();
	_im->setPlayer(_player);

	minimapFrameCout = 0;
	minimapCurrentX = 0;

	return S_OK;
}

void VellieScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
	//SAFE_DELETE(_im);
}

void VellieScene::update(void)
{
	gameNode::update();
	_player->update();
	_im->update();

	doorPosFunc();

	velliCameraMove();

	skyLoopBack_offsetX -= 0.3f;
	skyLoopMiddle_offsetX += 0.2f;
	skyLoopFront_offsetX -= 0.2f;

	cloud1_offsetX -= 0.25f;
	cloud2_offsetX += 0.32f;

	Create(1);
	Delete();

	minimapUpdate();

	// minimap 카운트
	minimapFrameCout++;
	if (minimapFrameCout > 6)
	{
		minimapFrameCout = 0;
		minimapCurrentX++;
		if (minimapCurrentX > 8)
		{
			minimapCurrentX = 0;
		}
	}
}

void VellieScene::render(void)
{
	RECT cloudRC = RectMake(cloudX, cloudY, 3840, 2880);
	RECT loopRC = RectMake(_player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2 - 50, WINSIZEX, WINSIZEY + 80);

	IMAGEMANAGER->findImage("backSkyBG")->render(getMemDC());
	skyBG->render(getMemDC(), _player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2);
	skyLoopBack->loopRender(getMemDC(), &loopRC, skyLoopBack_offsetX, 0);
	skyLoopMiddle->loopRender(getMemDC(), &loopRC, skyLoopMiddle_offsetX, 0);
	skyLoopFront->loopRender(getMemDC(), &loopRC, skyLoopFront_offsetX, 0);
	
	IMAGEMANAGER->findImage("cloud2")->loopRender(getMemDC(), &cloudRC, cloud2_offsetX, 0);
	_backgroundBehind->render(getMemDC(), cameraX, cameraY);
	IMAGEMANAGER->findImage("cloud1")->loopRender(getMemDC(), &cloudRC, cloud1_offsetX, 0);

	_background->render(getMemDC(), cameraX, cameraY);
	_backgroundCheck->render(getPixel(), cameraX, cameraY);
	
	_player->render();

	//문 렉트 
	for (int i = 0; i < 7; i++)
	{
		Rectangle(getMemDC(), door[i].rc.left, door[i].rc.top, door[i].rc.right, door[i].rc.bottom);
	}

	// minimap Render
	IMAGEMANAGER->findImage("miniMap")->render(getMemDC(),0,0);
	IMAGEMANAGER->findImage("minimapBG")->render(getMemDC(), 1060,500);

	if (goDungeon) // 만약 이벤트씬에서 마을로 다시 왔다면 던전위치가 표시됨. 
	{
		IMAGEMANAGER->findImage("minimapPoint")->frameRender(getMemDC(), 1055, 600, minimapCurrentX, 0);
	}

	HBRUSH miniBrush = CreateSolidBrush(RGB(150,190,250));
	HPEN miniPen = CreatePen(PS_SOLID, 2, RGB(255,255, 255));
	SelectObject(getMemDC(), miniPen);
	SelectObject(getMemDC(), miniBrush);
	// mini player
	Ellipse(getMemDC(),miniRC.left,miniRC.top,miniRC.right,miniRC.bottom);
	DeleteObject(miniPen);
	DeleteObject(miniBrush);

	_im->render();
}

//마을 카메라. //모든 클래스를 상속받는 이곳에서 카메라를 설치하자.
//마을 배경 크기 3840,2880
void VellieScene::velliCameraMove()
{
	//만약 플레이어가 오른쪽에 있고, 배경x축이 - (배경이미지-화면사이즈)보다 크다면
	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840-WINSIZEX)) // 오른쪽
	{
		cameraX -= _player->_player.x - WINSIZEX / 2;
		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;
		
		// 문 
		for (int i = 0; i < 7; i++)
		{
			door[i].x -= _player->_player.x - WINSIZEX / 2;
		}
		// 이위치는 지정된 extern위치를 플레이어에 넣는데 카메라가 적용되야지 정상이여서 넣음. 
		DoorPos[toVellieDoor].x -= _player->_player.x - WINSIZEX / 2;

		cloudX -= _player->_player.x - WINSIZEX / 2;

  //      // item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.x -= _player->_player.x - WINSIZEX / 2;
		}

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
	{
		cameraX += WINSIZEX / 2 - _player->_player.x;
		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;

		// 문
		for (int i = 0; i < 7; i++)
		{
			door[i].x += WINSIZEX / 2 - _player->_player.x;
		}

		cloudX += WINSIZEX / 2 - _player->_player.x;

		DoorPos[toVellieDoor].x += WINSIZEX / 2 - _player->_player.x;

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.x += WINSIZEX / 2 - _player->_player.x;
		}

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880-WINSIZEY)) // 오른쪽
	{
		cameraY -= _player->_player.y - WINSIZEY / 2;
		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;

		//문 
		for (int i = 0; i < 7; i++)
		{
			door[i].y -= _player->_player.y - WINSIZEY / 2;
		}
		DoorPos[toVellieDoor].y -= _player->_player.y - WINSIZEY / 2;

		cloudY -= _player->_player.y - WINSIZEY / 2;

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.y -= _player->_player.y - WINSIZEY / 2;
		}

		_player->_player.y = WINSIZEY / 2;
	}

	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
	{
		cameraY += WINSIZEY / 2 - _player->_player.y;
		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;

		//문
		for (int i = 0; i < 7; i++)
		{
			door[i].y += WINSIZEY / 2 - _player->_player.y;
		}

		cloudY += WINSIZEY / 2 - _player->_player.y;

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.y += WINSIZEY / 2 - _player->_player.y;
		}

		DoorPos[toVellieDoor].y += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// 이미 위에서 카메라 화면제한이 되어있어서 플레이어x,y는 항상 WINSIZEX/2,WINSIZEY/2로 되어있고
	// 화면제한에 다까이오게되면 중점을 벗어나므로 밑에와같이 플레이어 화면제한을 걸어줄 수 있다.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX-50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}

void VellieScene::doorPosFunc()
{
	if (startDoor == 0)
	{
		_player->_player.x = DoorPos[toVellieDoor].x;
		_player->_player.y = DoorPos[toVellieDoor].y;
		_player->miniX = DoorPos[toVellieDoor].x;
		_player->miniY = DoorPos[toVellieDoor].y;
		startDoor = 1;
	}

	RECT rcTemp;
	for (int i = 0; i < 7; i++)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &door[i].rc))
		{
			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				if (i == 1) // 1이면 오투스집 렉트.
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("otusHouseScene");
				}
				if (i == 2) // 상점
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("storeScene");
				}
				if (i == 3) // 랩
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("labScene");
				}
				if (i == 4) // 붐버맨
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("bombamanScene");
				}
				if (i == 5) // 이벤트 브릿지
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("eventBridgeScene");
				}
				if (i == 6) // 던전
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("dungeonScene");
				}
			}
		}
	}

	// 문 렉트
	for (int i = 0; i < 7; i++)
	{
		door[i].rc = RectMake(door[i].x, door[i].y, 40, 100);
	}
}

void VellieScene::doorPosInit()
{
	// 문 위치. 
	// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
	door[0].x = -300;
	door[0].y = -300;
	door[1].x = 2200;
	door[1].y = 1645;
	door[2].x = 813;
	door[2].y = 640;
	door[3].x = 2746;
	door[3].y = 407;
	door[4].x = 1693;
	door[4].y = 903;
	door[5].x = 3037;
	door[5].y = 2222;
	door[6].x = 341;
	door[6].y = 2612;

	// 다른씬에서 마을로 넘어올때 받는 선택하는 좌표값.
	// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
	DoorPos[0].x = 2200;
	DoorPos[0].y = 1645;
	DoorPos[1].x = 2200;
	DoorPos[1].y = 1645;
	DoorPos[2].x = 813;
	DoorPos[2].y = 640;
	DoorPos[3].x = 2746;
	DoorPos[3].y = 407;
	DoorPos[4].x = 1693;
	DoorPos[4].y = 903;
	DoorPos[5].x = 3037;
	DoorPos[5].y = 2222;
	DoorPos[6].x = 341;
	DoorPos[6].y = 2612;
}

void VellieScene::minimapUpdate()
{
	// 20으로 나눠서 이동구간을 줄인다. 플레이어x,y는 이미 카메라에 잡혀있어서
	// 플레이어 클래스에서 따로 좌표를 만들어서 카메라에 잡히지않으면서 플레이어랑 똑같이 움직이게 하였다.
	miniRC = RectMakeCenter(_player->miniX / 20 + 1060, _player->miniY / 20 + 500, 10, 10);

	// 미니점 왼쪽이 화면 왼쪽보다 작아진다면, 
	if (miniRC.left < mapRC.left)
	{
		miniRC.left = mapRC.left;
		miniRC.right = miniRC.left + 10;
	}
	if (miniRC.right > mapRC.right)
	{
		miniRC.right = mapRC.right;
		miniRC.left = miniRC.right - 10;
	}
	if (miniRC.top < mapRC.top)
	{
		miniRC.top = mapRC.top;
		miniRC.bottom = miniRC.top + 10;
	}
	if (miniRC.bottom > mapRC.bottom)
	{
		miniRC.bottom = mapRC.bottom;
		miniRC.top = miniRC.bottom - 10;
	}
}

void VellieScene::Create(int Num)
{
	itemShowFrame++;
	if (itemShowFrame == 1)
	{
		_im->setFruit0(100,100);
		_im->setFruit0(20, 100);
		_im->setFruit0(50, 100);
		_im->setFruit0(300, 10);
		_im->setFruit0(10, 100);
		_im->setFruit0(200, 500);
		_im->setFruit0(1000, 2000);
		_im->setFruit0(3000, 2000);
		_im->setFruit0(30, 200);
		_im->setFruit0(50, 300);
		_im->setFruit0(300, 100);
		_im->setFruit0(400, 100);
		_im->setFruit0(440, 150);

	}
}


// 여긴되는데 
void VellieScene::Delete(void)
{
	for (int i = 0; i < _im->getItem().size(); ++i)
	{
		//if (_im->getItem()[i]->tem.x <300) _im->removeItem(i);
	}
}