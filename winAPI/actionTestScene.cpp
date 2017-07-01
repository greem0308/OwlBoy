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
	//숙제
	//1. 액숀함수 3개 만들것..
	//2. 액숀함수 1개는 지정 (베이지어 곡선 만들것...)
	//3. 콜백 추가할것.
	//4. 액숀매니져 만들기....
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
			// 선생님이 만드신 함수
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f);
		}

		if (KEYMANAGER->isOnceKeyDown('2'))
		{
			//콜백
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f, callback);
		}

		if (KEYMANAGER->isOnceKeyDown('3'))
		{
			//처음으로 돌아감.
			ACTMANAGER->moveTo(_image, _ptMouse.x, _ptMouse.y, 3.0f, true);
		}

		if (KEYMANAGER->isOnceKeyDown('4'))
		{
			// 끝점만 찍으면 다 계산되서 베지어 그려줌.
			ACTMANAGER->moveToBezier(_image, _ptMouse.x, _ptMouse.y, 3.0f);
		}

		if (KEYMANAGER->isOnceKeyDown('5'))
		{
			// 중간 두점을 인자로 줄수 있음.
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

// moveTo()함수가 끝나면 실행됨.
void actionTestScene::callback(void)
{
	MessageBox(_hWnd, "매니저 만듬", " 매니저 만듬! ", MB_OK);
}