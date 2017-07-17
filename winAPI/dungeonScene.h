#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"
#include "itemManager.h"

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

	int enemyShowFrame; //enemy ���͸� �ѹ��� ������ �� ī����.

	// �ǰ� �ѹ��� ��Բ�. 
	bool hurt;
	int hitCount;

	// puzzleRock
	struct tagPuzzleRock
	{
		RECT rc;
		bool Break;
		float x, y,radius;
	};
	
	int tbState; // teamBomb ������Ʈ
	int lampState; // lamp ������Ʈ

	// ���� �� 3�� 
	struct tagDoor
	{
		RECT rc;
		RECT collisionRC;
		bool open; 
		float x, y,cx,cy;

		int frameCount;
		int currentX;
		DOOR doorState; // door ������Ʈ
	};

	// ���� ��ź 
	struct tagSteamBomb
	{
		RECT rc;
		bool life;
		bool timeOver;
		float x, y;
		TEAMBOMB tbState;

		int frameCount;
		int currentX;

		// ��� �������� ���� ������
		bool follow;
		bool cast;
		bool showCurve ;
		float castGravity;
		float speed;
		float angle;

		float gravity ;
		float groundgrv ;
		bool ground; 

		int downLength; // follow�ɶ� ������̷� �����. 

		// ����µ� �ð� ������ �����ϰ�. 
		int timeframeCount;
		int timecurrentX;
	};

	// ������ �������� ���
	struct tagKeyBlock
	{
		RECT rc; // �̹��������� ���� ��Ʈ
		RECT collisionRC;// ������ ���� �Ʒ��� �������� �ϴ� ��Ʈ
		bool down;
		bool colliRCdown; // �浹�Ǵ� ��Ʈ ��������ϱ�.
		float x, y,cx,cy;
		KEYBLOCK kbState;

		int frameCount;
		int currentX;

		// ��� ������ ������..
		int frameCount0; 
		int currentX0;

		int frameCount1;
		int currentX1;
	};
	
	// ����
	struct tagCloud
	{
		RECT rc;
		RECT waterRC; //�� ��Ʈ.
		bool life;
		bool waterFall;
		float x, y;
		bool follow;

		// ���� ��ü �ִϸ��̼�
		int frameCount;
		int currentX;

		// �������� �������� �� 
		int waterFrameCount;
		int waterCurrentX;

		// �칰 ������. 
		RECT waterBowl;
		float wbX, wbY;
		WATERFALL waterState;
		int wfFrameCount;
		int wfCurrentX;

		// �칰 �� ���� �����µǰ�, �� ���� �ϴ� ������. 
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

	// 0�� �� ���� �ʿ��� �͵� ����
	virtual void doorFunc0();
	virtual void doorInit();

	// Ű��Ͽ� ������ �÷��̾��.. 
	virtual void keyBlockCollision();

	// rc�� ������Ʈ �Ұ͵� ����
	virtual void updateRC();


	// ������ź �Լ� __________________________________________________________________________________������ź
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


	// ���� ____________________________________________________________________________________________����
	virtual void cloudInit();
	virtual void cloudUpdate();
	virtual void cloudRender();


	dungeonScene();
	~dungeonScene();
};

