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

	//_background = IMAGEMANAGER->addImage("bg", "pixelTest/bg.bmp", 1280, 720, true);
	//_backgroundCheck = IMAGEMANAGER->addImage("pixelBG", "pixelTest/pixelBG.bmp", 1280, 720, false);

	//하늘 
	skyBG= IMAGEMANAGER->addImage("skyBG","vellie/velli.bmp",1280, 720,true,RGB(255,0,255));
	skyLoopFront= IMAGEMANAGER->addImage("skyLoopFront","vellie/cloudLoopFront.bmp",1280,720,true,RGB(255,0,255));
	skyLoopMiddle= IMAGEMANAGER->addImage("skyLoopMiddle","vellie/cloudLoopMiddle.bmp", 1280, 720,true,RGB(255,0,255));
	skyLoopBack= IMAGEMANAGER->addImage("skyLoopBack","vellie/cloudLoopBack.bmp", 1280, 720,true,RGB(255,0,255));

	_background = IMAGEMANAGER->addImage("bg","vellie/velli.bmp",3840,2880,true,RGB(255,0,255));
	_backgroundBehind = IMAGEMANAGER->addImage("villeBehindBG", "vellie/villeBehindBG.bmp", 3840, 2880, true, RGB(255, 0, 255)); 
	_backgroundCheck = IMAGEMANAGER->addImage("villePixelPink", "vellie/villePixelPink.bmp", 3840, 2880,false);

	IMAGEMANAGER->addImage("miniMap", "UI/miniMap.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("alphaOption", "UI/alpha_option.bmp", 1280, 720, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(0,0);

	cameraX = 0;
	cameraY = 0;

	// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
	door[0].x = 100;
	door[0].y = 100;
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

	// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
	DoorPos[0].x = 100;
	DoorPos[0].y = 100;
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

	return S_OK;
}

void VellieScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void VellieScene::update(void)
{
	gameNode::update();

	if (startDoor == 0)
	{
	    _player->_player.x = DoorPos[toVellieDoor].x;
	    _player->_player.y = DoorPos[toVellieDoor].y;
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
					SCENEMANAGER->changeScene("otusHouseScene");
				}
			}
		}
	}

	// 문 렉트
	for (int i = 0; i < 7; i++)
	{
		door[i].rc = RectMake(door[i].x, door[i].y, 40, 100);
	}

	velliCameraMove();
	_player->update();
}

void VellieScene::render(void)
{
	//skyBG->loopRender(getMemDC(),);

	_backgroundBehind->render(getMemDC(), cameraX, cameraY);
	_background->render(getMemDC(), cameraX, cameraY);
	_backgroundCheck->render(getPixel(), cameraX, cameraY);
	
	IMAGEMANAGER->findImage("miniMap")->render(getMemDC());

	_player->render();

	//문 렉트 
	for (int i = 0; i < 7; i++)
	{
		Rectangle(getMemDC(), door[i].rc.left, door[i].rc.top, door[i].rc.right, door[i].rc.bottom);
	}
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
		DoorPos[toVellieDoor].x += WINSIZEX / 2 - _player->_player.x;

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
