#include "stdafx.h"
#include "eventBridgeScene.h"


eventBridgeScene::eventBridgeScene()
{
}


eventBridgeScene::~eventBridgeScene()
{
}

HRESULT eventBridgeScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("BG", "Scene/eventBridge/eventBridgeBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("PixelPink", "Scene/eventBridge/eventBridgePixelPink.bmp", 1280, 720, false);

	behindBG = IMAGEMANAGER->addImage("BehindBG", "Scene/eventBridge/eventBridgeBehindBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("SkyBG","Scene/eventBridge/eventBridgeSkyBG.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("cloudLoopBG", "Scene/eventBridge/cloudLoopBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("monk", "Scene/eventBridge/monk.bmp", 2210*1.3, 130 * 1.3, 17, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("clock", "Scene/eventBridge/clock.bmp", 1700, 100, 17, 1, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(30, 450);

	monk_frameCount = 0;
	monk_CurrentX = 0;
	
	// 문
	rc = RectMake(30,450, 50, 90);

	//monkRC
	monkRC = RectMake(680,265,50,50);

	offsetX = 0;
	offsetcloudX = 0;

	return S_OK;
}

void eventBridgeScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void eventBridgeScene::update(void)
{
	gameNode::update();
	_player->update();

	offsetX += 0.2f;
	offsetcloudX -= 0.28f;

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->hp = _player->_player.hp;
			toVellieDoor = 5;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}

	monk_frameCount++;
	if (monk_frameCount > 18)
	{
		monk_frameCount = 0;
		monk_CurrentX++;
		if (monk_CurrentX > 16)
		{
			monk_CurrentX = 0;
		}
	}
}

void eventBridgeScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());

	IMAGEMANAGER->findImage("PixelPink")->render(getPixel()); // 분홍.

	RECT loopRC = RectMake(0,0,1280,720); //루프렌더 그릴 범위 렉트
	IMAGEMANAGER->findImage("SkyBG")->loopRender(getMemDC(),&loopRC, offsetX,0);// 스카이 루프

	IMAGEMANAGER->findImage("BehindBG")->render(getMemDC(), 0, 0); // 뒷배경.
	IMAGEMANAGER->findImage("cloudLoopBG")->loopRender(getMemDC(), &loopRC, offsetcloudX, 0);// 구름 루프

	IMAGEMANAGER->findImage("BG")->render(getMemDC(), 0, 0); // 앞배경. 
	
	IMAGEMANAGER->findImage("monk")->frameRender(getMemDC(),monkRC.left-70,monkRC.top-70, monk_CurrentX,0);
	//Rectangle(getMemDC(), monkRC.left, monkRC.top,monkRC.right,monkRC.bottom);

	IMAGEMANAGER->findImage("clock")->frameRender(getMemDC(),960,250, 0, 0);

	_player->render();

	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}


