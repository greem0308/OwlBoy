#include "StdAfx.h"
#include "actionTestScene.h"

actionTestScene::actionTestScene(void)
{
}

actionTestScene::~actionTestScene(void)
{
}

HRESULT actionTestScene::init(void)
{
	//����
	//1. �׼��Լ� 3�� �����..
	//2. �׼��Լ� 1���� ���� (�������� � �����...)
	//3. �ݹ� �߰��Ұ�.
	//4. �׼�Ŵ��� �����....
	//5. 

	gameNode::init();

	IMAGEMANAGER->addImage("space", "space.bmp", WINSIZEX, WINSIZEY);

	_image = IMAGEMANAGER->addImage("rocket", "rocket.bmp", 52, 64, true);
	_image->setX(WINSIZEX/2);
	_image->setY(WINSIZEY/2);

	_action1 = new action;
	_action1->init();
	
	//_action1->moveTo(_image, 500, 100, 5.2f);

	return S_OK;
}

void actionTestScene::release(void)
{
	gameNode::release();

	_action1->release();
 	SAFE_DELETE(_action1);
}

void actionTestScene::update(void)
{
	gameNode::update();

	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	//{	
		if (KEYMANAGER->isOnceKeyDown('1'))
		{
			// �������� ����� �Լ�
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f);
		}

		if (KEYMANAGER->isOnceKeyDown('2'))
		{
			//�ݹ�
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f, callback);
		}

		if (KEYMANAGER->isOnceKeyDown('3'))
		{
			//ó������ ���ư�.
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f, true);
		}

		if (KEYMANAGER->isOnceKeyDown('4'))
		{
			// ������ ������ �� ���Ǽ� ������ �׷���.
			ACTMANAGER->moveToBezier(_image, _ptMouse.x, _ptMouse.y, 3.0f);
		}

		if (KEYMANAGER->isOnceKeyDown('5'))
		{
			// �߰� ������ ���ڷ� �ټ� ����.
			ACTMANAGER->moveToBezier(_image, WINSIZEX / 2 + 200, WINSIZEY / 2 + 200, WINSIZEX / 2 - 200, WINSIZEY / 2 - 200,_ptMouse.x,_ptMouse.y,3.0f);
		}
	//}

	//_action1->update();
	ACTMANAGER->update();
}

void actionTestScene::render(void)
{
	IMAGEMANAGER->findImage("space")->render(getMemDC());

	_image->render(getMemDC());
}

// moveTo()�Լ��� ������ �����.
void actionTestScene::callback(void)
{
	MessageBox(_hWnd, "�Ŵ��� ����", " �Ŵ��� ����! ", MB_OK);
}