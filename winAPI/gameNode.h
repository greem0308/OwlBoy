#pragma once

#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backImage", 
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 255,0,255 �Ʒ��� 
static image* _backPixel = IMAGEMANAGER->addImage("backPixel",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 0,0,255 ����
static image* _backPixelBlue = IMAGEMANAGER->addImage("backPixelBlue",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 0,255,0 ������
static image* _backPixelGreen = IMAGEMANAGER->addImage("backPixelGreen",
	"backImage.bmp", WINSIZEX, WINSIZEY);

// 255,255,0 ����
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
	
	// ��ũ
	image* getBackPixel(void) { return _backPixel; }
	HDC getPixel() { return _backPixel->getMemDC(); }

	// �Ķ���. 
	image* getBackPixelBlue(void) { return _backPixelBlue; }
	HDC getPixelBlue() { return _backPixelBlue->getMemDC(); }

	// �ʷϻ�. 
	image* getBackPixelGreen(void) { return _backPixelGreen; }
	HDC getPixelGreen() { return _backPixelGreen->getMemDC(); }

	// �����. 
	image* getBackPixelYellow(void) { return _backPixelYellow; }
	HDC getPixelYellow() { return _backPixelYellow->getMemDC(); }

	HDC getHDC() {return _hdc;}

	//�޽��� ���ν���
	LRESULT MainProc(HWND hWnd, UINT iMessage, 
		WPARAM wParam, LPARAM lParam);

	gameNode(void);
	virtual ~gameNode(void);
};

