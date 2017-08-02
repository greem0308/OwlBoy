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
	IMAGEMANAGER->addImage("labBG", "Scene/lab/labBG.bmp", 1280*1.2, 720 * 1.2, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("labPixelPink", "Scene/lab/labPixelPink.bmp", 1280 * 1.2, 720 * 1.2, false);
	IMAGEMANAGER->addImage("lab_blue", "Scene/lab/lab_blue.bmp", 1280 * 1.2, 720 * 1.2, false);
	IMAGEMANAGER->addImage("lab_green", "Scene/lab/lab_green.bmp", 1280 * 1.2, 720 * 1.2, false);
	IMAGEMANAGER->addImage("lab_yellow", "Scene/lab/lab_yellow.bmp", 1280 * 1.2, 720 * 1.2, false);

	IMAGEMANAGER->addFrameImage("lab_object2", "Scene/lab/lab_object2.bmp", 1416* 2,90 * 2, 24, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lab_object4", "Scene/lab/lab_object4.bmp", 280 * 2,38 * 2, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lab_object5", "Scene/lab/lab_object5.bmp", 480* 2,67 * 2, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lab_object6", "Scene/lab/lab_object6.bmp", 376 * 2, 41 * 2, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lab_object7", "Scene/lab/lab_object7.bmp", 3731 * 2, 65 * 2,41, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("Professor", "Scene/lab/Professor.bmp", 512*2,64*2, 8, 1, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(417,345);
	_player->playNum = 9;
	_player->geddy.x = 417;
	_player->geddy.y = 345;

	frameCount = 0;
	CurrentX = 0;

	// 연구실 문
	rc = RectMake(390, 310, 50, 100);

	frameCount2 = 0;
	CurrentX2= 0;
	frameCount4 = 0;
	CurrentX4 = 0;
	frameCount5 = 0;
	CurrentX5 = 0;
	frameCount6 = 0;
	CurrentX6 = 0;

	frameCount7 = 0;
	CurrentX7 = 0;

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

	frameFunc();

}

void labScene::render(void)
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

	IMAGEMANAGER->findImage("labPixelPink")->render(getPixel(), -120, -65); // 분홍.
	IMAGEMANAGER->findImage("lab_blue")->render(getPixelBlue(), -120, -65);
	IMAGEMANAGER->findImage("lab_green")->render(getPixelGreen(), -120, -65);
	IMAGEMANAGER->findImage("lab_yellow")->render(getPixelYellow(), -120, -65);

	IMAGEMANAGER->findImage("labBG")->render(getMemDC(), -120, -65); // 배경. 

	// 자잘한 것들
	IMAGEMANAGER->findImage("lab_object2")->frameRender(getMemDC(), 730, 235, CurrentX2, 0);
	IMAGEMANAGER->findImage("lab_object4")->frameRender(getMemDC(), 585, 315, CurrentX4, 0);
	IMAGEMANAGER->findImage("lab_object5")->frameRender(getMemDC(), 520, 430, CurrentX5, 0);
	IMAGEMANAGER->findImage("lab_object6")->frameRender(getMemDC(), 635, 523, CurrentX6, 0);
	IMAGEMANAGER->findImage("lab_object7")->frameRender(getMemDC(),550,190, CurrentX7,0);

	IMAGEMANAGER->findImage("Professor")->frameRender(getMemDC(), 320, 128, CurrentX5, 0);

	_player->render();

//	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}

void labScene::frameFunc(void)
{

	frameCount2++;
	if (frameCount2 > 10)
	{
		frameCount2 = 0;
		CurrentX2++;
		if (CurrentX2 > 23)
		{
			CurrentX2 = 0;
		}
	}


	frameCount4++;
	if (frameCount4 > 10)
	{
		frameCount4 = 0;
		CurrentX4++;
		if (CurrentX4 > 7)
		{
			CurrentX4 = 0;
		}
	}
	frameCount5++;
	if (frameCount5 > 10)
	{
		frameCount5 = 0;
		CurrentX5++;
		if (CurrentX5 > 7)
		{
			CurrentX5 = 0;
		}
	}

	frameCount6++;
	if (frameCount6 > 10)
	{
		frameCount6 = 0;
		CurrentX6++;
		if (CurrentX6 > 7)
		{
			CurrentX6 = 0;
		}
	}

	frameCount7++;
	if (frameCount7 > 16)
	{
		frameCount7 = 0;
		CurrentX7++;
		if (CurrentX7 > 40)
		{
			CurrentX7 = 0;
		}
	}
}