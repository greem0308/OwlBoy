#pragma once

#include "animation.h"
//=================================================
//======== 20170309 #이미지 클래스# ========
//=================================================

class image
{
private:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0,	//리소스로 로딩한다
		LOAD_FILE,			//파일로부터 로딩한다
		LOAD_EMPTY,			//그냥 아무것도 없는거 생성.	
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

		tagImage() //구조체 초기화
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
	LPIMAGE_INFO	_imageInfo;		//이미지 정보
	CHAR*			_fileName;		//이미지 경로 명

	BOOL			_tran;			//투명 배경이 있냐?
	COLORREF		_transColor;	//투명 컬러키~~!!

	BLENDFUNCTION	_blendFunc;		//알파블렌딩을 위한 정보
	LPIMAGE_INFO	_blendImage;	//알파블렌드를 사용하기 위한 이미지 정보

public:
	//빈 비트맵 만든다!
	HRESULT init(int width, int height);

	//파일로부터 읽어오기!
	HRESULT init(const char* fileName, int width,
		int height, BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	HRESULT init(const char* fileName, float x, float y, int width,
		int height, BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//이미지 + 프레임
	HRESULT init(const char* fileName, float x, float y,
		int width, int height, int frameX, int frameY,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//투명 키 셋팅
	void setTransColor(BOOL trans, COLORREF transColor);

	//일반 렌더
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//프레임 렌더
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//알파렌더
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	//루프렌더
	void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

	//애니렌더
	void aniRender(HDC hdc, int destX, int destY, animation* ani);

	//삭제
	void release(void);

	//DC 얻어온다
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

	//좌표 x
	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX(void) { return _imageInfo->x; }

	//좌표 y
	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY(void) { return _imageInfo->y; }

	//좌표 x, y
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//가로해상도
	inline int getWidth(void) { return _imageInfo->width; }

	//세로해상도
	inline int getHeight(void) { return _imageInfo->height; }

	//바운딩박스
	inline RECT boundingBox(void)
	{
		RECT rc = { (int)(_imageInfo->x - (_imageInfo->width / 2)),
			(int)(_imageInfo->y - (_imageInfo->height / 2)),
			(int)(_imageInfo->x + (_imageInfo->width / 2)),
			(int)(_imageInfo->y + (_imageInfo->height / 2)) };

		return rc;
	}

	//프레임
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

