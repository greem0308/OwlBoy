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

	_player = new player;
	_player->init();

	rc = RectMake(WINSIZEX/2-300,WINSIZEY/2,50,50);
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
	//if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	//{
	//	if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
	//	{
	//		_player->_player.hp -= 4;
	//	}
	//}
	//if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	//{
	//	DATABASE->getElementData("player")->hp = _player->_player.hp;
	//	SCENEMANAGER->changeScene("VellieScene");
	//}
}

void otusHouseScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel());
	_player->render();
	Rectangle(getMemDC(),rc.left,rc.top,rc.right,rc.bottom);

}
