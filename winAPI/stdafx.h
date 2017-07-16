// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

//================================================
//####### 2017.03.20 헤더파일 인끌루드 #############
//================================================
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN //MFC가 아닌 응용 프로그램의
//경우에는 WIN32_LEAN_AND_MEAN를 정의해 빌드 시간을 단축시킨다.

#include <windows.h> //윈도우 헤더 인클루드

#include <stdio.h> //스탠다드 입출력 헤더파일 인클루드
//printf, scanf, puts, gets.. 등등
#include <tchar.h> //윈도우에서 사용할 문자열 출력 헤더 인클루드
//TextOut, DrawText, wsprintf..등등..

//프로젝트는 기본적으로 유니코드임. 영어, 한글 모두 2바이트...
//MBCS (Multi Byte Character Set) -> 
//기존에 사용하던 멀티바이트 형태의 문자열임
//WBCS (Wide Byte Character Set) ->
//모든 문자열을 2바이트로 처리한다.. 유니코드 기반..

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "txtDataManager.h"
#include "timeManager.h"
#include "sceneManager.h"
#include "database.h"
#include "utils.h"
#include "collision.h"
#include "frameAnimationManager.h"
#include "soundManager.h"
#include "keyAnimationManager.h"

//================================================
//############### 2017.03.20 디파인 ###############
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
//############### 2017.03.20 전역변수 ###############
//================================================
extern HINSTANCE	_hInstance; //프로그램 인스턴스
extern HWND			_hWnd;		//윈도우 핸들
extern POINT		_ptMouse;	//마우스
extern bool			_leftButtonDown;
extern bool         _soundOpen; // 사운드 데이터베이스에 넣고 콜백으로 하려다가 빡쳐서 그냥 이걸로함. 나중에 수정할시간이 있었으면 좋겠다.

#define degreeToHodo(a) (((a)*(3.141592))/(180))
#define hodoToToDegree(a) (((180)*(a))/(3.141592))


struct velliToDoorPos
{
	float x, y;
};
// 다른씬에서 마을로 넘어올때 받는 선택하는 좌표값.
extern velliToDoorPos DoorPos[7];

// 다른씬에서 마을로 넘어올때 받는 번호값. 이 번호가 위 구조체의 번호로 들어감.
// init에선 0. 0은 마을로 돌아올때 초기화시키려는 것. 
extern int toVellieDoor;

// 업데이트에서 플레이어 좌표값을 위의 번호에 맞는 구조체 좌표로 넣어준다음에 시작하게할 불 
extern int startDoor;

// 시작 불을 트루시키기위해 있는 장치.. 
extern int doorFrame;

//아이템 카운트. 어느아이템을 살거냐. 
extern int itemCount;

// 마우스 게디잡을때 화살표 바뀌는 것 구현. 
extern bool mouseGun;
extern bool GeddyThrow;

// 이벤트에서 다시 마을 왔을때 던전 표시 
extern bool goDungeon;


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
