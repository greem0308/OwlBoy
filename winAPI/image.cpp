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

//�� ��Ʈ�� �����!
HRESULT image::init(int width, int height)
{
	//���ʱ�ȭ�� ����ؼ�~~
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo				= new IMAGE_INFO;
	_imageInfo->loadType	= LOAD_EMPTY;
	_imageInfo->resID		= 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //��Ʈ�� �̹��� �����
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_imageInfo->width = width;
	_imageInfo->height = height;
	
	_fileName = NULL;

	//�����÷� �����Ѵ�
	_tran = false;
	_transColor = RGB(0, 0, 0);

	//���ĺ��� �ɼ�
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//���ĺ��� ����� ���� �̹��� ����
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//���� DC �����Ѵ�
	ReleaseDC(_hWnd, hdc);

	//�̹��� ����µ� �����ϸ�...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//���Ϸκ��� �о����!
HRESULT image::init(const char* fileName, int width,
	int height, BOOL trans, COLORREF transColor)

{
	//���� ��ΰ� �߸� �Ǿ�����...
	if (fileName == NULL) return E_FAIL;

	//���ʱ�ȭ�� ����ؼ�~~
	if (_imageInfo != NULL) release();

	//DC��´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� ��� �̸� �����Ѵ�
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//�����÷� �����Ѵ�
	_tran = trans;
	_transColor = transColor;

	//���ĺ��� �ɼ�
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//���ĺ��� ����� ���� �̹��� ����
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	
	//���� DC ����
	ReleaseDC(_hWnd, hdc);

	//�̹��� ����µ� �����ϸ�...
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
	//���� ��ΰ� �߸� �Ǿ�����...
	if (fileName == NULL) return E_FAIL;

	//���ʱ�ȭ�� ����ؼ�~~
	if (_imageInfo != NULL) release();

	//DC��´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� ��� �̸� �����Ѵ�
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//�����÷� �����Ѵ�
	_tran = trans;
	_transColor = transColor;

	//���� ���� �ɼ�
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//���ĺ��� ����� ���� �̹��� ����
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//���� DC ����
	ReleaseDC(_hWnd, hdc);

	//�̹��� ����µ� �����ϸ�...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//�̹��� + ������
HRESULT image::init(const char* fileName, float x, float y,
	int width, int height, int frameX, int frameY,
	BOOL trans, COLORREF transColor)
{
	//���� ��ΰ� �߸� �Ǿ�����...
	if (fileName == NULL) return E_FAIL;

	//���ʱ�ȭ�� ����ؼ�~~
	if (_imageInfo != NULL) release();

	//DC��´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//�� �̹����� ����ϰڽ��ϴ�...
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

	//���� ��� �̸� �����Ѵ�
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//�����÷� �����Ѵ�
	_tran = trans;
	_transColor = transColor;

	//���� ���� �ɼ�
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//���ĺ��� ����� ���� �̹��� ����
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//���� DC ����
	ReleaseDC(_hWnd, hdc);

	//�̹��� ����µ� �����ϸ�...
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
	//���� ��ΰ� �߸� �Ǿ�����...
	if (fileName == NULL) return E_FAIL;

	//���ʱ�ȭ�� ����ؼ�~~
	if (_imageInfo != NULL) release();

	//DC��´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//�� �̹����� ����ϰڽ��ϴ�...
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

	//���� ��� �̸� �����Ѵ�
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//�����÷� �����Ѵ�
	_tran = trans;
	_transColor = transColor;

	//���� ���� �ɼ�
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	//���ĺ��� ����� ���� �̹��� ����
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);//��Ʈ���� ����ϱ� ���ؼ� �޸� DC�� ������ش� (ȭ��DC�� ������ �ȵ�!)
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);//�� �̹����� ����ϰڽ��ϴ�...
	_blendImage->x = 0;
	_blendImage->y = 0;
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//���� DC ����
	ReleaseDC(_hWnd, hdc);

	//�̹��� ����µ� �����ϸ�...
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;

}

//����
void image::release(void)
{
	if (_imageInfo != NULL)
	{
		//�̹��� �����Ѵ�
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//���ĺ��� ��� �̹��� �����Ѵ�
		SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		//������ ����
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(_blendImage);

		//���� �÷� �ʱ�ȭ
		_tran = FALSE;
		_transColor = RGB(0, 0, 0);
	}
}

//���� Ű ����
void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_tran = trans;
	_transColor = transColor;
}

