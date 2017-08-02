#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"
#include "itemManager.h"

#define RINGMAX 30

enum TEAMBOMB
{
	TB_IDLE, TB_DANGER1, TB_DANGER2, TB_WALK
};

enum DOOR
{
    D_IDLE,D_OPEN
};

enum KEYBLOCK
{
	KB_IDLE,KB_DOWN
};

enum WATERFALL
{
	WF_IDLE, WF_LAMPON,WF_FALL
};

class dungeonScene : public gameNode
{
public:
	player* _player;
	enemyManager* _em;
	itemManager* _item;

	bool camera;
	int cameraX;
	int cameraY;

	RECT rc;

	int enemyShowFrame; //enemy 백터를 한번만 나오게 할 카운터.

	// 피가 한번만 닳게끔. 
	bool hurt;
	int hitCount;

	// 플레이어 위치에서 루프만 할 하늘 배경 //////
	image* skyLayer1; //기본 뒤에 파랑.
	image* skyLayer2;
	image* skyLayer3; // 기본 뒤 배경 큰 것. 

	float layer1_offsetX;
	float layer2_offsetX;
	
	// puzzleRock
	struct tagPuzzleRock
	{
		RECT rc;
		bool Break;
		float x, y,radius;
	};
	
	int tbState; // teamBomb 스테이트
	int lampState; // lamp 스테이트

	// 던전 문 3개 
	struct tagDoor
	{
		RECT rc;
		RECT collisionRC;
		bool open; 
		float x, y,cx,cy;

		int frameCount;
		int currentX;
		DOOR doorState; // door 스테이트
	};

	// 시한 폭탄 
	struct tagSteamBomb
	{
		RECT rc;
		RECT hitRC; //터질때 플레이어 때릴렉트.
		bool life;
		bool timeOver;
		float x, y;
		TEAMBOMB tbState;

		int frameCount;
		int currentX;

		// 들고 던져지기 위한 변수들
		bool follow;
		bool cast;
		bool showCurve ;
		float castGravity;
		float speed;
		float angle;

		float gravity ;
		float groundgrv ;
		bool ground; 

		bool gravityOpen;

		int downLength; // follow될때 어느높이로 들건지. 

		// 들었는데 시간 지나면 폭발하게. 
		int timeframeCount;
		int timecurrentX;

		// 왔다갔다. 
		int moveFrame;
		bool moveUp;
		bool moveDown;
	};

	// 누르면 문열리는 블록
	struct tagKeyBlock
	{
		RECT rc; // 이미지렌더를 위한 렉트
		RECT collisionRC;// 누르면 정말 아래로 내려가게 하는 렉트
		bool down;
		bool colliRCdown; // 충돌되는 렉트 사라지게하기.
		float x, y,cx,cy;
		KEYBLOCK kbState;

		int frameCount;
		int currentX;

		// 블록 내려갈 프레임..
		int frameCount0; 
		int currentX0;

		int frameCount1;
		int currentX1;
	};
	
	// 구름
	struct tagCloud
	{
		RECT rc;
		RECT waterRC; //물 렉트.
		bool life;
		bool waterFall;
		float x, y;
		bool follow;

		// 구름 자체 애니메이션
		int frameCount;
		int currentX;

		// 구름에서 떨어지는 물 
		int waterFrameCount;
		int waterCurrentX;

		// 우물 프레임. 
		RECT waterBowl;
		float wbX, wbY;
		WATERFALL waterState;
		int wfFrameCount;
		int wfCurrentX;

		// 우물 물 차면 램프온되고, 문 오픈 하는 프레임. 
		int timeFrameCount;
	};

	// lamp 
	struct tagBowl
	{
		RECT rc;
		float x, y;
		bool up;
	};

	tagPuzzleRock puzzleRock;
	tagBowl bowl[2];
	tagSteamBomb teamBomb;
	tagCloud cloud[2];
	tagKeyBlock keyBlock[3];
	tagDoor door[3];

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void dungeonCameraMove();

	virtual void Delete(void);
	virtual void Create(int Num);

	virtual void collisionFunc();
	virtual void toolCollisionFunc();

	virtual void toolFrameFunc();
	
	virtual void showRect();

	// 0번 문 열때 필요한 것들 모음
	virtual void doorFunc0();
	virtual void doorInit();

	// 키블록에 앉을때 플레이어랑.. 
	virtual void keyBlockCollision();

	// rc들 업데이트 할것들 모음
	virtual void updateRC();


	// 시한폭탄 함수 __________________________________________________________________________________시한폭탄
	struct tagCurveLine
	{
		RECT rc;
		FLOAT angle;
		FLOAT x;
		FLOAT y;
	};
	tagCurveLine curveLine[50];

	virtual void teamBombInit();
	virtual void teamBombUpdate();

	virtual void CastFunc();
	virtual void CurveLineFunc();
	virtual void CurveLineRender();

	virtual void bombPixelCollision();
	virtual void bombMove();


	// 구름 ____________________________________________________________________________________________구름
	virtual void cloudInit();
	virtual void cloudUpdate();
	virtual void cloudRender();

	// 링 _______________________________________________________________________________________________링

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

		int frameCount;
		int currentX;
		
		bool eat;
		int eatCount;

		int eatenFrameCount; // 먹을 때 애니메이션 될 여지를 주기 위한 프레임. 
		RINGSTATE ringState;
	};
	tagRing ring[RINGMAX];

	virtual void ringInit();
	virtual void ringUpdate();
	virtual void ringRender();
	virtual void ringRenderBehind();
	virtual void ringFrameFunc();

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

		int frameCount;
		int currentX;
		
		bool eat;
		int eatCount;
		
		int eatenFrameCount; // 먹을 때 애니메이션 될 여지를 주기 위한 프레임. 
		WRINGSTATE wringState;
	};
	WtagRing wring[RINGMAX];

	virtual void WringInit();
	virtual void WringUpdate();
	virtual void WringRender();
	virtual void WringRenderBehind();
	virtual void WringFrameFunc();


	// 던전으로 가는 문. 
	struct tagBossDoor
	{
		float x, y;
		RECT toBossRC;

		// 문 막고 있는 렉트. 밀려지면 위로 점점 올라감. 
		bool push;
		float rx, ry;
		RECT bossRock;
	};
	tagBossDoor bossDoor;



	
	// effect &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& effect
	
	// 시한폭탄 시간 다 되면 터지는 이펙트 
	struct tagFX
	{
		RECT rc;
		bool fire;
		int frameCount;
		int currentX;
	};
	tagFX fx1;

	virtual void fx1Init(void);
	virtual void fx1Update(void);
	virtual void fx1Render(void);

	// 시한폭탄이랑 돌 터지는 이펙트 
	struct tagFX2
	{
		RECT rc;
		bool fire;
		int frameCount;
		int currentX;
	};
	tagFX fx2;

	virtual void fx2Init(void);
	virtual void fx2Update(void);
	virtual void fx2Render(void);

	dungeonScene();
	~dungeonScene();
};

