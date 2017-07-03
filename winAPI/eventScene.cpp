#include "stdafx.h"
#include "eventScene.h"


eventScene::eventScene()
{
}


eventScene::~eventScene()
{
}

HRESULT eventScene::init(void)
{
	gameNode::init();

	_player = new player;
	_player->init(100,100);

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);
	return S_OK;
}

void eventScene::release(void)
{
	gameNode::release();
	SAFE_DELETE(_player);
}

void eventScene::update(void)
{
	gameNode::update();
	_player->update();

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		DATABASE->getElementData("player")->hp = _player->_player.hp;
		SCENEMANAGER->changeScene("VellieScene");
	}
}

void eventScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel());
	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

}
