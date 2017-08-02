#include "stdafx.h"
#include "storeScene.h"


storeScene::storeScene()
{
}


storeScene::~storeScene()
{
}

HRESULT storeScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("storeBG", "Scene/storeBG.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("storeFront", "Scene/storeFront.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("storePixelPink", "Scene/storePixelPink.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("storeBG_blue", "Scene/storeBG_blue.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("storeBG_green", "Scene/storeBG_green.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("storeBG_yellow", "Scene/storeBG_yellow.bmp", 1280, 720, false);

	IMAGEMANAGER->addImage("storeItem1","UI/storeItem1.bmp",1280,720,true,RGB(255,0,255));
	IMAGEMANAGER->addImage("storeItem2", "UI/storeItem2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("storeItem3", "UI/storeItem3.bmp", 1280, 720, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addFrameImage("buyBtn", "UI/buyBtn.bmp", 85,68, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("storeNPC", "Scene/storeNPC.bmp", 506*1.4,60*1.4,11, 1, true, RGB(255, 0, 255));


	_player = new player;
	_player->init(280,400);
	_player->playNum = 3;
	_player->geddy.x = 290;
	_player->geddy.y = 400;

	frameCount = 0;
	CurrentX = 0;

	//���� ��
	rc = RectMake(285,345, 50, 90);

	NPCrc = RectMake(506,350,50,50);
	NPCframeCount=0;
	NPCcurrentX=0;

	storeWindow = false;

	itemCount = 0;

	buyItem1Btn = new button;
	buyItem1Btn->init("buyBtn", 670,526, PointMake(0, 1),
		PointMake(0, 0), cbItem1BuyBtn);
	
	buyItem2Btn = new button;
	buyItem2Btn->init("buyBtn", 670, 526, PointMake(0, 1),
		PointMake(0, 0), cbItem1BuyBtn);
	
	buyItem3Btn = new button;
	buyItem3Btn->init("buyBtn", 670, 526, PointMake(0, 1),
		PointMake(0, 0), cbItem1BuyBtn);


	return S_OK;
}

void storeScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void storeScene::update(void)
{
	gameNode::update();
	_player->update();

	RECT rcTemp;

	// �� 
	if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
			toVellieDoor = 2;
			startDoor = 0;
			SCENEMANAGER->changeScene("VellieScene");
		}
	}

	// NPC
	if (PtInRect( &NPCrc ,_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
		{
			storeWindow = true;
		}
	}

	//������ ������â 
	if (storeWindow)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			itemCount++;
			if (itemCount > 2) itemCount = 0;

		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			itemCount--;
			if (itemCount < 0) itemCount = 2;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			storeWindow = false;
		}
	}


	if (storeWindow)
	{
		if (itemCount == 0)
		{
			buyItem1Btn->update();
		}
		else if (itemCount == 1)
		{
			buyItem2Btn->update();
		}
		else if (itemCount == 2)
		{
			buyItem3Btn->update();
		}
	}

	//NPC animation
	NPCframeCount++;
	if (NPCframeCount>15)
	{
		NPCframeCount = 0;
		NPCcurrentX++;
		if (NPCcurrentX > 10)
		{
			NPCcurrentX = 0;
		}
	}
}

void storeScene::render(void)
{
	// �ڿ� �ȼ����� �ȵǰԲ�.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // ����
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // ��ũ
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // ���
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // �׸�
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // ���ο�

	IMAGEMANAGER->findImage("storePixelPink")->render(getPixel()); // ��ȫ													
	IMAGEMANAGER->findImage("storeBG_blue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("storeBG_green")->render(getPixelGreen());
	IMAGEMANAGER->findImage("storeBG_yellow")->render(getPixelYellow());

	IMAGEMANAGER->findImage("storeBG")->render(getMemDC(), 0, 0); // ���. 
	//Rectangle(getMemDC(), NPCrc.left, NPCrc.top, NPCrc.right, NPCrc.bottom);
	IMAGEMANAGER->findImage("storeNPC")->frameRender(getMemDC(), 495, 325, NPCcurrentX, 0);
	IMAGEMANAGER->findImage("storeFront")->render(getMemDC(), 0, 0); // ���. 
	if (storeWindow)
	{
		if (itemCount == 0)
		{
			buyItem1Btn->render();
			IMAGEMANAGER->findImage("storeItem1")->render(getMemDC());
		}
		else if (itemCount == 1)
		{
			buyItem2Btn->render();
			IMAGEMANAGER->findImage("storeItem2")->render(getMemDC());
		}
		else if (itemCount == 2)
		{
			buyItem3Btn->render();
			IMAGEMANAGER->findImage("storeItem3")->render(getMemDC());
		}
	}

	_player->render();

	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);
}

//��ݰŸ�
void storeScene::cbItem1BuyBtn(void) 
{
	// ��ư �����µ�, ������ī��Ʈ�� 1�̸�, 
	// �κ��丮â�� ������ ���� �޽����µ� Ʈ��Ǹ鼭 �������� ���δ�.
	// ���� �κ��丮â���� �������� Ŭ���ϸ�, ������1 ���� Ʈ��ǰ� ���� �������� ������ �Ʒ������۱׸��� ��������.   
	// �����ͺ��̽��� �÷��̾��� ���ǵ尡 �����Ѵ�. 
	if (itemCount == 0)
	{
		DATABASE->getElementData("player")->shootSpeed = 8.0f;
		DATABASE->getElementData("player")->inventoryOpen = 1; //�κ��丮�� �ڵ����� ����. 
		DATABASE->getElementData("player")->coin -= 300;
	}
	if (itemCount == 1)
	{
		DATABASE->getElementData("player")->maxHP = 100;
		DATABASE->getElementData("player")->inventoryOpen = 1;
		DATABASE->getElementData("player")->coin -= 200;

	}
	if (itemCount == 2)
	{
		DATABASE->getElementData("player")->speed = 5.0f;
		DATABASE->getElementData("player")->inventoryOpen = 1;
		DATABASE->getElementData("player")->coin -= 100;
	}	
}

