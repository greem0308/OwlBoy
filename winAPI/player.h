#pragma once
#include "gameNode.h"
#include "animation.h"
#include "bullets.h"

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

enum SHOOTSTATE
{
	NONESHOOT, SHOOT
};

enum GeddySTATE
{
	gIDLE,gFALL,gFOLLOW
};

class player : public gameNode
{
public:
	struct tagPlayer
	{
		image* imgR;
		image* imgL;
		RECT rc;
		RECT heatRC;
		bool ground;
		bool jump;
		bool fly;
		bool fall;
		bool life;
		bool turn;
		bool flyTurn;
		int jumpCount;
		int hp;
		RECT HPbar;
		int coin;
		int frameCount;
		int currentX;
		float x, y, radius,angle, speed, gravity, groundgrv;
		STATE state;
		DIRECTION direction;
		SHOOTSTATE shootState;
		int shootFrameCount;
		int shootCurrentY;

		missileM1*  _fire;
	};
	tagPlayer _player;

	struct tagGeddy
	{
		bool follow;
		RECT rc;
		RECT heatRC;
		int bodyFrameX;
		int bodyFrameY;
		int gunFrameX;
		int gunFrameY;
		float x, y, radius;
		RECT gunRC;
		float gunX, gunY, gunRadius;
		//for jump, gravity___________________________
		bool ground;
		float gravity, groundgrv;
	};
	tagGeddy geddy;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void keyControl(void);
	virtual void jump(void);
	virtual void frameFunc(void);
	virtual void PixelCollision(void);

	virtual void geddyFunc(void);
	virtual void geddyPixelCollision(void);

	// database �� �������� �÷��̾� ������ ���Ѵ�.. 
	void setHP(int hp) { _player.hp = hp; }
	void setCoin(int coin) { _player.coin = coin; }

	virtual void removeMissile(int arrNum);

	inline missileM1* getMissile(void) { return _player._fire; }
	inline RECT getRC(void) { return _player.rc; }

	player();
	~player();
};

