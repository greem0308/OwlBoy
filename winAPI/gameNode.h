#pragma once

#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backImage", 
	"backImage.bmp", WINSIZEX, WINSIZEY);

static image* _backPixel = IMAGEMANAGER->addImage("backPixel",
	"backImage.bmp", WINSIZEX, WINSIZEY);

static image* _backPixelBlue = IMAGEMANAGER->addImage("backPixelBlue",
	"backImage.bmp", WINSIZEX, WINSIZEY);

#define MapSpeed 3.0f;
class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	image* getBackBuffer(void) {return _backBuffer;}
	HDC getMemDC() {return _backBuffer->getMemDC();}
	image* getBackPixel(void) { return _backPixel; }
	HDC getPixel() { return _backPixel->getMemDC(); }

	// �Ķ���. �̰� �ε����� �׳� �������ϴ� DC(��ȭ��)
	image* getBackPixelBlue(void) { return _backPixelBlue; }
	HDC getPixelBlue() { return _backPixelBlue->getMemDC(); }

	HDC getHDC() {return _hdc;}

	//�޽��� ���ν���
	LRESULT MainProc(HWND hWnd, UINT iMessage, 
		WPARAM wParam, LPARAM lParam);

	gameNode(void);
	virtual ~gameNode(void);
};

