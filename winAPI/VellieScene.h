#pragma once
#include "gameNode.h"
#include "player.h"

class VellieScene : public gameNode
{
public:

	player* _player;
	
	//카메라에 연동시켜야하므로 x,y,가 필요함.
	struct structDoor
	{
		// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
		float x,y;
		RECT rc;
	};
	structDoor door[7];

	// 플레이어 위치에서 루프만 할 하늘배경
	image* skyBG; //기본 뒤에 파랑.
	image* skyLoopFront; 
	image* skyLoopMiddle;
	image* skyLoopBack;

	image* _background;
	image* _backgroundBehind;

	image* _backgroundCheck;

	int cameraX;
	int cameraY;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void velliCameraMove();

	VellieScene();
	~VellieScene();
};

