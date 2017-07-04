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

	_player = new player;
	_player->init(400, 200);

	frameCount = 0;
	CurrentX = 0;

	//�չ��� �� ��.
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
		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->hp = _player->_player.hp;
			toVellieDoor = 4;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}
}

void bombamanScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // �ڿ� �ȼ����� �ȵǰԲ�.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // �ڿ� �ȼ����� �ȵǰԲ�.

	IMAGEMANAGER->findImage("bombamanPixelPink")->render(getPixel()); // ��ȫ.

	IMAGEMANAGER->findImage("bombamanBG")->render(getMemDC(), 0, 0); // ���. 

	_player->render();

	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}
