#include "stdafx.h"
#include "labScene.h"


labScene::labScene()
{
}


labScene::~labScene()
{
}

HRESULT labScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("labBG", "Scene/lab/labBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("labPixelPink", "Scene/lab/labPixelPink.bmp", 1280, 720, false);

	_player = new player;
	_player->init(400, 200);

	frameCount = 0;
	CurrentX = 0;

	// 연구실 문
	rc = RectMake(416, 311, 50, 90);

	return S_OK;
}

void labScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void labScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
			toVellieDoor = 3;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}
}

void labScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); 
	
	IMAGEMANAGER->findImage("labPixelPink")->render(getPixel()); // 분홍.

	IMAGEMANAGER->findImage("labBG")->render(getMemDC(), 0, 0); // 배경. 

	_player->render();

	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}
