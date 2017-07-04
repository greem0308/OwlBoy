#include "StdAfx.h"
#include "gameStudy.h"


gameStudy::gameStudy(void)
{
}

gameStudy::~gameStudy(void)
{
}

HRESULT gameStudy::init(void)
{
	gameNode::init(true);

	IMAGEMANAGER->addImage("space2", "space2.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("mapImage", "backImage.bmp", WINSIZEX, WINSIZEY);

	//씬 생성 
	SCENEMANAGER->addScene("test", new test);
	SCENEMANAGER->addScene("startScene", new startScene);
	SCENEMANAGER->addScene("otusHouseScene", new otusHouseScene);
	SCENEMANAGER->addScene("VellieScene", new VellieScene);
	SCENEMANAGER->addScene("storeScene", new storeScene);
	SCENEMANAGER->addScene("labScene", new labScene);
	SCENEMANAGER->addScene("bombamanScene", new bombamanScene);
	SCENEMANAGER->addScene("eventBridgeScene", new eventBridgeScene);

	//현재 씬을 _sceneOne으로 설정
	SCENEMANAGER->changeScene("startScene");

	//_velli = new VellieScene;
	//_velli->init();

	return S_OK;
}

void gameStudy::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_velli);
}

void gameStudy::update(void)
{
	gameNode::update();
	SCENEMANAGER->update();
	//_velli->update();
	//velliCameraMove();
}

void gameStudy::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());
	SCENEMANAGER->render();
	//_velli->render();
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}


//마을 카메라. //모든 클래스를 상속받는 이곳에서 카메라를 설치하자.
//마을 배경 크기 3840,2880
//void gameStudy::velliCameraMove()
//{
//	//만약 플레이어가 오른쪽에 있고, 배경x축이 - (배경이미지-화면사이즈)보다 크다면
//	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840 - WINSIZEX)) // 오른쪽
//	{
//		cameraX -= _player->_player.x - WINSIZEX / 2;
//		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;
//
//		// 문 
//		door.otusX -= _player->_player.x - WINSIZEX / 2;
//
//		_player->_player.x = WINSIZEX / 2;
//	}
//
//	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
//	{
//		cameraX += WINSIZEX / 2 - _player->_player.x;
//		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;
//
//		// 문
//		door.otusX += WINSIZEX / 2 - _player->_player.x;
//
//		_player->_player.x = WINSIZEX / 2;
//	}
//
//	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880 - WINSIZEY)) // 오른쪽
//	{
//		cameraY -= _player->_player.y - WINSIZEY / 2;
//		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;
//
//		//문 
//		door.otusY -= _player->_player.y - WINSIZEY / 2;
//
//		_player->_player.y = WINSIZEY / 2;
//	}
//
//	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,
//	{
//		cameraY += WINSIZEY / 2 - _player->_player.y;
//		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;
//
//		//문
//		door.otusY += WINSIZEY / 2 - _player->_player.y;
//
//		_player->_player.y = WINSIZEY / 2;
//	}
//
//	// 이미 위에서 카메라 화면제한이 되어있어서 플레이어x,y는 항상 WINSIZEX/2,WINSIZEY/2로 되어있고
//	// 화면제한에 다까이오게되면 중점을 벗어나므로 밑에와같이 플레이어 화면제한을 걸어줄 수 있다.
//	if (_player->_player.x < 50) _player->_player.x = 50;
//	if (_player->_player.x >WINSIZEX - 50) _player->_player.x = WINSIZEX - 50;
//	if (_player->_player.y < 50) _player->_player.y = 50;
//	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
//}
//
