#include "stdafx.h"
#include "BossScene.h"


BossScene::BossScene()
{
}


BossScene::~BossScene()
{
}


HRESULT BossScene::init(void)
{
	gameNode::init();

	IMAGEMANAGER->addFrameImage("bossShip", "boss/bossShip.bmp",1293*1.8,283*1.8,3,1,true,RGB(255,0,255));

	IMAGEMANAGER->addImage("bossBG", "boss/bossBG.bmp", 2560,1440, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("rockRight", "boss/rockRight.bmp", 270,50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("rockLeft", "boss/rockLeft.bmp", 270, 50, true, RGB(255, 0, 255));

	// waterFall
	IMAGEMANAGER->addFrameImage("waterFall", "dungeon/waterFall.bmp", 351 * 1.5, 159 * 1.5, 3, 1, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(100,100);

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);

	frameCount = 0;
	currentX = 0;

	cameraX = 0;
	cameraY = 0;

	return S_OK;
}

void BossScene::release(void)
{
	gameNode::release();
	SAFE_DELETE(_player);
}

void BossScene::update(void)
{
	gameNode::update();
	_player->update();

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
		SCENEMANAGER->changeScene("VellieScene");
	}

	frameCount++;
	if (frameCount > 20)
	{
		frameCount = 0;
		currentX++;
		if (currentX > 1)
		{
			currentX = 0;
		}
	}

	bossCameraMove();
}

void BossScene::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getPixel());
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());

	IMAGEMANAGER->findImage("bossBG")->render(getMemDC(), cameraX, cameraY);

	IMAGEMANAGER->findImage("bossShip")->frameRender(getMemDC(),50,1000,currentX,0);
	IMAGEMANAGER->findImage("rockRight")->render(getMemDC(), 0, 500);
	IMAGEMANAGER->findImage("rockLeft")->render(getMemDC(), 300, 500);

	_player->render();
	Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}


void BossScene::bossCameraMove()
{

	//만약 플레이어가 오른쪽에 있고, 배경x축이 - (배경이미지-화면사이즈)보다 크다면
	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(2560 - WINSIZEX)) // 오른쪽
	{
		cameraX -= _player->_player.x - WINSIZEX / 2;
		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
	{
		cameraX += WINSIZEX / 2 - _player->_player.x;
		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(1440 - WINSIZEY)) // 오른쪽
	{
		cameraY -= _player->_player.y - WINSIZEY / 2;
		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;

		_player->_player.y = WINSIZEY / 2;
	}

	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
	{
		cameraY += WINSIZEY / 2 - _player->_player.y;
		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// 이미 위에서 카메라 화면제한이 되어있어서 플레이어x,y는 항상 WINSIZEX/2,WINSIZEY/2로 되어있고
	// 화면제한에 다까이오게되면 중점을 벗어나므로 밑에와같이 플레이어 화면제한을 걸어줄 수 있다.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX - 50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}