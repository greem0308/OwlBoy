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

	//�ϴ� 
	// �����Ҷ� �ڿ� ���� �Ⱥ��̰�
	IMAGEMANAGER->addImage("backSkyBG", "vellie/skyBG.bmp", 3840, 2880, true, RGB(255, 0, 255));

	skyBG = IMAGEMANAGER->addImage("skyBG", "vellie/skyBG.bmp", 1280, 720 * 2, true, RGB(255, 0, 255));
	skyLoopFront = IMAGEMANAGER->addImage("skyLoopFront", "vellie/cloudLoopFront.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopMiddle = IMAGEMANAGER->addImage("skyLoopMiddle", "vellie/cloudLoopMiddle.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopBack = IMAGEMANAGER->addImage("skyLoopBack", "vellie/cloudLoopBack.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));

	// vellie ���.
	_background = IMAGEMANAGER->addImage("bg", "vellie/velli.bmp", 3840, 2880, true, RGB(255, 0, 255));
	_backgroundBehind = IMAGEMANAGER->addImage("villeBehindBG", "vellie/villeBehindBG.bmp", 3840, 2880, true, RGB(255, 0, 255));
	_backgroundCheck = IMAGEMANAGER->addImage("villePixelPink", "vellie/villePixelPink.bmp", 3840, 2880, false);

	IMAGEMANAGER->addImage("miniMap", "UI/miniMap.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("alphaOption", "UI/alpha_option.bmp", 1280, 720, true, RGB(255, 0, 255));

	// ���� ����1,2 
	IMAGEMANAGER->addImage("cloud1", "vellie/cloud1.bmp", 3840, 2880, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("cloud2", "vellie/cloud2.bmp", 3840, 2880, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("nightSky1", "vellie/nightSky1.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	//mimimap vellie
	IMAGEMANAGER->addImage("minimapBG", "UI/minimapBG.bmp", 3840 / 20, 2880 / 20, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(3037,2222);
	_player->miniX = 3037;
	_player->miniY = 2222;

	cameraX = 0;
	cameraY = 0;

	skyLoopFront_offsetX = 0;
	skyLoopMiddle_offsetX = 0;
	skyLoopBack_offsetX = 0;

	cloud1_offsetX = 0;
	cloud2_offsetX = 0;
	cloudX = 0;
	cloudY = 0;

	doorPosInit();

	em = new enemyManager;
	em->init();
	em->setPlayer(_player);
	shipShowFrame = 0;

	mapRC = RectMake(1060, 500, 3840 / 20, 2880 / 20);

	return S_OK;
}

void eventScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
	//SAFE_DELETE(em);
}

void eventScene::update(void)
{
	gameNode::update();
	em->update();

	doorPosFunc();

	velliCameraMove();
	_player->update();

	skyLoopBack_offsetX -= 0.3f;
	skyLoopMiddle_offsetX += 0.2f;
	skyLoopFront_offsetX -= 0.2f;

	cloud1_offsetX -= 0.25f;
	cloud2_offsetX += 0.32f;

	int rand = RND->getFromIntTo(0,50);
	Create(rand);
	Delete();

	minimapUpdate();
}

void eventScene::render(void)
{
	RECT cloudRC = RectMake(cloudX, cloudY, 3840, 2880);
	RECT loopRC = RectMake(_player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2 - 50, WINSIZEX, WINSIZEY + 80);

	IMAGEMANAGER->findImage("backSkyBG")->render(getMemDC());
	
	skyBG->render(getMemDC(), _player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2);
	skyLoopBack->loopRender(getMemDC(), &loopRC, skyLoopBack_offsetX, 0);
	skyLoopMiddle->loopRender(getMemDC(), &loopRC, skyLoopMiddle_offsetX, 0);
	skyLoopFront->loopRender(getMemDC(), &loopRC, skyLoopFront_offsetX, 0);

	// ��.
	//IMAGEMANAGER->findImage("nightSky1")->alphaRender(getMemDC(), 90);

	IMAGEMANAGER->findImage("cloud2")->loopRender(getMemDC(), &cloudRC, cloud2_offsetX, 0);
	_backgroundBehind->render(getMemDC(), cameraX, cameraY);
	IMAGEMANAGER->findImage("cloud1")->loopRender(getMemDC(), &cloudRC, cloud1_offsetX, 0);

	_background->render(getMemDC(), cameraX, cameraY);
	_backgroundCheck->render(getPixel(), cameraX, cameraY);

	// ��
	IMAGEMANAGER->findImage("nightSky1")->alphaRender(getMemDC(), 170);

	IMAGEMANAGER->findImage("miniMap")->render(getMemDC());

	_player->render();

	//�� ��Ʈ 
	for (int i = 0; i < 7; i++)
	{
		Rectangle(getMemDC(), door[i].rc.left, door[i].rc.top, door[i].rc.right, door[i].rc.bottom);
	}

	em->render();

	// minimap Render
	IMAGEMANAGER->findImage("miniMap")->render(getMemDC(), 0, 0);
	IMAGEMANAGER->findImage("minimapBG")->render(getMemDC(), 1060, 500);

	HBRUSH miniBrush = CreateSolidBrush(RGB(150, 190, 250));
	HPEN miniPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(getMemDC(), miniPen);
	SelectObject(getMemDC(), miniBrush);
	// mini player
	Ellipse(getMemDC(), miniRC.left, miniRC.top, miniRC.right, miniRC.bottom);
	DeleteObject(miniPen);
	DeleteObject(miniBrush);
}

//���� ī�޶�. //��� Ŭ������ ��ӹ޴� �̰����� ī�޶� ��ġ����.
//���� ��� ũ�� 3840,2880
void eventScene::velliCameraMove()
{
	//���� �÷��̾ �����ʿ� �ְ�, ���x���� - (����̹���-ȭ�������)���� ũ�ٸ�
	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840 - WINSIZEX)) // ������
	{
		cameraX -= _player->_player.x - WINSIZEX / 2;
		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;

		// �� 
		for (int i = 0; i < 7; i++)
		{
			door[i].x -= _player->_player.x - WINSIZEX / 2;
		}
		// ����ġ�� ������ extern��ġ�� �÷��̾ �ִµ� ī�޶� ����Ǿ��� �����̿��� ����. 
		DoorPos[toVellieDoor].x -= _player->_player.x - WINSIZEX / 2;

		cloudX -= _player->_player.x - WINSIZEX / 2;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
	{
		cameraX += WINSIZEX / 2 - _player->_player.x;
		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;

		// ��
		for (int i = 0; i < 7; i++)
		{
			door[i].x += WINSIZEX / 2 - _player->_player.x;
		}

		cloudX += WINSIZEX / 2 - _player->_player.x;

		DoorPos[toVellieDoor].x += WINSIZEX / 2 - _player->_player.x;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880 - WINSIZEY)) // ������
	{
		cameraY -= _player->_player.y - WINSIZEY / 2;
		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;

		//�� 
		for (int i = 0; i < 7; i++)
		{
			door[i].y -= _player->_player.y - WINSIZEY / 2;
		}
		DoorPos[toVellieDoor].y -= _player->_player.y - WINSIZEY / 2;

		cloudY -= _player->_player.y - WINSIZEY / 2;

		_player->_player.y = WINSIZEY / 2;
	}

	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // ���ʿ��ְ�, ī�޶�X�� 0���� ũ�ٸ�,
	{
		cameraY += WINSIZEY / 2 - _player->_player.y;
		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;

		//��
		for (int i = 0; i < 7; i++)
		{
			door[i].y += WINSIZEY / 2 - _player->_player.y;
		}

		cloudY += WINSIZEY / 2 - _player->_player.y;

		DoorPos[toVellieDoor].y += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// �̹� ������ ī�޶� ȭ�������� �Ǿ��־ �÷��̾�x,y�� �׻� WINSIZEX/2,WINSIZEY/2�� �Ǿ��ְ�
	// ȭ�����ѿ� �ٱ��̿��ԵǸ� ������ ����Ƿ� �ؿ��Ͱ��� �÷��̾� ȭ�������� �ɾ��� �� �ִ�.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX - 50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}

void eventScene::doorPosFunc()
{
	if (startDoor == 0)
	{
		_player->_player.x = DoorPos[toVellieDoor].x;
		_player->_player.y = DoorPos[toVellieDoor].y;
		startDoor = 1;
	}

	RECT rcTemp;
	for (int i = 0; i < 7; i++)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &door[i].rc))
		{
			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				//if (i == 1) // 1�̸� �������� ��Ʈ.
				//{
				//	toVellieDoor = 0;
				//	startDoor = 0;
				//	SCENEMANAGER->changeScene("otusHouseScene");
				//}
				//if (i == 2) // ����
				//{
				//	toVellieDoor = 0;
				//	startDoor = 0;
				//	SCENEMANAGER->changeScene("storeScene");
				//}
				if (i == 3) // ��
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("labScene");
				}
				//if (i == 4) // �չ���
				//{
				//	toVellieDoor = 0;
				//	startDoor = 0;
				//	SCENEMANAGER->changeScene("bombamanScene");
				//}
				//if (i == 5) // �̺�Ʈ �긴��
				//{
				//	toVellieDoor = 0;
				//	startDoor = 0;
				//	SCENEMANAGER->changeScene("eventBridgeScene");
				//}
				//if (i == 6) // ����
				//{
				//	toVellieDoor = 0;
				//	startDoor = 0;
				//	SCENEMANAGER->changeScene("otusHouseScene");
				//}
			}
		}
	}

	// �� ��Ʈ
	for (int i = 0; i < 7; i++)
	{
		door[i].rc = RectMake(door[i].x, door[i].y, 40, 100);
	}
}

