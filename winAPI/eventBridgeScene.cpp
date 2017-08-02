#include "stdafx.h"
#include "eventBridgeScene.h"


eventBridgeScene::eventBridgeScene()
{
}


eventBridgeScene::~eventBridgeScene()
{
}

HRESULT eventBridgeScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("BG", "Scene/eventBridge/eventBridgeBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	backgroundPink = IMAGEMANAGER->addImage("PixelPink", "Scene/eventBridge/eventBridgePixelPink.bmp", 1280, 720, false);

	behindBG = IMAGEMANAGER->addImage("BehindBG", "Scene/eventBridge/eventBridgeBehindBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("SkyBG","Scene/eventBridge/eventBridgeSkyBG.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("cloudLoopBG", "Scene/eventBridge/cloudLoopBG.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("monk", "Scene/eventBridge/monk.bmp", 2210*1.3, 130 * 1.3, 17, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("clock", "Scene/eventBridge/clock.bmp", 1700, 100, 17, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("talk", "UI/talk.bmp", 50,40,2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("monkTalk", "UI/monkTalk.bmp", 150, 50, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("nightSky", "vellie/nightSky.bmp", WINSIZEX, WINSIZEY,true, RGB(255, 0, 255));

	_player = new player;
	_player->init(30, 450);
	_player->playNum = 8;
	_player->geddy.x = 40;
	_player->geddy.y = 450;

	monk_frameCount = 0;
	monk_CurrentX = 0;
	
	// ��
	rc = RectMake(30,450, 50, 90);
	//�̺�Ʈ ��
	eventRC = RectMake(30,450,50,90);
	//monkRC
	monkRC = RectMake(680,265,50,50);

	offsetX = 0;
	offsetcloudX = 0;

	talk = false;
	talkFrame = 0;
	talkCurrentX = 0;

	monkTalk = false;
	monkTalkFrame = 0;

	eventBridgeStart = false;
	eventBridgeEnd = false;

	nightAlpha = 0;

	//event accel
	accelX = 0.3f; //+
	accelCoundX = 0.3f; //-

	nightskyFrame = 0;

	clockFrame = 0;
	clockCurrentX = 0;

	eventDoor = false;

	return S_OK;
}

void eventBridgeScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void eventBridgeScene::update(void)
{
	gameNode::update();
	_player->update();

	offsetX += accelX;
	offsetcloudX -= accelCoundX;

	RECT rcTemp;
	if (!eventDoor)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &rc))
		{
			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
				toVellieDoor = 5;
				startDoor = 0;
				SCENEMANAGER->changeScene("VellieScene");
			}
		}
	}

	if (eventDoor)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &eventRC))
		{
			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
				toVellieDoor = 5;
				startDoor = 0;
				SCENEMANAGER->changeScene("eventScene");
			}
		}
	}

	eventFunc();    
}

void eventBridgeScene::render(void)
{
	// �ڿ� �ȼ����� �ȵǰԲ�.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // ����
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // ��ũ
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // ���
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // �׸�
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // ���ο�

	IMAGEMANAGER->findImage("PixelPink")->render(getPixel()); // ��ȫ.

	RECT loopRC = RectMake(0,0,1280,720); //�������� �׸� ���� ��Ʈ
	IMAGEMANAGER->findImage("SkyBG")->loopRender(getMemDC(),&loopRC, offsetX,0);// ��ī�� ����
	// ��.
	IMAGEMANAGER->findImage("nightSky")->alphaRender(getMemDC(), nightAlpha);
	IMAGEMANAGER->findImage("BehindBG")->render(getMemDC(), 0, 0); // �޹��.
	IMAGEMANAGER->findImage("cloudLoopBG")->loopRender(getMemDC(), &loopRC, offsetcloudX, 0);// ���� ����

	IMAGEMANAGER->findImage("BG")->render(getMemDC(), 0, 0); // �չ��. 
	
	IMAGEMANAGER->findImage("monk")->frameRender(getMemDC(),monkRC.left-70,monkRC.top-70, monk_CurrentX,0);
	//Rectangle(getMemDC(), monkRC.left, monkRC.top,monkRC.right,monkRC.bottom);

	IMAGEMANAGER->findImage("clock")->frameRender(getMemDC(),958,255, clockCurrentX, 0);

	_player->render();

	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);

	HBRUSH playerH = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH playerM = CreateSolidBrush(RGB(100, 0, 255));
	SelectObject(getMemDC(), playerH);
	Rectangle(getMemDC(), _player->_player.HPbar.left, _player->_player.HPbar.top, _player->_player.HPbar.right, _player->_player.HPbar.bottom);
	DeleteObject(playerH);

	if (talk)
	{
		// talk�̹����� ����
		IMAGEMANAGER->findImage("talk")->frameRender(getMemDC(), monkRC.left, monkRC.top - 70, talkCurrentX, 0);
	}
	if (monkTalk)
	{
		//������ ���ϴ°� ����. 
		IMAGEMANAGER->findImage("monkTalk")->render(getMemDC(),monkRC.left-40, monkRC.top-68);
	}

	// ��.
	IMAGEMANAGER->findImage("nightSky")->alphaRender(getMemDC(),nightAlpha);
}


