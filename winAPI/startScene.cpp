#include "stdafx.h"
#include "startScene.h"


startScene::startScene()
{
}


startScene::~startScene()
{
}

HRESULT startScene::init(void)
{
	gameNode::init();

	startRC = RectMakeCenter(WINSIZEX/2,WINSIZEY/2,250,50);

	soundRC.rc = RectMakeCenter(WINSIZEX/2, WINSIZEY/2+100, 250, 50);

	return S_OK;
}

void startScene::release(void)
{
	gameNode::release();
}

void startScene::update(void)
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		if (PtInRect(&startRC, _ptMouse))
		{
			SCENEMANAGER->changeScene("otusHouseScene");
		}
		if (PtInRect(&soundRC.rc, _ptMouse))
		{
			
		}
	}

}

void startScene::render(void)
{
	Rectangle(getMemDC(),startRC.left, startRC.top, startRC.right, startRC.bottom);
	Rectangle(getMemDC(), soundRC.rc.left, soundRC.rc.top, soundRC.rc.right, soundRC.rc.bottom);
}
