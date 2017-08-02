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

	IMAGEMANAGER->addFrameImage("startScene", "Scene/startScene.bmp",19200,720,15,1,true,RGB(255,0,255));
	
	IMAGEMANAGER->addFrameImage("startBtn", "UI/startBtn.bmp", 133*1.2,74*1.2, 1, 2,true,RGB(255,0,255));
	IMAGEMANAGER->addImage("item1_shoot", "UI/item1_shoot.bmp", 42, 40, true, RGB(255, 0, 255));

	frameCount = 0;
	currentX = 0;
	startDirec = RIGHT;

	startBtn = new button;
	startBtn->init("startBtn", 150 + 500, 380, PointMake(0,1),
		PointMake(0, 0), cbStartBtn);

	_player = new player;
	_player->init(2200, 1645);
	_player->miniX = 2200;
	_player->miniY = 1645;
	_player->playNum = 0;

	return S_OK;
}

void startScene::release(void)
{
	gameNode::release();
}

void startScene::update(void)
{
	gameNode::update();
	startBtn->update();
	_player->update();

	rotate(); // 화면 배경 회전.
}

void startScene::render(void)
{
	_player->render();

	IMAGEMANAGER->findImage("startScene")->frameRender(getMemDC(),0,0,currentX,0);
	
	startBtn->render();
}

void startScene::rotate(void)
{
	frameCount++;

	if (startDirec == RIGHT)
	{
		if (frameCount > 15)
		{
			frameCount = 0;
			++currentX;
			if (currentX > 13)
			{
				startDirec = LEFT;
				currentX = 14;
			}
		}
	}

	if (startDirec == LEFT)
	{
		if (frameCount > 15)
		{
			frameCount = 0;
			--currentX;
			if (currentX <= 0)
			{
				startDirec = RIGHT;
				currentX = 1;
			}
		}
	}
}


void startScene::cbStartBtn()
{
	SCENEMANAGER->changeScene("otusHouseScene");
}