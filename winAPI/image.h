#pragma once

#include "animation.h"
//=================================================
//======== 20170309 #�̹��� Ŭ����# ========
//=================================================

class image
{
private:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0,	//���ҽ��� �ε��Ѵ�
		LOAD_FILE,			//���Ϸκ��� �ε��Ѵ�
		LOAD_EMPTY,			//�׳� �ƹ��͵� ���°� ����.	
		LOAD_END
	};

	typedef struct tagImage
	{
		DWORD		resID;
		HDC			hMemDC;
		HBITMAP		hBit;
		HBITMAP		hOBit;
		float		x;
		float		y;
		int			width;
		int			height;
		int			currentFrameX;
		int			currentFrameY;
		int			maxFrameX;
		int			maxFrameY;
		int			frameWidth;
		int			frameHeight;
		BYTE		loadType;
		RECT		boundingBox;

		tagImage() //����ü �ʱ�ȭ
		{
			resID			= 0;
			hMemDC			= NULL;
			hBit			= NULL;			
			hOBit			= NULL;
			x				= 0;
			y				= 0;
			width			= 0;
			height			= 0;
			currentFrameX	= 0;
			currentFrameY	= 0;
			maxFrameX		= 0;
			maxFrameY		= 0;
			frameWidth		= 0;
			frameHeight		= 0;
			loadType		= LOAD_RESOURCE;
			boundingBox		= RectMake(0, 0, 0, 0);
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//�̹��� ����
	CHAR*			_fileName;		//�̹��� ��� ��

	BOOL			_tran;			//���� ����� �ֳ�?
	COLORREF		_transColor;	//���� �÷�Ű~~!!

	BLENDFUNCTION	_blendFunc;		//���ĺ������� ���� ����
	LPIMAGE_INFO	_blendImage;	//���ĺ����带 ����ϱ� ���� �̹��� ����

public:
	//�� ��Ʈ�� �����!
	HRESULT init(int width, int height);

	//���Ϸκ��� �о����!
	HRESULT init(const char* fileName, int width,
		int height, BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	HRESULT init(const char* fileName, float x, float y, int width,
		int height, BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//�̹��� + ������
	HRESULT init(const char* fileName, float x, float y,
		int width, int height, int frameX, int frameY,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//���� Ű ����
	void setTransColor(BOOL trans, COLORREF transColor);

	//�Ϲ� ����
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//������ ����
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//���ķ���
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	//��������
	void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

	//�ִϷ���
	void aniRender(HDC hdc, int destX, int destY, animation* ani);

	//����
	void release(void);

	//DC ���´�
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

	//��ǥ x
	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX(void) { return _imageInfo->x; }

	//��ǥ y
	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY(void) { return _imageInfo->y; }

	//��ǥ x, y
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//�����ػ�
	inline int getWidth(void) { return _imageInfo->width; }

	//�����ػ�
	inline int getHeight(void) { return _imageInfo->height; }

	//�ٿ���ڽ�
	inline RECT boundingBox(void)
	{
		RECT rc = { (int)(_imageInfo->x - (_imageInfo->width / 2)),
			(int)(_imageInfo->y - (_imageInfo->height / 2)),
			(int)(_imageInfo->x + (_imageInfo->width / 2)),
			(int)(_imageInfo->y + (_imageInfo->height / 2)) };

		return rc;
	}

	//������
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

	image();
	~image();
};
