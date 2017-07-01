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

	_background = IMAGEMANAGER->addImage("bg", "pixelTest/bg.bmp", 1280, 720, true);
	_backgroundCheck = IMAGEMANAGER->addImage("pixelBG", "pixelTest/pixelBG.bmp", 1280, 720, false);

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);
	
	_player = new player;
	_player->init();

	return S_OK;
}

void VellieScene::release(void)
{
	gameNode::release();
	SAFE_DELETE(_player);
}

void VellieScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
		{

			_player->_player.hp += 2;
		}
	}
	//if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	//{
	//	DATABASE->getElementData("player")->hp = _player->_player.hp;
	//	SCENEMANAGER->changeScene("otusHouseScene");
	//}
}

void VellieScene::render(void)
{
	_background->render(getMemDC(), 0, 0);
	_backgroundCheck->render(getPixel(), 0, 0);
	
	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

}
