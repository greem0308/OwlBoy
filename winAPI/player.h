#pragma once
#include "gameNode.h"
#include "animation.h"
#include "bullets.h"
#include "button.h"
#include "progressBar.h"

#define CURVE_CIRCLE_SIZE 30 // Ŀ�� �� ������. Ŀ�� ���̰��ݵ� ������. 
#define CURVE_CIRCLE_LINE 10 // Ŀ�� �� �? 

#define Door

//�ε��� �̹��� �������ش�
//_loadingBar->setGuage(_current, _vLoadItem.size());

enum DIRECTION
{
	RIGHT,LEFT,UP,DOWN
};

enum STATE
{
	IDLE=0,RUN,JUMP, TURN, FALL,ROLL, // ����������
	FLYIDLE,FLYRUN,FLYTURN,FLYFALL,FLYROLL,FLYHOLD,FLYEAT,FLYDRINK,// ����
	HURT,DIE,SLEEP	 // ���� �� 
};

//��
enum SHOOTSTATE
{
	NONESHOOT, SHOOT
};

// �Ե�� 
enum GeddySTATE
{
	gSHOOT,gIDLE,gFALL,gNONE
};

class player : public gameNode
{
public:
	progressBar* _hpBar;

	struct tagPlayer
	{
		image* imgR;
		image* imgL;
		RECT rc;
		RECT heatRC;
		bool jump;
		bool fly;
		bool fall;
		bool life;
		bool turn;
		bool flyTurn;
		int jumpCount;
		int currentHP;
		int maxHP;
		RECT HPbar;
		int coin;
		int frameCount;
		int currentX;
		STATE state;
		DIRECTION direction;
		SHOOTSTATE shootState;

		int shootFrameCount;
		int shootCurrentY;

		float shootSpeed;
		missileM1*  _fire;

		//������
		float x, y, radius,angle, speed, gravity, groundgrv;
		bool ground;
	};
	tagPlayer _player;

	//minimap�� �÷��̾� ��ǥ
	float miniX, miniY;

	struct tagGeddy
	{
		bool follow;
		RECT rc;
		RECT heatRC;
		int bodyFrameX;
		int bodyFrameY;
		int gunFrameX;
		int gunFrameY;
		float angle;
		float speed;
		float castGravity;
		bool cast; // ������. 
		bool showCurve;

		float x, y, radius;
		RECT gunRC;
		float gunX, gunY, gunRadius;

		int frameCount;
	
		//for jump, gravity___________________________
		bool ground;
		GeddySTATE geddyState;
		float gravity, groundgrv;
	};
	tagGeddy geddy;

	struct tagCurveLine
	{
		RECT rc;
		FLOAT angle;
		FLOAT x;
		FLOAT y;
	};
	tagCurveLine curveLine[50];

	// sound __________________________________________________________________________________________________________
	button* soundBtn;

	bool soundOpen;
	static void cbSoundBtn();
	
	char* NowPlayList;
	virtual void soundInit();
	virtual void soundUpdate();
	virtual void soundRender();

	// inventory ______________________________________________________________________________________________________
	button* inventoryBtn;
	
	bool inventoryOpen;
	static void cbInventoryBtn(void);

	virtual void invenInit();
	virtual void invenUpdate();
	virtual void invenRender();

	struct tagInven
	{
		RECT wearRC;//����
		RECT notWearRC;
		bool notWear;
		bool wear;
		bool item;
	};
	tagInven inven[3];
	int itemTextCount;

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void keyControl(void);
	virtual void frameFunc(void); 

	virtual void jump(void);
	virtual void PixelCollision(void);

	virtual void geddyFunc(void);
	virtual void geddyFrameFunc(void); // �Ե�� ������. (���� ���� )
	virtual void geddyPixelCollision(void);
	virtual void geddyCastFunc();
	virtual void CurveLineFunc(void); // ������ ����.

	// database �� �������� �÷��̾� ������ ���Ѵ�.. 
	void setCurrentHP(int currentHP) { _player.currentHP = currentHP; } // currentHP
	void setMaxHP(int maxHP) { _player.maxHP = maxHP; }
	void setSpeed(float speed) { _player.speed = speed; }
	void setShootSpeed(float shootSpeed) { _player.shootSpeed = shootSpeed; }
	void setCoin(int coin) { _player.coin = coin; }
	void setsoundOpen(bool soundOpen) { soundOpen = soundOpen; }
	void setinventoryOpen(int inventoryOpen) { inventoryOpen = inventoryOpen; }

	virtual void removeMissile(int arrNum);

	inline missileM1* getMissile(void) { return _player._fire; }
	inline RECT getRC(void) { return _player.rc; }

	player();
	~player();
};