void eventScene::doorPosInit()
{
	// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
	door[0].x = 100;
	door[0].y = 100;
	door[1].x = 2200;
	door[1].y = 1645;
	door[2].x = 813;
	door[2].y = 640;
	door[3].x = 2746;
	door[3].y = 407;
	door[4].x = 1693;
	door[4].y = 903;
	door[5].x = 3037;
	door[5].y = 2222;
	door[6].x = 341;
	door[6].y = 2612;

	// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
	DoorPos[0].x = 100;
	DoorPos[0].y = 100;
	DoorPos[1].x = 2200;
	DoorPos[1].y = 1645;
	DoorPos[2].x = 813;
	DoorPos[2].y = 640;
	DoorPos[3].x = 2746;
	DoorPos[3].y = 407;
	DoorPos[4].x = 1693;
	DoorPos[4].y = 903;
	DoorPos[5].x = 3037;
	DoorPos[5].y = 2222;
	DoorPos[6].x = 341;
	DoorPos[6].y = 2612;
}


void eventScene::Create(int Num)
{
	shipShowFrame++;
	if (shipShowFrame == 1)
	{
		em->setEventShip(WINSIZEX/2, 500);
		em->setEventShip(1000, 1000);

		em->setEventShip(WINSIZEX / 2, 2000);
		em->setEventShip(2000, 2000);
	}
}

