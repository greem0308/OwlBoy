#pragma once
#include "gameNode.h"
#include "animation.h"
#include "bullets.h"
#include "button.h"
#include "progressBar.h"
//#include "mp3Player.h"

#define CURVE_CIRCLE_SIZE 30 // Ŀ�� �� ������. Ŀ�� ���̰��ݵ� ������. 
#define CURVE_CIRCLE_LINE 10 // Ŀ�� �� �? 

#define Door

#define EFFECT_MAX 200

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
	// mp3Player* _mp3Player;
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
	// ����� /////////////////////////////////////////////////////////////////////////////
	button* soundBtn;
	bool soundOpen;
	static void cbSoundBtn();
	// ���� ���� ���� 

	// �÷��̸���Ʈ ��� �迭..
	// ���⼭�� �� ��Ʈ 3��.  
	struct tagblock
	{
		RECT rc;
		int x, y;
	};
	tagblock b[3];

	int px;
	int py;

	bool MouseCheck;
	RECT barRC;
	RECT barRC2;

	char* NowPlayList; // �÷��̸���Ʈ.. 

	float volume; // ������ ���� 

	int PlayList;  //�� ���� ����ġ�� �־ ��Ʈ�������� �ؼ� nowplaylist�� �ѱ����.  
	bool musicStart;
	int musicStartFrame;
	int playNum;

	virtual void soundInit();
	virtual void soundUpdate();
	virtual void soundRender();

	// ȿ���� /////////////////////////////////////////////////////////////////////////////

	tagblock b1;

	int px1;
	int py1;

	bool MouseCheck1;
	RECT barRC1;
	RECT barRC21;

	char* NowPlayList1; // �÷��̸���Ʈ.. 

	float volume1; // ������ ���� 

	int PlayList1;  //�� ���� ����ġ�� �־ ��Ʈ�������� �ؼ� nowplaylist�� �ѱ����.  
	bool musicStart1;
	int musicStartFrame1;
	int playNum1;

	 bool se1; // �÷��̾� �Ѿ�. 
	 bool se2; // ���� (�ȵ� )
	 bool se3; // �Ա� 
	 bool se4; // ��
	 bool se5; // �̺�Ʈ �긴��. 
	 bool se6; // ����. 
	 
	 bool se7; // ���. 
	 bool se8; // ������. 
	 bool se9; // �� ����.
	 bool se10; // �� ��. 

	 bool se11; // ���� �Ѿ�. 
	 bool se12; // ������ ������. 

	virtual void soundInit1();
	virtual void soundUpdate1();
	virtual void soundRender1();
	virtual void setSoundIndex(int index);

	// �����Բ��� �õ�.
	string _musicTitle;
	int _soundIndex;

	vector<string> _vTitle;

	virtual void play();
	virtual void stop();
	virtual void pause();

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

	////effect _________________________________________________________________________________________________________effect
	//struct tagPlayerEffect
	//{
	//	RECT rc;
	//	float x, y, radius;
	//	int frameCount, currentX;
	//	bool life;
	//	bool fire;
	//	int lifeFrame;
	//};
	//tagPlayerEffect pEffect[EFFECT_MAX];

	//bool otherHit;
	//int otherHitCount;

	//virtual void effectInit(void);
	//virtual void effectUpdate(void);
	//virtual void effectRender(void);
	//virtual void effectFire(void);

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void keyControl(void);
	virtual void frameFunc(void); 

	virtual void jump(void);
	
	virtual void geddyFunc(void);
	virtual void geddyFrameFunc(void); // �Ե�� ������. (���� ���� )
	virtual void geddyPixelCollision(void);
	virtual void geddyCastFunc();
	virtual void CurveLineFunc(void); // ������ ����.

	// �÷��̾� �� �̶� �ȼ��̶� 
	virtual void PixelCollision(void);
	virtual bool blueCollision(void);
	virtual bool greenCollision(void);
	virtual bool yellowCollision(void);

	// �Ѿ��̶�  �ȼ��̶�
	virtual void gunPixelCollision(void);

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

