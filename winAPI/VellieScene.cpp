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
	
	//�ϴ� 
	// �����Ҷ� �ڿ� ���� �Ⱥ��̰�
	IMAGEMANAGER->addImage("backSkyBG", "vellie/skyBG.bmp", 3840, 2880, true, RGB(255, 0, 255));

	skyBG = IMAGEMANAGER->addImage("skyBG", "vellie/skyBG.bmp", 1280, 720 * 2, true, RGB(255, 0, 255));
	skyLoopFront = IMAGEMANAGER->addImage("skyLoopFront", "vellie/cloudLoopFront.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopMiddle = IMAGEMANAGER->addImage("skyLoopMiddle", "vellie/cloudLoopMiddle.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));
	skyLoopBack = IMAGEMANAGER->addImage("skyLoopBack", "vellie/cloudLoopBack.bmp", 1280, 720 * 1.3, true, RGB(255, 0, 255));

	// vellie ���.
	_background = IMAGEMANAGER->addImage("bg","vellie/velli.bmp",3840,2880,true,RGB(255,0,255));
	// ���� ��
	IMAGEMANAGER->addImage("fruitFront", "vellie/fruitFront.bmp", 3840, 2880, true, RGB(255, 0, 255));
	_backgroundBehind = IMAGEMANAGER->addImage("villeBehindBG", "vellie/villeBehindBG.bmp", 3840, 2880, true, RGB(255, 0, 255));
	
	// �ȼ� �浹 
	_backgroundCheck = IMAGEMANAGER->addImage("villePixelPink", "vellie/villePixelPink.bmp", 3840, 2880,false);
	IMAGEMANAGER->addImage("vellieBlue", "vellie/vellieBlue.bmp", 3840, 2880, false);
	IMAGEMANAGER->addImage("vellieGreen", "vellie/vellieGreen.bmp", 3840, 2880, false);

	IMAGEMANAGER->addImage("miniMap", "UI/miniMap.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("alphaOption", "UI/alpha_option.bmp", 1280, 720, true, RGB(255, 0, 255));

	// ���� ����1,2 
	IMAGEMANAGER->addImage("cloud1", "vellie/cloud1.bmp",3840, 2880, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("cloud2", "vellie/cloud2.bmp",3840, 2880, true, RGB(255, 0, 255));

	//mimimap vellie
	IMAGEMANAGER->addImage("minimapBG", "UI/minimapBG.bmp", 3840 / 20, 2880 / 20, true, RGB(255, 0, 255));

	// ��쳪 
	IMAGEMANAGER->addImage("sounaFront", "vellie/sounaFront.bmp", 3840, 2880, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("SaunaWater","vellie/SaunaWater.bmp",205,39,true,RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("splash", "vellie/splash.bmp",612,51,6,1, true, RGB(255, 0, 255));
	
	sounaFrameCount=0;
	sounaCurrentX=0;
	souna=false;
	sounaX = 2930;
	sounaY = 1422;

	cameraX = 0;
	cameraY = 0;
	
	skyLoopFront_offsetX=0;
	skyLoopMiddle_offsetX=0;
	skyLoopBack_offsetX=0;

	cloud1_offsetX = 0;
	cloud2_offsetX = 0;
	cloudX=0;
	cloudY=0;

	doorPosInit();

	mapRC = RectMake(1060, 500, 3840/20,2880/20);

	itemShowFrame = 0;

	_player = new player;
	_player->init(2200, 1645);
	_player->miniX = 2200;
	_player->miniY = 1645;
	_player->playNum = 2;
	_player->geddy.x = 1874;
	_player->geddy.y = 1564;

	_im = new itemManager;
	_im->init();
	_im->setPlayer(_player);

	minimapFrameCout = 0;
	minimapCurrentX = 0;

	posX = 0;
	posY = 0;

	Create(1);

	ringInit();
	WringInit();

	return S_OK;
}

void VellieScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
	//SAFE_DELETE(_im);
}

void VellieScene::update(void)
{
	gameNode::update();
	_player->update();
	_im->update();
	ringUpdate();
	WringUpdate();
	
	doorPosFunc();

	velliCameraMove();

	collision();

	skyLoopBack_offsetX -= 0.3f;
	skyLoopMiddle_offsetX += 0.2f;
	skyLoopFront_offsetX -= 0.2f;

	cloud1_offsetX -= 0.25f;
	cloud2_offsetX += 0.32f;

	Delete();

	minimapUpdate();

	// minimap ī��Ʈ
	minimapFrameCout++;
	if (minimapFrameCout > 6)
	{
		minimapFrameCout = 0;
		minimapCurrentX++;
		if (minimapCurrentX > 8)
		{
			minimapCurrentX = 0;
		}
	}
}

void VellieScene::render(void)
{
	// �ڿ� �ȼ����� �ȵǰԲ�.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // ����
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // ��ũ
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // ���
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // �׸�
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // ���ο�

	RECT cloudRC = RectMake(cloudX, cloudY, 3840, 2880);
	RECT loopRC = RectMake(_player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2 - 50, WINSIZEX, WINSIZEY + 80);

	IMAGEMANAGER->findImage("backSkyBG")->render(getMemDC());
	skyBG->render(getMemDC(), _player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2);
	skyLoopBack->loopRender(getMemDC(), &loopRC, skyLoopBack_offsetX, 0);
	skyLoopMiddle->loopRender(getMemDC(), &loopRC, skyLoopMiddle_offsetX, 0);
	skyLoopFront->loopRender(getMemDC(), &loopRC, skyLoopFront_offsetX, 0);
	
	IMAGEMANAGER->findImage("cloud2")->loopRender(getMemDC(), &cloudRC, cloud2_offsetX, 0);
	_backgroundBehind->render(getMemDC(), cameraX, cameraY);
	IMAGEMANAGER->findImage("cloud1")->loopRender(getMemDC(), &cloudRC, cloud1_offsetX, 0);

	_background->render(getMemDC(), cameraX, cameraY);
	_backgroundCheck->render(getPixel(), cameraX, cameraY);
	IMAGEMANAGER->findImage("vellieBlue")->render(getPixelBlue(),cameraX, cameraY);
	IMAGEMANAGER->findImage("vellieGreen")->render(getPixelGreen(), cameraX, cameraY);

	ringRenderBehind();
	WringRenderBehind();

	_player->render();

	ringRender();
	WringRender();

	_im->render();

	IMAGEMANAGER->findImage("fruitFront")->render(getMemDC(), cameraX, cameraY);

	//�� ��Ʈ 
	//for (int i = 0; i < 7; i++)
	//{
	//	Rectangle(getMemDC(), door[i].rc.left, door[i].rc.top, door[i].rc.right, door[i].rc.bottom);
	//}


	//IMAGEMANAGER->findImage("SaunaWater")->alphaRender(getMemDC(),sounaX,sounaY,150);
	IMAGEMANAGER->findImage("SaunaWater")->render(getMemDC(), sounaX, sounaY);
	if (souna)
	{
		IMAGEMANAGER->findImage("splash")->frameRender(getMemDC(), _player->_player.x, _player->_player.y, sounaCurrentX, 0);
	}
	IMAGEMANAGER->findImage("sounaFront")->render(getMemDC(),cameraX,cameraY);

	// minimap Render
	IMAGEMANAGER->findImage("miniMap")->render(getMemDC(), 0, 0);
	IMAGEMANAGER->findImage("minimapBG")->render(getMemDC(), 1060, 500);

	//�̴ϸ� ���� �귯����...  
	HBRUSH miniBrush = CreateSolidBrush(RGB(150, 190, 250));
	HPEN miniPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(getMemDC(), miniPen);
	SelectObject(getMemDC(), miniBrush);
	// mini player
	Ellipse(getMemDC(), miniRC.left, miniRC.top, miniRC.right, miniRC.bottom);
	DeleteObject(miniPen);
	DeleteObject(miniBrush);
	if (goDungeon) // ���� �̺�Ʈ������ ������ �ٽ� �Դٸ� ������ġ�� ǥ�õ�. 
	{
		IMAGEMANAGER->findImage("minimapPoint")->frameRender(getMemDC(), 1055, 600, minimapCurrentX, 0);
	}

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
		for (int i = 0; i < 7; i++)
		{
			door[i].x -= _player->_player.x - WINSIZEX / 2;
		}
		// ����ġ�� ������ extern��ġ�� �÷��̾ �ִµ� ī�޶� ����Ǿ��� �����̿��� ����. 
		DoorPos[toVellieDoor].x -= _player->_player.x - WINSIZEX / 2;

		cloudX -= _player->_player.x - WINSIZEX / 2;

  //      // item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.x -= _player->_player.x - WINSIZEX / 2;
		}

		// ��
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].x -= _player->_player.x - WINSIZEX / 2;
			wring[i].x -= _player->_player.x - WINSIZEX / 2;
		}

		sounaX -= _player->_player.x - WINSIZEX / 2;

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

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.x += WINSIZEX / 2 - _player->_player.x;
		}

		// ��
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].x += WINSIZEX / 2 - _player->_player.x;
			wring[i].x += WINSIZEX / 2 - _player->_player.x;
		}

		sounaX += WINSIZEX / 2 - _player->_player.x;

		_player->_player.x = WINSIZEX / 2;
	}

	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880-WINSIZEY)) // ������
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

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.y -= _player->_player.y - WINSIZEY / 2;
		}

		// ��
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].y -= _player->_player.y - WINSIZEY / 2;
			wring[i].y -= _player->_player.y - WINSIZEY / 2;
		}

		sounaY -= _player->_player.y - WINSIZEY / 2;

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

		// item	
		for (int i = 0; i < _im->getItem().size(); ++i)
		{
			_im->getItem()[i]->tem.y += WINSIZEY / 2 - _player->_player.y;
		}

		// ��
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].y += WINSIZEY / 2 - _player->_player.y;
			wring[i].y += WINSIZEY / 2 - _player->_player.y;
		}

		DoorPos[toVellieDoor].y += WINSIZEY / 2 - _player->_player.y;

		sounaY += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// �̹� ������ ī�޶� ȭ�������� �Ǿ��־ �÷��̾�x,y�� �׻� WINSIZEX/2,WINSIZEY/2�� �Ǿ��ְ�
	// ȭ�����ѿ� �ٱ��̿��ԵǸ� ������ ����Ƿ� �ؿ��Ͱ��� �÷��̾� ȭ�������� �ɾ��� �� �ִ�.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX-50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}

