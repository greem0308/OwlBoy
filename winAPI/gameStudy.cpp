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

	//�� ���� 
	SCENEMANAGER->addScene("test", new test);
	SCENEMANAGER->addScene("startScene", new startScene);
	SCENEMANAGER->addScene("otusHouseScene", new otusHouseScene);
	SCENEMANAGER->addScene("VellieScene", new VellieScene);
	SCENEMANAGER->addScene("storeScene", new storeScene);
	SCENEMANAGER->addScene("labScene", new labScene);
	SCENEMANAGER->addScene("bombamanScene", new bombamanScene);
	SCENEMANAGER->addScene("eventBridgeScene", new eventBridgeScene);

	//���� ���� _sceneOne���� ����
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


//���� ī�޶�. //��� Ŭ������ ��ӹ޴� �̰����� ī�޶� ��ġ����.
//���� ��� ũ�� 3840,2880
//void gameStudy::velliCameraMove()
//{
//	//���� �÷��̾ �����ʿ� �ְ�, ���x���� - (����̹���-ȭ�������)���� ũ�ٸ�
//	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840 - WINSIZEX)) // ������
//	{
//		cameraX -= _player->_player.x - WINSIZEX / 2;
//		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;
//
//		// �� 
//		door.otusX -= _player->_player.x - WINSIZEX / 2;
//
//		_player->_player.x = WINSIZEX / 2;
//	}
//
//	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
//	{
//		cameraX += WINSIZEX / 2 - _player->_player.x;
//		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;
//
//		// ��
//		door.otusX += WINSIZEX / 2 - _player->_player.x;
//
//		_player->_player.x = WINSIZEX / 2;
//	}
//
//	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880 - WINSIZEY)) // ������
//	{
//		cameraY -= _player->_player.y - WINSIZEY / 2;
//		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;
//
//		//�� 
//		door.otusY -= _player->_player.y - WINSIZEY / 2;
//
//		_player->_player.y = WINSIZEY / 2;
//	}
//
//	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
//	{
//		cameraY += WINSIZEY / 2 - _player->_player.y;
//		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;
//
//		//��
//		door.otusY += WINSIZEY / 2 - _player->_player.y;
//
//		_player->_player.y = WINSIZEY / 2;
//	}
//
//	// �̹� ������ ī�޶� ȭ�������� �Ǿ��־ �÷��̾�x,y�� �׻� WINSIZEX/2,WINSIZEY/2�� �Ǿ��ְ�
//	// ȭ�����ѿ� �ٱ��̿��ԵǸ� ������ ����Ƿ� �ؿ��Ͱ��� �÷��̾� ȭ�������� �ɾ��� �� �ִ�.
//	if (_player->_player.x < 50) _player->_player.x = 50;
//	if (_player->_player.x >WINSIZEX - 50) _player->_player.x = WINSIZEX - 50;
//	if (_player->_player.y < 50) _player->_player.y = 50;
//	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
//}
//
