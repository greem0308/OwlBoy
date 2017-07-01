#include "stdafx.h"
#include "dungeonScene.h"


dungeonScene::dungeonScene()
{
}


dungeonScene::~dungeonScene()
{
}

HRESULT dungeonScene::init(void)
{
	gameNode::init();

	_player = new player;
	_player->init();

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);
	return S_OK;
}

void dungeonScene::release(void)
{
	gameNode::release();
	SAFE_DELETE(_player);
}

void dungeonScene::update(void)
{
	gameNode::update();
	_player->update();

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		DATABASE->getElementData("player")->hp = _player->_player.hp;
		SCENEMANAGER->changeScene("VellieScene");
	}
}

void dungeonScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel());
	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

}
