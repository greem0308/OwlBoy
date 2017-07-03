#include "stdafx.h"
#include "VellieScene.h"


VellieScene::VellieScene()
{
}


VellieScene::~VellieScene()
{
}

HRESULT VellieScene::init(void)
{
	gameNode::init();

	//_background = IMAGEMANAGER->addImage("bg", "pixelTest/bg.bmp", 1280, 720, true);
	//_backgroundCheck = IMAGEMANAGER->addImage("pixelBG", "pixelTest/pixelBG.bmp", 1280, 720, false);

	_background = IMAGEMANAGER->addImage("bg","Scene/velli.bmp",3840,2880,true,RGB(255,0,255));
	_backgroundCheck = IMAGEMANAGER->addImage("villePixelPink", "Scene/villePixelPink.bmp", 3840, 2880,false);

	IMAGEMANAGER->addImage("miniMap", "UI/miniMap.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("alphaOption", "UI/alpha_option.bmp", 1280, 720, true, RGB(255, 0, 255));

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);
	
	_player = new player;
	_player->init(500,0);

	cameraX = 0;
	cameraY = 0;

	door.otusX = 2200;
	door.otusY = 1645;

	door.otusRC = RectMake(door.otusX, door.otusY, 35, 60);
	return S_OK;
}

void VellieScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void VellieScene::update(void)
{
	gameNode::update();

	RECT rcTemp;
	//if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	//{
	//	if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
	//	{
	//		_player->_player.hp += 2;
	//	}
	//}
	//if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	//{
	//	DATABASE->getElementData("player")->hp = _player->_player.hp;
	//	SCENEMANAGER->changeScene("otusHouseScene");
	//}

	if (IntersectRect(&rcTemp, &_player->_player.rc, &door.otusRC))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			SCENEMANAGER->changeScene("otusHouseScene");
		}
	}

	door.otusRC = RectMake(door.otusX, door.otusY, 35, 60);
	velliCameraMove();
	_player->update();
}

void VellieScene::render(void)
{
	_background->render(getMemDC(), cameraX, cameraY);
	_backgroundCheck->render(getPixel(), cameraX, cameraY);
	
	IMAGEMANAGER->findImage("miniMap")->render(getMemDC());

	_player->render();

	//�� 
	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

	//otusScene���� ���� ��Ʈ
	Rectangle(getMemDC(), door.otusRC.left, door.otusRC.top, door.otusRC.right, door.otusRC.bottom);
}

//���� ī�޶�. //��� Ŭ������ ��ӹ޴� �̰����� ī�޶� ��ġ����.
//���� ��� ũ�� 3840,2880
void VellieScene::velliCameraMove()
{
	//���� �÷��̾ �����ʿ� �ְ�, ���x���� - (����̹���-ȭ�������)���� ũ�ٸ�
	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840-WINSIZEX)) // ������
	{
		cameraX -= _player->_player.x - WINSIZEX / 2;  
		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;

		// �� 
		door.otusX -= _player->_player.x - WINSIZEX / 2;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
	{
		cameraX += WINSIZEX / 2 - _player->_player.x;
		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;

		// ��
		door.otusX += WINSIZEX / 2 - _player->_player.x;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880-WINSIZEY)) // ������
	{
		cameraY -= _player->_player.y - WINSIZEY / 2; 
		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;

		//�� 
		door.otusY -= _player->_player.y - WINSIZEY / 2;

		_player->_player.y = WINSIZEY / 2;
	}

	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
	{
		cameraY += WINSIZEY / 2 - _player->_player.y;
		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;

		//��
		door.otusY += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// �̹� ������ ī�޶� ȭ�������� �Ǿ��־ �÷��̾�x,y�� �׻� WINSIZEX/2,WINSIZEY/2�� �Ǿ��ְ�
	// ȭ�����ѿ� �ٱ��̿��ԵǸ� ������ ����Ƿ� �ؿ��Ͱ��� �÷��̾� ȭ�������� �ɾ��� �� �ִ�.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX-50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}