void eventBridgeScene:: eventFunc(void)
{
	RECT rcTemp;
	// ������ �ִϸ��̼� ������
	monk_frameCount++;
	if (monk_frameCount > 18)
	{
		monk_frameCount = 0;
		monk_CurrentX++;
		if (monk_CurrentX > 16)
		{
			monk_CurrentX = 0;
		}
	}

	// ���� �÷��̾�� �������� �ε�ġ�� �� �̸�Ƽ���� �߰�, ������ Ŭ���ϸ� �������� ���ϰ�,(���ϴ� �̹��� �߰�.)
	// ���� ���� �� �ð� �������� ���ӵ��� �ް� �����ð��� ������ �������鼭 �����. 
	//�׸��� ���� ���οƮ�� Ʈ��ǰ� �����մ� �����ΰ��� ����Ʈ�� �޽��ȴ�. 
	if (IntersectRect(&rcTemp, &_player->_player.rc, &monkRC))
	{
		talk = true;

		if (KEYMANAGER->isOnceKeyDown(MK_RBUTTON))
		{
			monkTalk = true;
			talk = false;
		}
	}
	else { talk = false; }

	if (monkTalk)
	{
		// ������ ���� 2�ʰ� ���ص� �� �޽��ǰ�
		// �̺�Ʈ ��ġ �۵�����. 
		monkTalkFrame++;
		if (monkTalkFrame == 119)
		{
			_player->se3 = true;
		}
		if (monkTalkFrame > 120)
		{
			monkTalk = false;
			eventBridgeStart = true;
		}
	}

	// talk�̸�Ƽ�� ������. 
	talkFrame++;
	if (talkFrame > 30)
	{
		talkFrame = 0;
		talkCurrentX++;
		if (talkCurrentX > 1)
		{
			talkCurrentX = 0;
		}
	}

	// ���ӵ� ����.
	if (eventBridgeStart)
	{
		eventDoor = true; // event������ ���� �� ����. 

		accelX += 0.04f;
		accelCoundX += 0.04f;

		nightskyFrame++;
		if (nightskyFrame > 3) //���� ������ ��ο����Բ� ������ �ɾ����. 
		{
			nightAlpha += 1;
			nightskyFrame = 0;
		}
		if (nightAlpha > 150)
		{
			nightAlpha = 150;
		}

		clockFrame++;
		if (clockFrame > 2)
		{
			clockFrame = 0;
			clockCurrentX++;
			if (clockCurrentX > 15)
			{
				clockCurrentX = 0;
			}
		}

		if (accelX > 22 && accelCoundX > 22)
		{
			eventBridgeStart = false;
			eventBridgeEnd = true;
		}
	}

	// ���ӵ��� ���� ������
	if (eventBridgeEnd)
	{
		accelX -= 0.05f;
		accelCoundX -= 0.05f;

		clockFrame++;
		if (clockFrame > 4)
		{
			clockFrame = 0;
			clockCurrentX++;
			if (clockCurrentX > 15)
			{
				clockCurrentX = 0;
			}
		}

		if (accelX <= 0.3f && accelCoundX <= 0.3f)
		{
			eventBridgeEnd = false;
		}
	}
}