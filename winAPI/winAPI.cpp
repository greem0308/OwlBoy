#include "stdafx.h"
#include "gameStudy.h"

//================================================
//####### 2017.03.10 �������� #############
//================================================
HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse;
bool _leftButtonDown;
bool _soundOpen;

// �ٸ������� ������ �Ѿ�ö� �޴� �����ϴ� ��ǥ��.
velliToDoorPos DoorPos[7];

int toVellieDoor;
int startDoor;
int doorFrame;
int itemCount;
bool mouseGun;
bool GeddyThrow;
bool goDungeon;

gameStudy _gs;

//================================================
//####### 2017.03.10 �Լ� ������Ÿ�� #############
//================================================
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void setWindowSize( int x, int y, int width, int height );
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow )
{
	MSG message;
	//������ ������ �����ϱ� ���� ����ü ����
	WNDCLASS wndClass;

	_hInstance = hInstance;
	wndClass.cbClsExtra = 0; //Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0; //������ ���� �޸�
							 //��׶��� ���� ����
	wndClass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC) WndProc; //������ ���ν���
	wndClass.lpszClassName = WINNAME; //Ŭ�����̸�
	wndClass.lpszMenuName = NULL; //�޴��̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	//������ ����Ѵ�.
	RegisterClass( &wndClass );

	//������ �����Ѵ�.
	_hWnd = CreateWindow(
		WINNAME, //������ Ŭ������ �̸�
		WINNAME, //������ Ÿ��Ʋ �� �̸�
		WINSTYLE, //������ ��Ÿ��~~
		WINSTARTX, //������ ȭ�� ��ǥ x
		WINSTARTY, //������ ȭ�� ��ǥ y
		WINSIZEX, //������ ȭ�� ��ǥ width
		WINSIZEY, //������ ȭ�� ��ǥ height
		NULL, //�θ�������
		(HMENU) NULL, //�޴��ڵ�
		hInstance, //�ν��Ͻ� ������ ����
		NULL //������ �� �ڽ� �����츦 �����ϸ� �������ֵ� �������
			 //������... NULL....................
	);

	setWindowSize( WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY );

	//ȭ�鿡 ������ �����ش�
	ShowWindow( _hWnd, cmdShow );

	//���� ���͵� �ʱ�ȭ
	if (FAILED(_gs.init()))
	{
		//�����ϸ� �ٷ� �����Ѵ�
		return 0;
	}
	
	
	//���ӿ�...
	while (true)
	{
		if (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{	
			TIMEMANAGER->update(60.0f); //�ʴ� 60���� ����
			_gs.update();
			_gs.render();
		}
	}

	/* �߰��� ���¾�~!!
	//�Ϲ����α׷��ֿ�..
	while ( GetMessage( &message, 0, 0, 0 ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}*/

	//���� ���͵� ����
	_gs.release();

	//��ϵ� ������ Ŭ���� ����.....
	UnregisterClass( WINNAME, hInstance );

	return message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return _gs.MainProc(hWnd, iMessage, wParam, lParam);
}

//������ ������ �缳��
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = {0, 0, width, height};
	//POINT pt;

	//������ �缳�� �Լ�
	AdjustWindowRect(&rc, WINSTYLE, false); //false�� �޴��� �ִ��� ������...
	SetWindowPos( _hWnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE );
}



