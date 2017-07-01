// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

//================================================
//####### 2017.03.20 ������� �β���� #############
//================================================
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN //MFC�� �ƴ� ���� ���α׷���
//��쿡�� WIN32_LEAN_AND_MEAN�� ������ ���� �ð��� �����Ų��.

#include <windows.h> //������ ��� ��Ŭ���

#include <stdio.h> //���Ĵٵ� ����� ������� ��Ŭ���
//printf, scanf, puts, gets.. ���
#include <tchar.h> //�����쿡�� ����� ���ڿ� ��� ��� ��Ŭ���
//TextOut, DrawText, wsprintf..���..

//������Ʈ�� �⺻������ �����ڵ���. ����, �ѱ� ��� 2����Ʈ...
//MBCS (Multi Byte Character Set) -> 
//������ ����ϴ� ��Ƽ����Ʈ ������ ���ڿ���
//WBCS (Wide Byte Character Set) ->
//��� ���ڿ��� 2����Ʈ�� ó���Ѵ�.. �����ڵ� ���..

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "txtDataManager.h"
#include "timeManager.h"
#include "sceneManager.h"
#include "database.h"
#include "utils.h"
#include "frameAnimationManager.h"
#include "soundManager.h"
#include "keyAnimationManager.h"

//================================================
//############### 2017.03.20 ������ ###############
//================================================

using namespace MY_UTIL;

#define WINNAME (LPSTR)(TEXT("APIWindow"))
#define WINSTARTX 0
#define WINSTARTY 0
#define WINSIZEX 1280
#define WINSIZEY 720
#define WINCENTER (PointMake(WINSIZEX / 2, WINSIZEY / 2))
#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define SAFE_DELETE(p)			{ if(p) {delete		(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{ if(p) {delete[]	(p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{ if(p) {(p)->release(); (p) = NULL;}}

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define TXTMANAGER txtDataManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define DATABASE database::getSingleton()
#define FRAMEANIMANAGER frameAnimationManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define KEYANIMANAGER keyAnimationManager::getSingleton()

//================================================
//############### 2017.03.20 �������� ###############
//================================================
extern HINSTANCE	_hInstance; //���α׷� �ν��Ͻ�
extern HWND			_hWnd;		//������ �ڵ�
extern POINT		_ptMouse;	//���콺
extern bool			_leftButtonDown;


#define degreeToHodo(a) (((a)*(3.141592))/(180))
#define hodoToToDegree(a) (((180)*(a))/(3.141592))


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
