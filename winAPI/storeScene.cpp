#include "stdafx.h"
#include "storeScene.h"


storeScene::storeScene()
{
}


storeScene::~storeScene()
{
}

HRESULT storeScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("storeBG", "Scene/storeBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("storePixelPink", "Scene/storePixelPink.bmp", 1280, 720, false);
	
	_player = new player;
	_player->init(400, 200);

	frameCount = 0;
	CurrentX = 0;

	//상점 문
	rc = RectMake(285,345, 50, 90);

	return S_OK;
}

void storeScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void storeScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->hp = _player->_player.hp;
			toVellieDoor = 2;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}
}

void storeScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 뒤에 픽셀배경들 안되게끔.

	IMAGEMANAGER->findImage("storePixelPink")->render(getPixel()); // 분홍.

	IMAGEMANAGER->findImage("storeBG")->render(getMemDC(), 0, 0); // 배경. 

	_player->render();

	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}