void VellieScene::minimapUpdate()
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

// ������Ʈ���� ���ϴ¾�. 
void VellieScene::doorPosFunc()
{
	if (startDoor == 0)
	{
		_player->_player.x = DoorPos[toVellieDoor].x;
		_player->_player.y = DoorPos[toVellieDoor].y;
		_player->geddy.x = DoorPos[toVellieDoor].x+30;
		_player->geddy.y = DoorPos[toVellieDoor].y+30;

		_player->miniX = DoorPos[toVellieDoor].x;
		_player->miniY = DoorPos[toVellieDoor].y;
		startDoor = 1;
	}

	RECT rcTemp;
	for (int i = 0; i < 7; i++)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &door[i].rc))
		{
			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				if (i == 1) // 1�̸� �������� ��Ʈ.
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("otusHouseScene");
				}
				if (i == 2) // ����
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("storeScene");
				}
				if (i == 3) // ��
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("labScene");
				}
				if (i == 4) // �չ���
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("bombamanScene");
				}
				if (i == 5) // �̺�Ʈ �긴��
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("eventBridgeScene");
				}
				if (i == 6) // ����
				{
					toVellieDoor = 0;
					startDoor = 0;
					SCENEMANAGER->changeScene("dungeonScene");
				}
			}
		}
	}

	// �� ��Ʈ
	for (int i = 0; i < 7; i++)
	{
		door[i].rc = RectMake(door[i].x, door[i].y, 40, 100);
	}
}

