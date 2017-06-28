#include "StdAfx.h"
#include "gameNode.h"

gameNode::gameNode(void)
	: _managerInit(FALSE)
{
}

gameNode::~gameNode(void)
{
}

HRESULT gameNode::init(void) //init, setup, create...
{
	_hdc = GetDC(_hWnd);
	return S_OK;
}

HRESULT gameNode::init(bool mangerInit) //init, setup, create...
{
	_hdc = GetDC(_hWnd);
	_managerInit = mangerInit;

	if (_managerInit)
	{
		KEYMANAGER->init();			//Ű �Ŵ��� ����
		IMAGEMANAGER->init();		//�̹��� �Ŵ��� ����
		TIMEMANAGER->init();		//Ÿ�ӸŴ��� ����
		TXTMANAGER->init();			//txt�Ŵ��� ����
		SCENEMANAGER->init();		//���Ŵ��� ����
		DATABASE->init();			//�����ͺ��̽� ����
		FRAMEANIMANAGER->init();	//�����Ӿִ� �޴��� ����
		SOUNDMANAGER->init();		//����Ŵ��� ����
		KEYANIMANAGER->init();		//Ű�ִ� �Ŵ��� ����
	}

	return S_OK;
}

void gameNode::release(void)
{
	if (_managerInit)
	{
		//���� �̱��� ����
		RND->releaseSingleton();

		//Ű �Ŵ��� �̱��� ����
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();

		//�̹��� �Ŵ��� �̱��� ����
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();

		//Ÿ�� �Ŵ��� �̱��� ����
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();

		//txt�Ŵ��� �̱��� ����
		TXTMANAGER->release();
		TXTMANAGER->releaseSingleton();

		//�� �Ŵ��� �̱��� ����
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();

		//db �̱��� ����
		DATABASE->release();
		DATABASE->releaseSingleton();

		//�����Ӿִ� �Ŵ��� ����
		FRAMEANIMANAGER->release();
		FRAMEANIMANAGER->releaseSingleton();

		//����Ŵ��� ����
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();

		//Ű�ִϸŴ��� ����
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();
	}

	ReleaseDC(_hWnd, _hdc);
}

//������Ʈ (����� ���� ���� ��ġ �ݿ�...) == WM_TIMER
void gameNode::update(void)
{
}

//���� (ȭ�� ���� �� ��� ����Ѵ�) == WM_PAINT
void gameNode::render(void)
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, 
		WPARAM wParam, LPARAM lParam)
{
	switch ( iMessage )
	{
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>(LOWORD(lParam));
			_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;
		case WM_LBUTTONDOWN:
			_leftButtonDown = true;
		break;
		case WM_LBUTTONUP:
			_leftButtonDown = false;
		break;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				//ESCŰ.....
				case VK_ESCAPE:
					PostMessage( hWnd, WM_DESTROY, 0, 0 );
				break;
			}
		break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
	}

	//������ ���ν������� ó������ ���� ������ �޽��� ó���Ѵ�.
	return ( DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}
