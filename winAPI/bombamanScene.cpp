#include "stdafx.h"
#include "bombamanScene.h"


bombamanScene::bombamanScene()
{
}


bombamanScene::~bombamanScene()
{
}

HRESULT bombamanScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("bombamanBG", "Scene/bombamanBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("bombamanPixelPink", "Scene/bombamanPixelPink.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bombaman_blue", "Scene/bombaman_blue.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bombaman_green", "Scene/bombaman_green.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bombaman_yellow", "Scene/bombaman_yellow.bmp", 1280, 720, false);

	_player = new player;
	_player->init(600,479);
	_player->playNum = 6;
	_player->geddy.x = 620;
	_player->geddy.x = 479;

	frameCount = 0;
	CurrentX = 0;

	//붐버맨 집 문.
	rc = RectMake(594,434, 50, 90);

	return S_OK;
}

void bombamanScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void bombamanScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
			toVellieDoor = 4;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}
}

void bombamanScene::render(void)
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

	IMAGEMANAGER->findImage("bombamanPixelPink")->render(getPixel()); // 분홍.
	IMAGEMANAGER->findImage("bombaman_blue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("bombaman_green")->render(getPixelGreen());
	IMAGEMANAGER->findImage("bombaman_yellow")->render(getPixelYellow());

	IMAGEMANAGER->findImage("bombamanBG")->render(getMemDC(), 0, 0); // 배경. 

	_player->render();

	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}