void VellieScene::doorPosInit()
{
	// �� ��Ʈ ��ġ ��ǥ��
	// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
	door[0].x = -300;
	door[0].y = -300;
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

	// �ٸ������� ������ �Ѿ�ö� �޴� �����ϴ� �÷��̾� ��ǥ��.
	// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
	DoorPos[0].x = 2200;
	DoorPos[0].y = 1645;
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

// init()�� ���ָ� �ѹ��� ��ġ�ǰ� ī�޶� �۵��ؼ� 
// �÷��̾ ������� ���� ������ ��ġ �����Ǿ�����
void VellieScene::Create(int Num)
{
	itemShowFrame++;
	if (itemShowFrame == 1)
	{
		    //���� �ɱ�.
			_im->setFruit0(1264,2040 );
			_im->setFruit0(1350,2040 );
			_im->setFruit1(1400,2040);
			
			_im->setFruit0(2012,1190);
			_im->setFruit2(2080,1190 );
			
			_im->setFruit0(1280,2655 );
			_im->setFruit1(1350, 2670);
			
			// ����
			_im->setFruit0(450,710);
			_im->setFruit1(500, 710);
			_im->setFruit2(550, 710);
			_im->setFruit2(580, 710);
			
			_im->setFruit0(2013,675 );
			_im->setFruit0(2060,675 );
			
			_im->setFruit1(2580,1125);
			_im->setFruit1(2620, 1125);
			
			_im->setFruit2(1300,578);
			_im->setFruit0(1350, 578);
			
			//Ǯ�ɱ�
			_im->setGlassObject(1150,2030);
			
			_im->setGlassObject2(820,1460);
			_im->setGlassObject(890, 1460);
			_im->setGlassObject(940, 1460);
			
			_im->setGlassObject2(2130, 670);
			_im->setGlassObject(2674, 1125);
			
			_im->setGlassObject2(730,2655);
			_im->setGlassObject2(650, 2655);
			_im->setGlassObject2(1250, 2650);
			_im->setGlassObject2(2840,2645);
			_im->setGlassObject(830, 2660);
			_im->setGlassObject(2235,2434);
	}
}


// ����Ǵµ� 
void VellieScene::Delete(void)
{
	for (int i = 0; i < _im->getItem().size(); ++i)
	{
		//if (_im->getItem()[i]->tem.x <300) _im->removeItem(i);
	}
}


// �� ___________________________________________________________________________________________________________�� ����
void VellieScene::ringInit()
{
	IMAGEMANAGER->addFrameImage("treeRingHeight", "item/treeRingHeight.bmp", 840 * 1.5, 280 * 1.5, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("treeRingHeightBehind", "item/treeRingHeightBehind.bmp", 840 * 1.5, 280 * 1.5, 6, 2, true, RGB(255, 0, 255));

	for (int i = 0; i < RINGMAX; i++)
	{
		ring[i].w = 20;
		ring[i].h = 150;
		ring[i].frameCount = 0;
		ring[i].currentX = 0;
		ring[i].eatenFrameCount = 0;
		ring[i].life = true;
		ring[i].eaten = false;

		ring[i].eat = false;
		ring[i].eatCount = 0;

		ring[i].ringState = RING_IDLE;
		ring[i].rc = { 0,0,10,10 }; // �ǹ̾���.����� �Ҷ� ������ ����. 
	}

	ring[0].x = 2310;
	ring[0].y = 1450;
	ring[1].x = 2430;
	ring[1].y = 1450;
	ring[2].x = 2550;
	ring[2].y = 1250;
	ring[3].x = 2670;
	ring[3].y = 1250;

	ring[4].x = 760;
	ring[4].y = 2300;
	ring[5].x = 860;
	ring[5].y = 2300;
	ring[6].x = 960;
	ring[6].y = 2300;

	ring[7].x = 1700;
	ring[7].y = 1900;
	ring[8].x = 1800;
	ring[8].y = 2000;
}

void VellieScene::ringUpdate()
{
	RECT tempRC;
	// ��Ʈ ������Ʈ
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			ring[i].rc = RectMake(ring[i].x, ring[i].y, ring[i].w, ring[i].h);
		}
		else
		{
			ring[i].rc = {0,0,10,10};
		}
	}

	// �÷��̾�� �ε�ġ��, 
	for (int i = 0; i < RINGMAX; i++)
	{
		// �ѹ��� ����.
		if (IntersectRect(&tempRC, &ring[i].rc, &_player->_player.rc))
		{
			ring[i].ringState = RING_EATEN;
			
			ring[i].eaten = true;
			ring[i].eat = true;
			break;
		}
	}

	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].eat)
		{
			ring[i].eatCount++;
			if (ring[i].eatCount % 30 == 0) // �ǰ� �ѹ��� ��Բ�. 
			{
				_player->se12 = true;
				DATABASE->getElementData("player")->coin += 10;
				ring[i].eat = false;
				ring[i].eatCount = 0;
			}
		}
	}

	// �÷��̾�� �ε�ġ��, 
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].eaten)
		{
			ring[i].eatenFrameCount++;
			if (ring[i].eatenFrameCount > 60)
			{
				ring[i].life = false;
				ring[i].eatenFrameCount = 0;
			}
		}
	}

	ringFrameFunc();
}

