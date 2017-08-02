#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"

class eventScene : public gameNode
{
public:
	player* _player;
	enemyManager* _em;
	int shipShowFrame; // 한번만 나타나게 하는 조건.

	//카메라에 연동시켜야하므로 x,y,가 필요함.
	struct structDoor
	{
		// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
		float x, y;
		RECT rc;
	};
	structDoor door[7];

	// 플레이어 위치에서 루프만 할 하늘배경
	image* skyBG; //기본 뒤에 파랑.
	image* skyLoopFront;
	image* skyLoopMiddle;
	image* skyLoopBack;

	float skyLoopFront_offsetX;
	float skyLoopMiddle_offsetX;
	float skyLoopBack_offsetX;

	float cloud1_offsetX;
	float cloud2_offsetX;
	// 카메라에 구름넣을때 필요한 변수x,y
	float cloudX;
	float cloudY;

	image* _background;
	image* _backgroundBehind;

	image* _backgroundCheck;

	int cameraX;
	int cameraY;

	// 피가 한번만 닳게끔. 
	bool hurt;
	int hitCount; 

	// 미니맵 포인트 
	int minimapFrameCout;
	int minimapCurrentX;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void doorPosInit(void);
	virtual void doorPosFunc(void);//각 다른씬에 들어갔다가 나오면 좌표를 설정. 

	virtual void Delete(void);
	virtual void Create(int Num);

	virtual void collisionFunc(void);

	RECT miniRC; // 플레이어
	RECT mapRC;
	virtual void minimapUpdate();

	virtual void velliCameraMove();

	eventScene();
	~eventScene();
};

