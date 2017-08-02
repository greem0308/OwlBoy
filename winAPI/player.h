#pragma once
#include "gameNode.h"
#include "animation.h"
#include "bullets.h"
#include "button.h"
#include "progressBar.h"
//#include "mp3Player.h"

#define CURVE_CIRCLE_SIZE 30 // 커브 공 사이즈. 커브 사이간격도 조절함. 
#define CURVE_CIRCLE_LINE 10 // 커브 공 몇개? 

#define Door

#define EFFECT_MAX 200

//로딩바 이미지 변경해준다
//_loadingBar->setGuage(_current, _vLoadItem.size());

enum DIRECTION
{
	RIGHT,LEFT,UP,DOWN
};

enum STATE
{
	IDLE=0,RUN,JUMP, TURN, FALL,ROLL, // 날지않을때
	FLYIDLE,FLYRUN,FLYTURN,FLYFALL,FLYROLL,FLYHOLD,FLYEAT,FLYDRINK,// 날때
	HURT,DIE,SLEEP	 // 양쪽 다 
};

//팔
enum SHOOTSTATE
{
	NONESHOOT, SHOOT
};

// 게디몸 
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

		//점프용
		float x, y, radius,angle, speed, gravity, groundgrv;
		bool ground;
	};
	tagPlayer _player;

	//minimap용 플레이어 좌표
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
		bool cast; // 던진다. 
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
	// 배경음 /////////////////////////////////////////////////////////////////////////////
	button* soundBtn;
	bool soundOpen;
	static void cbSoundBtn();
	// 사운드 숙제 복붙 

	// 플레이리스트 블록 배열..
	// 여기서는 바 렉트 3개.  
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

	char* NowPlayList; // 플레이리스트.. 

	float volume; // 볼륨값 변수 

	int PlayList;  //이 값을 스위치에 넣어서 스트링값으로 해서 nowplaylist에 넘길것임.  
	bool musicStart;
	int musicStartFrame;
	int playNum;

	virtual void soundInit();
	virtual void soundUpdate();
	virtual void soundRender();

	// 효과음 /////////////////////////////////////////////////////////////////////////////

	tagblock b1;

	int px1;
	int py1;

	bool MouseCheck1;
	RECT barRC1;
	RECT barRC21;

	char* NowPlayList1; // 플레이리스트.. 

	float volume1; // 볼륨값 변수 

	int PlayList1;  //이 값을 스위치에 넣어서 스트링값으로 해서 nowplaylist에 넘길것임.  
	bool musicStart1;
	int musicStartFrame1;
	int playNum1;

	 bool se1; // 플레이어 총알. 
	 bool se2; // 날기 (안됨 )
	 bool se3; // 먹기 
	 bool se4; // 링
	 bool se5; // 이벤트 브릿지. 
	 bool se6; // 대포. 
	 
	 bool se7; // 블록. 
	 bool se8; // 문열림. 
	 bool se9; // 적 폭발.
	 bool se10; // 물 줌. 

	 bool se11; // 보스 총알. 
	 bool se12; // 해적선 터질때. 

	virtual void soundInit1();
	virtual void soundUpdate1();
	virtual void soundRender1();
	virtual void setSoundIndex(int index);

	// 선생님껄로 시도.
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
		RECT wearRC;//착용
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
	virtual void geddyFrameFunc(void); // 게디몸 프레임. (팔은 따로 )
	virtual void geddyPixelCollision(void);
	virtual void geddyCastFunc();
	virtual void CurveLineFunc(void); // 던질떄 궤적.

	// 플레이어 몸 이랑 픽셀이랑 
	virtual void PixelCollision(void);
	virtual bool blueCollision(void);
	virtual bool greenCollision(void);
	virtual bool yellowCollision(void);

	// 총알이랑  픽셀이랑
	virtual void gunPixelCollision(void);

	// database 의 변수값을 플레이어 변수로 셋한다.. 
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

