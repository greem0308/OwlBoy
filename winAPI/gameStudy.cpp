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

	IMAGEMANAGER->addFrameImage("arrow", "UI/arrow.bmp", 216*2,108*2,6,3, true, RGB(255, 0, 255));

	//�� ���� 
	SCENEMANAGER->addScene("test", new test);
	SCENEMANAGER->addScene("startScene", new startScene);
	SCENEMANAGER->addScene("otusHouseScene", new otusHouseScene);
	SCENEMANAGER->addScene("VellieScene", new VellieScene);
	SCENEMANAGER->addScene("storeScene", new storeScene);
	SCENEMANAGER->addScene("labScene", new labScene);
	SCENEMANAGER->addScene("bombamanScene", new bombamanScene);
	SCENEMANAGER->addScene("eventBridgeScene", new eventBridgeScene);
	SCENEMANAGER->addScene("eventScene", new eventScene);

	//���� ���� _sceneOne���� ����
	SCENEMANAGER->changeScene("VellieScene");

	//_velli = new VellieScene;
	//_velli->init();
	
	arrowFrameCount = 0;
	arrowCurrentX=0;
	arrowCurrentY = 0;
	arrowState=arrowIDLE;
	mouseGun = false;
	GeddyThrow = false;

	SetCursor(NULL);
	ShowCursor(FALSE);
	
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

	mouseFunc();
}

void gameStudy::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());
	SCENEMANAGER->render();
	//_velli->render();
	TIMEMANAGER->render(getMemDC());

	IMAGEMANAGER->findImage("arrow")->frameRender(getMemDC(), _ptMouse.x, _ptMouse.y,arrowCurrentX,arrowCurrentY);

	this->getBackBuffer()->render(getHDC(), 0, 0);
}

void gameStudy::mouseFunc(void)
{
	// �Ե� ��� ���� Ʈ�簡 �Ǹ�, ���°� �ٲ��.
	// �Ե� �����ʾҴٸ�,
	if (!mouseGun)
	{
		arrowState = arrowIDLE;
		arrowCurrentY = 0;
	}
	//�Ե� ��Ҵٸ�
	if (mouseGun)
	{
		arrowState = arrowGUN;
		arrowCurrentY = 2;
	}
	if (GeddyThrow)
	{
		arrowState = arrowTHROW;
		arrowCurrentY = 1;
	}

	switch (arrowState)
	{
	case arrowIDLE:
		arrowCurrentX = 0;
		break;
	case arrowGUN:
		arrowFrameCount++;
		if (arrowFrameCount > 2)
		{
			arrowFrameCount = 0;
			arrowCurrentX++;
			if (arrowCurrentX > 5)
			{
				arrowCurrentX = 0;
			}
		}
		break;
	case arrowTHROW:
		arrowFrameCount++;
		if (arrowFrameCount > 6)
		{
			arrowFrameCount = 0;
			arrowCurrentX++;
			if (arrowCurrentX > 5)
			{
				arrowCurrentX = 0;
			}
		}
		break;
	default:
		break;
	}

}
