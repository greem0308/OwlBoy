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
	SAFE_DELETE(_player);
}

void otusHouseScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
		{
			_player->_player.hp -= 4;
		}
	}
	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		DATABASE->getElementData("player")->hp = _player->_player.hp;
		SCENEMANAGER->changeScene("VellieScene");
	}
}

void otusHouseScene::render(void)
{
	_player->render();
	Rectangle(getMemDC(),rc.left,rc.top,rc.right,rc.bottom);

	char str[128];
	SetTextAlign(getMemDC(), TA_RIGHT);
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(251, 225, 108));
	HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY±Ã¼­B"));
	SelectObject(getMemDC(), myFont);

	sprintf(str, "%d", _player->_player.hp);
	TextOut(getMemDC(), 670, 270, str, strlen(str));
}
