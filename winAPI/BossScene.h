#pragma once
#include "gameNode.h"
#include "player.h"
#include "bullets.h"

#define RINGMAX 30

// 몸 
enum bossSTATE
{
	bTOP,bMIDDLE,bDOWN, bIDLE, bRUN, bFALL,bHURT,bNONE
};
//팔
enum bossSHOOTSTATE
{
	bossNONESHOOT, bossSHOOT
};

enum bossDIRECTION
{
	bossRIGHT, bossLEFT
};

enum bfollowerSTATE
{
	BF_MOVE,BF_SLEEP, BF_WAKEUP
};

enum cannonINOUT_STATE
{
  CANNON_IDLE,CANNON_IN, CANNON_OUT
};

class BossScene : public gameNode
{
public:
	player* _player;

	RECT rc;

	int cameraX;
	int cameraY;

	//loop
	float layer1_offsetX;
	float layer2_offsetX;

	RECT frontRC; // 앞에 돌무더기
	
	// boss _______________________________________________________________________________________________________boss
	struct tagBoss
	{
		RECT followerRC;
		RECT bodyRC;
		RECT gunRC;
		float x,y,fx,fy,speed,angle, shootSpeed;
		
		bool life;
		bool follow;
		int frameCount;
		int currentX;

		// follower 프레임.
		int FframeCount;
		int FcurrentX;

		// 총 쏠 팔 프레임 
		int shootFrameCount;
		int shootCurrentX;

		int gunFrameCount;
		int gunCurrentX;
		int gunCurrentY;

		int bodyFrameX;
		int bodyFrameY;
	
		bossSTATE bossState;
		bossSHOOTSTATE shootState;
		bossDIRECTION direction;
		bfollowerSTATE bfState;

		float bodyExcelY; 

		float followerExcelX;
		float followerExcelY;
		float followerExcelY_Down;

		float excelDown;
		float excelUp;
		
		int downFrameCount;

		// 총 쏠 타이밍. 
		int gFrameCount;
		POINT down;

		// 거리에 따라 가게 할수가 없어서 렉트를 만듬. 
		RECT downRC; 
		// 지점과 보스간의 거리.
		float downDistance;

		bool hitCheck; // 맞았냐?
		int hitCount;  // 한번만 맞게 하는 얘
		int attackCount;// 몇번 맞으면 대포로 전환되게

		missileM2*  _fire2;
	};
	tagBoss boss;

	bool start; // 보스 공격 시작. 

	// 보스 공격 , 대포 공격
	bool bossAttack;
	bool cannonAttack;
	// 보스 해당지점으로 이동한 후 내려가서 대포로 이동 후 라이프 펄스 
	bool bossIN;
	bool bossOUT;
	bool bossIN2;
	bool bossOUT2;
	bool bossIN3;

	bool bossFirstFollow; // 처음 시작했을때 되게끔.. bossIN조건이 될떄 부딧쳐서 만듬. 

	// *  얘네 중요. 
	// 공격 패턴 카운트 
	int attackCount; // 몇대맞으면 상태변경... 
	int attackPettern;
	int shipAttackedCount2; // 패턴 2에서 돌이 몇번 충돌할지 정함. 
	int shipAttackedCount3; // 패턴 3 //
	int realCount2; // 연속으로 들어와서 만들어줌. 
	int realCount3;

	POINT maxTop;
	POINT maxBottom;
	bool down;
	bool up;

	// 배 흔들림. 
	int shipShakeFrame;
	bool shipShakeUp;
	bool shipShakeDown;
	bool shake;

	// 캐논 인아웃 
	int cannonInoutX;
	int cannonInoutY;
	cannonINOUT_STATE inoutState;
	int inoutFrameCount;
	int inoutCurrentX;

	bool talk; //talk.이미지 뜨게하기 위한 장치. 
	int talkFrame;
	int talkCurrentX;

	bool bossTalk; // 수도승이 말함. 
	int  bossTalkFrame;// 말한 후 펄스되지전 2초 

	// 플레이어 다칠때 
	bool hurt; 
	int hitCount;

	virtual void bossInit();
	virtual void bossUpdate();
	virtual void bossRender();

	virtual void bossAttackFunc();
	virtual void bossINupdate();
	virtual void bossOUTupdate();
	virtual void bossPettern();
	virtual void bossFunc();

	virtual void bossFrameFunc();
	virtual void bossFollowerFunc();
	virtual void cannonINPUTframeFunc();

	virtual void bossStart();

	virtual void bossCollision();

    virtual bool pinkCollision();
	virtual bool blueCollision();
	
	 
	// ship _____________________________________________________________________________________________________ship
	enum tagGun1
	{
		gun1IDLE, gun1WAIT, gun1SHOOT
	};

	struct tagCannon
	{
		RECT rc;
		POINT center;
		POINT cannonEnd;
		float shootAngle;
		int radius;
		int cannon;
		float angle; //라디안 값임
		int frameCount;
		int currentX;
		// 대포 두번 연속 쏘려면 필요한 변수들 
		int frameCount2;
		int currentX2;
		int frameCount3;
		int currentX3;

		bool fire;
		bulletM1* _bullet; // enemyShi
	};
	tagCannon _cannon;

	// 위에 대포에 맞으면 떨어질 돌.
	struct tagShipRock
	{
		RECT rc;
		float x, y;
		bool life;
		bool fall;
		float shipOffsetY; //떨어질떄 가속도
	};
	tagShipRock shipRock[3];

	int cannonFrameNum;

	// 이펙트 시간. 
	int fxFrameCount;
	int fxCurrentX;

	// 총구 애니메이션.  
	int gun1State;
	int gunFrameCount;
	int gunCurrentX; // 가로. 즉. 6번..
	int gunCurrentY; // 여기가 각도에 따라서 선택해야할 y프레임. 

	virtual void shipInit();
	virtual void shipUpdate();
	virtual void shipRender();
	virtual void shipShoot(void);
	virtual void gunFrameFunc(void);
	virtual void shipRockCollision(); // 배대포랑 위에돌 부딧치면 떨어지게.

	virtual void removeMissile1(int arrNum);
	
	inline bulletM1* getMissile1(void) { return _cannon._bullet; }


	// waterfall __________________________________________________________________________________________________waterfall
	struct tagWaterfall
	{
		RECT rc;
		float x, y;
		int frameCount;
		int currentX;
	};
	tagWaterfall waterFall[3];

	
	struct tagShip
	{
		float x, y;
		RECT rc;
		int frameCount;
		int currentX;
	};
	tagShip ship;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void bgFrameFunc();
	virtual void bgRender();

	virtual void removeMissile(int arrNum);

	inline missileM2* getMissile(void) { return boss._fire2; }


	//shipEffectFunc__________________________________________________________________________________________shipEffectFunc
	// 0 = 배, 1= 이펙트1, 2=이펙트2
	struct tagEffect
	{
		RECT rc;
		bool life;
		float x, y;
		int frameCount;
		int currentX;
	};
	tagEffect fx[5];

	virtual void shipEffectInit();
	virtual void shipEffectUpdate();
	virtual void shipEffectRender();
	virtual void shipEffectInPettern();


	// effect &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& effect

	struct tagFX3
	{
		RECT rc;
		bool fire;
		int frameCount;
		int currentX;
	};
	tagFX3 fx3;

	virtual void fx3Init(void);
	virtual void fx3Update(void);
	virtual void fx3Render(void);

	BossScene();
	~BossScene();
};