void VellieScene::ringRender()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			//Rectangle(getMemDC(), ring[i].rc.left, ring[i].rc.top, ring[i].rc.right, ring[i].rc.bottom);

			IMAGEMANAGER->findImage("treeRingHeight")->frameRender(getMemDC(), ring[i].rc.left - 90, ring[i].rc.top - 25,
				ring[i].currentX, ring[i].ringState);
		}
	}
}

// �÷��̾� �ڿ� ���� �� ��,  
void VellieScene::ringRenderBehind()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			IMAGEMANAGER->findImage("treeRingHeightBehind")->frameRender(getMemDC(), ring[i].rc.left - 90, ring[i].rc.top - 25,
				ring[i].currentX, ring[i].ringState);
		}
	}
}

void VellieScene::ringFrameFunc()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		ring[i].frameCount++;
		switch (ring[i].ringState)
		{
		case RING_IDLE:
			if (ring[i].frameCount > 60)
			{
				ring[i].frameCount = 0;
				ring[i].currentX++;
				if (ring[i].currentX > 1)
				{
					ring[i].currentX = 0;
				}
			}
			break;
		case RING_EATEN:
			if (ring[i].frameCount > 12)
			{
				ring[i].frameCount = 0;
				ring[i].currentX++;
				if (ring[i].currentX > 5)
				{
					ring[i].currentX = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}


// �� ___________________________________________________________________________________________________________�� ����
void VellieScene::WringInit()
{
	IMAGEMANAGER->addFrameImage("treeRingWidth", "item/treeRingWidth.bmp", 840 * 1.5, 280 * 1.5, 6, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("treeRingWidthBehind", "item/treeRingWidthBehind.bmp", 840 * 1.5, 280 * 1.5, 6, 2, true, RGB(255, 0, 255));

	for (int i = 0; i < RINGMAX; i++)
	{
		wring[i].w = 150;
		wring[i].h = 20;
		wring[i].frameCount = 0;
		wring[i].currentX = 0;
		wring[i].eatenFrameCount = 0;
		wring[i].life = true;
		wring[i].eaten = false;

		wring[i].wringState = WRING_IDLE;
		wring[i].rc = { 0,0,10,10 }; // �ǹ̾���.����� �Ҷ� ������ ����. 

		wring[i].eat = false;
		wring[i].eatCount = 0;
	}
	wring[0].x = 1500;
	wring[0].y = 400;
	wring[1].x = 1600;
	wring[1].y = 500;
	wring[2].x = 1700;
	wring[2].y = 600;

	wring[5].x = 1200;
	wring[5].y = 1250;
	wring[6].x = 1200;
	wring[6].y = 1350;

	wring[3].x = 3400;
	wring[3].y = 1750;
	wring[4].x = 3400;
	wring[4].y = 1900;
}

void VellieScene::WringUpdate()
{
	RECT tempRC;
	// ��Ʈ ������Ʈ
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			wring[i].rc = RectMake(wring[i].x, wring[i].y, wring[i].w, wring[i].h);
		}
		else
		{
			wring[i].rc = { 0,0,10,10 };
		}
	}

	// �÷��̾�� �ε�ġ��, 

	for (int i = 0; i < RINGMAX; i++)
	{
		// �ѹ��� ����.
		if (IntersectRect(&tempRC, &wring[i].rc, &_player->_player.rc))
		{
			wring[i].wringState = WRING_EATEN;
		
			wring[i].eaten = true;
			wring[i].eat = true;
			break;
		}
	}

	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].eat)
		{
			wring[i].eatCount++;
			if (wring[i].eatCount % 20 == 0) // �ǰ� �ѹ��� ��Բ�. 
			{
				_player->se12 = true;
				DATABASE->getElementData("player")->coin += 10;
				wring[i].eat = false;
				wring[i].eatCount = 0;
			}
		}
	}

	// �÷��̾�� �ε�ġ��, 
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].eaten)
		{
			wring[i].eatenFrameCount++;
			if (wring[i].eatenFrameCount > 60)
			{
				wring[i].life = false;
				wring[i].eatenFrameCount = 0;
			}
		}
	}

	WringFrameFunc();
}