// ����Ǵµ� 
void eventScene::Delete(void)
{
	for (int i = 0; i < em->getMinion().size(); ++i)
	{
		if (em->getMinion()[i]->_enemy.x <300) em->removeMinion(i);
	}
}

void eventScene::minimapUpdate()
{
	// 20���� ������ �̵������� ���δ�. �÷��̾�x,y�� �̹� ī�޶� �����־
	// �÷��̾� Ŭ�������� ���� ��ǥ�� ���� ī�޶� �����������鼭 �÷��̾�� �Ȱ��� �����̰� �Ͽ���.
	miniRC = RectMakeCenter(_player->miniX / 20 + 1060, _player->miniY / 20 + 500, 10, 10);

	// �̴��� ������ ȭ�� ���ʺ��� �۾����ٸ�, 
	if (miniRC.left < mapRC.left)
	{
		miniRC.left = mapRC.left;
		miniRC.right = miniRC.left + 10;
	}
	if (miniRC.right > mapRC.right)
	{
		miniRC.right = mapRC.right;
		miniRC.left = miniRC.right - 10;
	}
	if (miniRC.top < mapRC.top)
	{
		miniRC.top = mapRC.top;
		miniRC.bottom = miniRC.top + 10;
	}
	if (miniRC.bottom > mapRC.bottom)
	{
		miniRC.bottom = mapRC.bottom;
		miniRC.top = miniRC.bottom - 10;
	}
}