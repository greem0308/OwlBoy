#include "StdAfx.h"
#include "collisionTestScene.h"

collisionTestScene::collisionTestScene(void)
{
}

collisionTestScene::~collisionTestScene(void)
{
}

HRESULT collisionTestScene::init(void)
{
	gameNode::init();

	IMAGEMANAGER->addImage("bg", "vellie/velli.bmp", WINSIZEX, WINSIZEY);

	_text = "";

	/*
	//Áß¾Ó °íÁ¤ ¿ø...
	_cir.x = WINSIZEX / 2;
	_cir.y = WINSIZEY / 2;
	_cir.r = 100;

	_cirMove.x = 0;
	_cirMove.y = WINSIZEY;
	_cirMove.r = 100;
	*/

	_rcMove._left = 100;
	_rcMove._top = 100;
	_rcMove._right = 200;
	_rcMove._bottom = 200;

	_rc._left = WINSIZEX / 2 - 150;
	_rc._top = WINSIZEY / 2 - 150;
	_rc._right = WINSIZEX / 2 + 150;
	_rc._bottom = WINSIZEY / 2 + 150;

	return S_OK;
}

void collisionTestScene::release(void)
{
	gameNode::release();
}

void collisionTestScene::update(void)
{
	gameNode::update();

//	_cirMove.x = _ptMouse.x;
//	_cirMove.y = _ptMouse.y;

	_rcMove.setCenterPos(_ptMouse.x, _ptMouse.y);

	/*
	if (isCollision(_cir, _cirMove))
	{
		_text = "±¤³âÀÌ";
	}
	else _text = "";*/

	/*
	if (isCollision_notinter(_cir, _cirMove))
	{
		_text = "±¤³âÀÌ";
	}
	else _text = "";
	*/

	if (isCollision_notinter(_rc, _rcMove))
	{
		_text = "±¤³âÀÌ";
	}
	else _text = "";
}

void collisionTestScene::render(void)
{
	IMAGEMANAGER->findImage("bg")->render(getMemDC());

//	EllipseMakeCenter(getMemDC(), _cir.x, 
//		_cir.y, _cir.r * 2, _cir.r * 2);
//	EllipseMakeCenter(getMemDC(), _cirMove.x, 
//		_cirMove.y, _cirMove.r * 2, _cirMove.r * 2);

	_rcMove.render(getMemDC());
	_rc.render(getMemDC());

	TextOut(getMemDC(), WINSIZEX - 100, 20, _text.c_str(), _text.length());
}
