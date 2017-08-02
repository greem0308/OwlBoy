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
	IMAGEMANAGER->addImage("otusPixelBlue", "Scene/otusPixelBlue.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("otusPixelGreen", "Scene/otusPixelGreen.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("otusPixelYellow", "Scene/otusPixelYellow.bmp", 1280, 720, false);
	
	_player = new player;
	_player->init(797,243);
	_player->playNum = 1;
	_player->geddy.x = 3000;
	_player->geddy.y = 3000;

	frameCount = 0;
	CurrentX = 0;

	rc = RectMake(450,480,50,90);

	_efm = new effectManager;
	_efm->init();
//	_efm->setPlayer(_player);
	
	effectShowFrame = 0;

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
	_efm->update();

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

	// 쏜 총알의 사격거리를 얻어야함... 마지막. 좌표. 
	for (int i = 0; i < _player->_player._fire->getVBullet().size(); i++)
	{

	}

	Create();
	Delete();
}

void otusHouseScene::render(void)
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

    // 현재 픽셀 배경.
	IMAGEMANAGER->findImage("otusPixelPink")->render(getPixel()); // 분홍.
    IMAGEMANAGER->findImage("otusPixelBlue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("otusPixelGreen")->render(getPixelGreen());
	IMAGEMANAGER->findImage("otusPixelYellow")->render(getPixelYellow());

	IMAGEMANAGER->findImage("otusHouse")->frameRender(getMemDC(), 0, 0, CurrentX, 0); // 배경. 

	_player->render();
	_efm->render();

	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}

void otusHouseScene::Create()
{
	effectShowFrame++;
	if (effectShowFrame == 1)
	{
		//_efm->setPlayerEffect(500,700);
	}
}

// 여긴되는데 
void otusHouseScene::Delete(void)
{
	for (int i = 0; i < _efm->getMinion().size(); ++i)
	{
		if (!_efm->getMinion()[i]->fx.life) _efm->removeMinion(i);
	}
}
