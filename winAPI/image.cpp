#include "stdafx.h"
#include "image.h"

#pragma comment(lib, "msimg32.lib")

image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_tran(FALSE),
	_transColor(RGB(0, 0, 0))
{
}

image::~image()
{
	release();
}

//빈 비트맵 만든다!
HRESULT image::init(int width, int height)
{
	//재초기화를 대비해서~~
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo				= new IMAGE_INFO;
	_imageInfo->loadType	= LOAD_EMPTY;
	_imageInfo->resID		= 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //비트맵 이미지 만든다
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//이 이미지를 사용하겠습니당...
	_imageInfo->width = width;
	_imageInfo->height = height;
	
	_fileName = NULL;

	//투명컬러 셋팅한다
	_tran = false;
	_transColor = RGB(0, 0, 0);

	//알파블렌드 옵션
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//알파블렌드 사용을 위한 이미지 설정
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//이 이미지를 사용하겠습니당...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//얻은 DC 해제한다
	ReleaseDC(_hWnd, hdc);

	//이미지 만드는데 실패하면...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//파일로부터 읽어오기!
HRESULT image::init(const char* fileName, int width,
	int height, BOOL trans, COLORREF transColor)

{
	//파일 경로가 잘못 되었으면...
	if (fileName == NULL) return E_FAIL;

	//재초기화를 대비해서~~
	if (_imageInfo != NULL) release();

	//DC얻는다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//이 이미지를 사용하겠습니당...
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//투명컬러 셋팅한다
	_tran = trans;
	_transColor = transColor;

	//알파블렌드 옵션
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//알파블렌드 사용을 위한 이미지 설정
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//이 이미지를 사용하겠습니당...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	
	//얻은 DC 해제
	ReleaseDC(_hWnd, hdc);

	//이미지 만드는데 실패하면...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width,
	int height, BOOL trans, COLORREF transColor)
{
	//파일 경로가 잘못 되었으면...
	if (fileName == NULL) return E_FAIL;

	//재초기화를 대비해서~~
	if (_imageInfo != NULL) release();

	//DC얻는다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//이 이미지를 사용하겠습니당...
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//투명컬러 셋팅한다
	_tran = trans;
	_transColor = transColor;

	//알파 블렌딩 옵션
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//알파블렌드 사용을 위한 이미지 설정
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//이 이미지를 사용하겠습니당...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//얻은 DC 해제
	ReleaseDC(_hWnd, hdc);

	//이미지 만드는데 실패하면...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//이미지 + 프레임
HRESULT image::init(const char* fileName, float x, float y,
	int width, int height, int frameX, int frameY,
	BOOL trans, COLORREF transColor)
{
	//파일 경로가 잘못 되었으면...
	if (fileName == NULL) return E_FAIL;

	//재초기화를 대비해서~~
	if (_imageInfo != NULL) release();

	//DC얻는다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//이 이미지를 사용하겠습니당...
	_imageInfo->x					= x - (width / 2);
	_imageInfo->y					= y - (height / 2);
	_imageInfo->width				= width;
	_imageInfo->height				= height;
	_imageInfo->currentFrameX		= 0;
	_imageInfo->currentFrameY		= 0;
	_imageInfo->maxFrameX			= frameX - 1;
	_imageInfo->maxFrameY			= frameY - 1;
	_imageInfo->frameWidth			= width / frameX;
	_imageInfo->frameHeight			= height / frameY;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//투명컬러 셋팅한다
	_tran = trans;
	_transColor = transColor;

	//알파 블렌딩 옵션
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//알파블렌드 사용을 위한 이미지 설정
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//이 이미지를 사용하겠습니당...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//얻은 DC 해제
	ReleaseDC(_hWnd, hdc);

	//이미지 만드는데 실패하면...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY,
	BOOL trans, COLORREF transColor)
{
	//파일 경로가 잘못 되었으면...
	if (fileName == NULL) return E_FAIL;

	//재초기화를 대비해서~~
	if (_imageInfo != NULL) release();

	//DC얻는다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//이 이미지를 사용하겠습니당...
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//투명컬러 셋팅한다
	_tran = trans;
	_transColor = transColor;

	//알파 블렌딩 옵션
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//알파블렌드 사용을 위한 이미지 설정
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//비트맵을 출력하기 위해서 메모리 DC를 만들어준다 (화면DC는 느려서 안됨!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//이 이미지를 사용하겠습니당...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//얻은 DC 해제
	ReleaseDC(_hWnd, hdc);

	//이미지 만드는데 실패하면...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;

}

//삭제
void image::release(void)
{
	if (_imageInfo != NULL)
	{
		//이미지 삭제한다
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//알파블렌드 사용 이미지 삭제한다
		SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		//포인터 삭제
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(_blendImage);

		//투명 컬러 초기화
		_tran = FALSE;
		_transColor = RGB(0, 0, 0);
	}
}

//투명 키 셋팅
void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_tran = trans;
	_transColor = transColor;
}

//노말 렌더
void image::render(HDC hdc)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			(int)_imageInfo->x,		//복사될 좌표 시작점 X
			(int)_imageInfo->y,		//복사될 좌표 시작점 Y
			_imageInfo->width, //복사될 크기 width
			_imageInfo->height,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			0, 0,				//복사 시작지점
			_imageInfo->width,	//복사될 영역지정width
			_imageInfo->height, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		//BitBlt는 DC간의 영역끼리 서로 고속복사를 해준다!!
		//(StrechBlt 비트맵의 확대 축소 가능하다!!)
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			_imageInfo->width, //복사될 크기 width
			_imageInfo->height,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			0, 0,				//복사 시작지점
			_imageInfo->width,	//복사될 영역지정width
			_imageInfo->height, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		//BitBlt는 DC간의 영역끼리 서로 고속복사를 해준다!!
		//(StrechBlt 비트맵의 확대 축소 가능하다!!)
		BitBlt(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			(int)_imageInfo->x,		//복사될 좌표 시작점 X
			(int)_imageInfo->y,		//복사될 좌표 시작점 Y
			sourWidth, //복사될 크기 width
			sourHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			sourX, sourY,				//복사 시작지점
			sourWidth,	//복사될 영역지정width
			sourHeight, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		//BitBlt는 DC간의 영역끼리 서로 고속복사를 해준다!!
		//(StrechBlt 비트맵의 확대 축소 가능하다!!)
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, sourWidth,
			sourHeight, _imageInfo->hMemDC,
			sourX, sourY, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			sourWidth, //복사될 크기 width
			sourHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			sourX, sourY,				//복사 시작지점
			sourWidth,	//복사될 영역지정width
			sourHeight, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		//BitBlt는 DC간의 영역끼리 서로 고속복사를 해준다!!
		//(StrechBlt 비트맵의 확대 축소 가능하다!!)
		BitBlt(hdc, destX, destY, sourWidth,
			sourHeight, _imageInfo->hMemDC,
			sourX, sourY, SRCCOPY);
	}
}

