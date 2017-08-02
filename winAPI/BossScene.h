#pragma once
#include "gameNode.h"
#include "player.h"
#include "bullets.h"

#define RINGMAX 30

// �� 
enum bossSTATE
{
	bTOP,bMIDDLE,bDOWN, bIDLE, bRUN, bFALL,bHURT,bNONE
};
//��
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

	RECT frontRC; // �տ� ��������
	
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

		// follower ������.
		int FframeCount;
		int FcurrentX;

		// �� �� �� ������ 
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

		// �� �� Ÿ�̹�. 
		int gFrameCount;
		POINT down;

		// �Ÿ��� ���� ���� �Ҽ��� ��� ��Ʈ�� ����. 
		RECT downRC; 
		// ������ �������� �Ÿ�.
		float downDistance;

		bool hitCheck; // �¾ҳ�?
		int hitCount;  // �ѹ��� �°� �ϴ� ��
		int attackCount;// ��� ������ ������ ��ȯ�ǰ�

		missileM2*  _fire2;
	};
	tagBoss boss;

	bool start; // ���� ���� ����. 

	// ���� ���� , ���� ����
	bool bossAttack;
	bool cannonAttack;
	// ���� �ش��������� �̵��� �� �������� ������ �̵� �� ������ �޽� 
	bool bossIN;
	bool bossOUT;
	bool bossIN2;
	bool bossOUT2;
	bool bossIN3;

	bool bossFirstFollow; // ó�� ���������� �ǰԲ�.. bossIN������ �ɋ� �ε��ļ� ����. 

	// *  ��� �߿�. 
	// ���� ���� ī��Ʈ 
	int attackCount; // �������� ���º���... 
	int attackPettern;
	int shipAttackedCount2; // ���� 2���� ���� ��� �浹���� ����. 
	int shipAttackedCount3; // ���� 3 //
	int realCount2; // �������� ���ͼ� �������. 
	int realCount3;

	POINT maxTop;
	POINT maxBottom;
	bool down;
	bool up;

	// �� ��鸲. 
	int shipShakeFrame;
	bool shipShakeUp;
	bool shipShakeDown;
	bool shake;

	// ĳ�� �ξƿ� 
	int cannonInoutX;
	int cannonInoutY;
	cannonINOUT_STATE inoutState;
	int inoutFrameCount;
	int inoutCurrentX;

	bool talk; //talk.�̹��� �߰��ϱ� ���� ��ġ. 
	int talkFrame;
	int talkCurrentX;

	bool bossTalk; // �������� ����. 
	int  bossTalkFrame;// ���� �� �޽������� 2�� 

	// �÷��̾� ��ĥ�� 
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
		float angle; //���� ����
		int frameCount;
		int currentX;
		// ���� �ι� ���� ����� �ʿ��� ������ 
		int frameCount2;
		int currentX2;
		int frameCount3;
		int currentX3;

		bool fire;
		bulletM1* _bullet; // enemyShi
	};
	tagCannon _cannon;

	// ���� ������ ������ ������ ��.
	struct tagShipRock
	{
		RECT rc;
		float x, y;
		bool life;
		bool fall;
		float shipOffsetY; //�������� ���ӵ�
	};
	tagShipRock shipRock[3];

	int cannonFrameNum;

	// ����Ʈ �ð�. 
	int fxFrameCount;
	int fxCurrentX;

	// �ѱ� �ִϸ��̼�.  
	int gun1State;
	int gunFrameCount;
	int gunCurrentX; // ����. ��. 6��..
	int gunCurrentY; // ���Ⱑ ������ ���� �����ؾ��� y������. 

	virtual void shipInit();
	virtual void shipUpdate();
	virtual void shipRender();
	virtual void shipShoot(void);
	virtual void gunFrameFunc(void);
	virtual void shipRockCollision(); // ������� ������ �ε�ġ�� ��������.

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
	// 0 = ��, 1= ����Ʈ1, 2=����Ʈ2
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

