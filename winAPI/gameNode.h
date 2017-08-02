#pragma once

#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backImage", 
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 255,0,255 아래쪽 
static image* _backPixel = IMAGEMANAGER->addImage("backPixel",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 0,0,255 왼쪽
static image* _backPixelBlue = IMAGEMANAGER->addImage("backPixelBlue",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 0,255,0 오른쪽
static image* _backPixelGreen = IMAGEMANAGER->addImage("backPixelGreen",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 255,255,0 위쪽
static image* _backPixelYellow = IMAGEMANAGER->addImage("backPixelYellow",
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
	
	// 핑크
	image* getBackPixel(void) { return _backPixel; }
	HDC getPixel() { return _backPixel->getMemDC(); }

	// 파란색. 
	image* getBackPixelBlue(void) { return _backPixelBlue; }
	HDC getPixelBlue() { return _backPixelBlue->getMemDC(); }

	// 초록색. 
	image* getBackPixelGreen(void) { return _backPixelGreen; }
	HDC getPixelGreen() { return _backPixelGreen->getMemDC(); }

	// 노랑색. 
	image* getBackPixelYellow(void) { return _backPixelYellow; }
	HDC getPixelYellow() { return _backPixelYellow->getMemDC(); }

	HDC getHDC() {return _hdc;}

	//메시지 프로시져
	LRESULT MainProc(HWND hWnd, UINT iMessage, 
		WPARAM wParam, LPARAM lParam);

	gameNode(void);
	virtual ~gameNode(void);
};