//�븻 ����
void image::render(HDC hdc)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //������ ����� DC
			(int)_imageInfo->x,		//����� ��ǥ ������ X
			(int)_imageInfo->y,		//����� ��ǥ ������ Y
			_imageInfo->width, //����� ũ�� width
			_imageInfo->height,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			0, 0,				//���� ��������
			_imageInfo->width,	//����� ��������width
			_imageInfo->height, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		//BitBlt�� DC���� �������� ���� ��Ӻ��縦 ���ش�!!
		//(StrechBlt ��Ʈ���� Ȯ�� ��� �����ϴ�!!)
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			_imageInfo->width, //����� ũ�� width
			_imageInfo->height,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			0, 0,				//���� ��������
			_imageInfo->width,	//����� ��������width
			_imageInfo->height, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		//BitBlt�� DC���� �������� ���� ��Ӻ��縦 ���ش�!!
		//(StrechBlt ��Ʈ���� Ȯ�� ��� �����ϴ�!!)
		BitBlt(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //������ ����� DC
			(int)_imageInfo->x,		//����� ��ǥ ������ X
			(int)_imageInfo->y,		//����� ��ǥ ������ Y
			sourWidth, //����� ũ�� width
			sourHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			sourX, sourY,				//���� ��������
			sourWidth,	//����� ��������width
			sourHeight, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		//BitBlt�� DC���� �������� ���� ��Ӻ��縦 ���ش�!!
		//(StrechBlt ��Ʈ���� Ȯ�� ��� �����ϴ�!!)
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, sourWidth,
			sourHeight, _imageInfo->hMemDC,
			sourX, sourY, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			sourWidth, //����� ũ�� width
			sourHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			sourX, sourY,				//���� ��������
			sourWidth,	//����� ��������width
			sourHeight, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		//BitBlt�� DC���� �������� ���� ��Ӻ��縦 ���ش�!!
		//(StrechBlt ��Ʈ���� Ȯ�� ��� �����ϴ�!!)
		BitBlt(hdc, destX, destY, sourWidth,
			sourHeight, _imageInfo->hMemDC,
			sourX, sourY, SRCCOPY);
	}
}

//������ ����
void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		GdiTransparentBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			_imageInfo->frameWidth, //����� ũ�� width
			_imageInfo->frameHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,				//���� ��������
			_imageInfo->frameWidth,	//����� ��������width
			_imageInfo->frameHeight, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		BitBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			_imageInfo->frameWidth, //����� ũ�� width
			_imageInfo->frameHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,				//���� ��������
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
		GdiTransparentBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			_imageInfo->frameWidth, //����� ũ�� width
			_imageInfo->frameHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,				//���� ��������
			_imageInfo->frameWidth,	//����� ��������width
			_imageInfo->frameHeight, //����� ��������height
			_transColor			//���翡�� ������ ���� �����Ѵ�
			);
	}
	else
	{
		BitBlt(hdc, //������ ����� DC
			destX,		//����� ��ǥ ������ X
			destY,		//����� ��ǥ ������ Y
			_imageInfo->frameWidth, //����� ũ�� width
			_imageInfo->frameHeight,//����� ũ�� height
			_imageInfo->hMemDC, //������ DC
			currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,				//���� ��������
			SRCCOPY
			);
	}
}

//���ķ���
void image::alphaRender(HDC hdc, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_tran)
	{
		//����ؾ� �� DC�� �׷��� �ִ� ������ blend�� �׸���
		BitBlt(_blendImage->hMemDC, 0,	0, 
			_imageInfo->width, _imageInfo->height,
			hdc, (int)_imageInfo->x, (int)_imageInfo->y, SRCCOPY);

		//����ؾ� �� �̹����� blend�� �׸���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//blendDC�� ����ؾ� �� DC�� �׸���
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


//��׶��� ����!!
void image::loopRender(HDC hdc, const LPRECT drawArea,
	int offsetX, int offsetY)
{
	//�ɼ� ���� ������ ��쿡 ����
	if (offsetX < 0) offsetX = _imageInfo->width +
		(offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height +
		(offsetY % _imageInfo->height);

	//�׸��� �ϴ� �ҽ��� ������.
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷����� DC ����
	RECT rcDest;

	//�׸��� �ϴ� ��ü ����
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//���� ����
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//�ҽ� ���� ���� ����Ѵ�!
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//�ҽ� ������ �׸��� ������ �Ѿ����...
		if (y + sourHeight > drawAreaH)
		{
			//�Ѿ �縸ŭ ���Ұ� �ø���
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�׷����� ������
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//���� ����
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//�ҽ� ���� ���� ����Ѵ�!
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//�ҽ� ������ �׸��� ������ �Ѿ����...
			if (x + sourWidth > drawAreaW)
			{
				//�Ѿ �縸ŭ ����Ʈ���� ��������~~
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//�׷����� ������
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//�׸���
			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top, rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}

	}

}

//�ִϷ���
void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y,
		ani->getFrameWidth(), ani->getFrameHeight());
}