//프레임 렌더
void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			_imageInfo->frameWidth, //복사될 크기 width
			_imageInfo->frameHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,				//복사 시작지점
			_imageInfo->frameWidth,	//복사될 영역지정width
			_imageInfo->frameHeight, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		BitBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			_imageInfo->frameWidth, //복사될 크기 width
			_imageInfo->frameHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,				//복사 시작지점
			SRCCOPY
			);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}

	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	if (_tran)
	{
		GdiTransparentBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			_imageInfo->frameWidth, //복사될 크기 width
			_imageInfo->frameHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,				//복사 시작지점
			_imageInfo->frameWidth,	//복사될 영역지정width
			_imageInfo->frameHeight, //복사될 영역지정height
			_transColor			//복사에서 제외할 색상 지정한다
			);
	}
	else
	{
		BitBlt(hdc, //복사할 장소의 DC
			destX,		//복사될 좌표 시작점 X
			destY,		//복사될 좌표 시작점 Y
			_imageInfo->frameWidth, //복사될 크기 width
			_imageInfo->frameHeight,//복사될 크기 height
			_imageInfo->hMemDC, //복사대상 DC
			currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,				//복사 시작지점
			SRCCOPY
			);
	}
}

//알파렌더
void image::alphaRender(HDC hdc, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_tran)
	{
		//출력해야 될 DC에 그려져 있는 내용을 blend에 그린다
		BitBlt(_blendImage->hMemDC, 0,	0, 
			_imageInfo->width, _imageInfo->height,
			hdc, (int)_imageInfo->x, (int)_imageInfo->y, SRCCOPY);

		//출력해야 될 이미지를 blend에 그린다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//blendDC를 출력해야 될 DC에 그린다
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{

}

void image::alphaRender(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{

}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{

}


//백그라운드 루프!!
void image::loopRender(HDC hdc, const LPRECT drawArea,
	int offsetX, int offsetY)
{
	//옵셋 값이 음수인 경우에 보정
	if (offsetX < 0) offsetX = _imageInfo->width +
		(offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height +
		(offsetY % _imageInfo->height);

	//그리려 하는 소스의 영역임.
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//그려지는 DC 영역
	RECT rcDest;

	//그리려 하는 전체 영역
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//세로 루핑
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//소스 영역 높이 계산한다!
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//소스 영역이 그리는 영역을 넘어갔으면...
		if (y + sourHeight > drawAreaH)
		{
			//넘어간 양만큼 바텀값 올린다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//그려지는 영역임
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로 루핑
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//소스 영역 가로 계산한다!
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//소스 영역이 그리는 영역을 넘어갔으면...
			if (x + sourWidth > drawAreaW)
			{
				//넘어간 양만큼 라이트값을 왼쪽으로~~
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//그려지는 영역임
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//그린다
			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top, rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}

	}

}

//애니렌더
void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y,
		ani->getFrameWidth(), ani->getFrameHeight());
}

