#include "stdafx.h"
#include "otusHouseScene.h"


otusHouseScene::otusHouseScene()
{
}


otusHouseScene::~otusHouseScene()
{
}

HRESULT otusHouseScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addFrameImage("otusHouse","Scene/otusHouse.bmp",5120,720,4,1,true,RGB(255,0,255));

	backgroundPink = IMAGEMANAGER->addImage("otusPixelPink", "Scene/otusPixelPink.bmp", 1280, 720, false);
	backgroundBlue = IMAGEMANAGER->addImage("otusPixelBlue", "Scene/otusPixelBlue.bmp", 1280, 720, false);

	_player = new player;
	_player->init(400,200);

	frameCount = 0;
	CurrentX = 0;

	rc = RectMake(450,480,50,90);
	
	return S_OK;
}

void otusHouseScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void otusHouseScene::update(void)
{
	gameNode::update();
	_player->update();
	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
			toVellieDoor = 1;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}

	frameCount++;
	if (frameCount > 9)
	{
		frameCount = 0;
		CurrentX++;
		if (CurrentX > 3)
		{
			CurrentX = 0;
		}
	}


}

void otusHouseScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // µÚ¿¡ ÇÈ¼¿¹è°æµé ¾ÈµÇ°Ô²û.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // µÚ¿¡ ÇÈ¼¿¹è°æµé ¾ÈµÇ°Ô²û.

	// ÇöÀç ÇÈ¼¿ ¹è°æ.
	IMAGEMANAGER->findImage("otusPixelPink")->render(getPixel()); // ºÐÈ«.
	//IMAGEMANAGER->findImage("otusPixelBlue")->render(getPixelBlue()); // ÆÄ¶û.

	IMAGEMANAGER->findImage("otusHouse")->frameRender(getMemDC(), 0, 0, CurrentX, 0); // ¹è°æ. 

	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}

