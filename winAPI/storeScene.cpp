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

	//상점 문
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

	// 문 
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

	//상점템 윈도우창 
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
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

	IMAGEMANAGER->findImage("storePixelPink")->render(getPixel()); // 분홍													
	IMAGEMANAGER->findImage("storeBG_blue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("storeBG_green")->render(getPixelGreen());
	IMAGEMANAGER->findImage("storeBG_yellow")->render(getPixelYellow());

	IMAGEMANAGER->findImage("storeBG")->render(getMemDC(), 0, 0); // 배경. 
	//Rectangle(getMemDC(), NPCrc.left, NPCrc.top, NPCrc.right, NPCrc.bottom);
	IMAGEMANAGER->findImage("storeNPC")->frameRender(getMemDC(), 495, 325, NPCcurrentX, 0);
	IMAGEMANAGER->findImage("storeFront")->render(getMemDC(), 0, 0); // 배경. 
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

//사격거리
void storeScene::cbItem1BuyBtn(void) 
{
	// 버튼 눌렀는데, 아이템카운트가 1이면, 
	// 인벤토리창에 아이템 불이 펄스였는데 트루되면서 아이템이 보인다.
	// 만약 인벤토리창에서 아이템을 클릭하면, 아이템1 불이 트루되고 위의 아이템이 켜지고 아래아이템그림이 없어진다.   
	// 데이터베이스의 플레이어의 스피드가 증가한다. 
	if (itemCount == 0)
	{
		DATABASE->getElementData("player")->shootSpeed = 8.0f;
		DATABASE->getElementData("player")->inventoryOpen = 1; //인벤토리를 자동으로 연다. 
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

