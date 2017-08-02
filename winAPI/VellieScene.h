#pragma once
#include "gameNode.h"
#include "player.h"
#include "itemManager.h"

#define RINGMAX 30

class VellieScene : public gameNode
{
public:
	player* _player;
	itemManager* _im;

	// 마을에서 문 위치. 
	//카메라에 연동시켜야하므로 x,y,가 필요함. 
	struct structDoor
	{
		// 0.마을,1.오투스집. 2.상점. 3.랩, 4.붐버맨, 5. 이벤트브릿지, 6.던전
		float x,y;
		RECT rc;
	};
	structDoor door[7];

	RECT miniRC; // 플레이어
	RECT mapRC;

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
	
	int posX;
	int posY;

	// 미니맵 포인트 
	int minimapFrameCout;
	int minimapCurrentX;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	
	virtual void doorPosInit(void);
	virtual void doorPosFunc(void);//각 다른씬에 들어갔다가 나오면 좌표를 설정. 

	int itemShowFrame;
	virtual void Delete(void);
	virtual void Create(int Num);

	virtual void velliCameraMove();

	virtual void minimapUpdate();

	virtual void collision(void);

	// 링 _______________________________________________________________________________________________링

	// 세로
	enum RINGSTATE
	{
		RING_IDLE, RING_EATEN
	};

	struct tagRing
	{
		RECT rc;
		float x, y, w, h;
		bool life;
		bool eaten;
		bool eat;
		int eatCount;

		int frameCount;
		int currentX;

		int eatenFrameCount; // 먹을 때 애니메이션 될 여지를 주기 위한 프레임. 
		RINGSTATE ringState;
	};
	tagRing ring[RINGMAX];

	virtual void ringInit();
	virtual void ringUpdate();
	virtual void ringRender();
	virtual void ringRenderBehind();
	virtual void ringFrameFunc();

	// 가로
	enum WRINGSTATE
	{
		WRING_IDLE, WRING_EATEN
	};

	struct WtagRing
	{
		RECT rc;
		float x, y, w, h;
		bool life;
		bool eaten;
		bool eat;
		int eatCount;
		int frameCount;
		int currentX;

		int eatenFrameCount; // 먹을 때 애니메이션 될 여지를 주기 위한 프레임. 
		WRINGSTATE wringState;
	};
	WtagRing wring[RINGMAX];


	virtual void WringInit();
	virtual void WringUpdate();
	virtual void WringRender();
	virtual void WringRenderBehind();
	virtual void WringFrameFunc();

	float sounaX,sounaY;
	int sounaFrameCount;
	int sounaCurrentX;
	bool souna;

	VellieScene();
	~VellieScene();
};

