#include "stdafx.h"
#include "eventBridgeScene.h"


eventBridgeScene::eventBridgeScene()
{
}


eventBridgeScene::~eventBridgeScene()
{
}

HRESULT eventBridgeScene::init(void)
{
	gameNode::init();

	_player = new player;
	_player->init();

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);
	return S_OK;
}

void eventBridgeScene::release(void)
{
	gameNode::release();
	SAFE_DELETE(_player);
}

void eventBridgeScene::update(void)
{
	gameNode::update();
	_player->update();

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		DATABASE->getElementData("player")->hp = _player->_player.hp;
		SCENEMANAGER->changeScene("VellieScene");
	}
}

void eventBridgeScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel());
	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

}