void VellieScene::WringRender()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			//Rectangle(getMemDC(), wring[i].rc.left, wring[i].rc.top, wring[i].rc.right, wring[i].rc.bottom);

			IMAGEMANAGER->findImage("treeRingWidth")->frameRender(getMemDC(), wring[i].rc.left - 25, wring[i].rc.top - 90,
				wring[i].currentX, wring[i].wringState);
		}
	}
}

// �÷��̾� �ڿ� ���� �� ��,  
void VellieScene::WringRenderBehind()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			IMAGEMANAGER->findImage("treeRingWidthBehind")->frameRender(getMemDC(), wring[i].rc.left - 25, wring[i].rc.top - 90,
				wring[i].currentX, wring[i].wringState);
		}
	}
}

void VellieScene::WringFrameFunc()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		wring[i].frameCount++;
		switch (wring[i].wringState)
		{
		case WRING_IDLE:
			if (wring[i].frameCount > 60)
			{
				wring[i].frameCount = 0;
				wring[i].currentX++;
				if (wring[i].currentX > 1)
				{
					wring[i].currentX = 0;
				}
			}
			break;
		case WRING_EATEN:
			if (wring[i].frameCount > 12)
			{
				wring[i].frameCount = 0;
				wring[i].currentX++;
				if (wring[i].currentX > 5)
				{
					wring[i].currentX = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}



void VellieScene::collision()
{
	RECT tempRC;

	// �÷��̾� �Ѿ˰� ���� �浹 _____________________________________________________________________________________
	for (int i = 0; i < _im->getItem().size(); ++i)
	{
		for (int j = 0; j < _player->_player._fire->getVBullet().size(); j++)
		{
			if (IntersectRect(&tempRC, &_player->_player._fire->getVBullet()[j].rc, &_im->getItem()[i]->tem.rc))
			{
				_im->getItem()[i]->tem.hitCheck = true;
				//_player->otherHit = true;
				//_player->effectFire();
				_player->removeMissile(j);
			}
		}
	}
}