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
	IDLE,RUN,JUMP,ROLL,ROTATE, // 날지않을때
	FLYIDLE,FLYRUN,FLYIDLE_HOLD,FLYRUN_HOLD,FLYFALL,FLYEAT,FLYROLL,FLYROTATE, // 날때
	DIE,SLEEP,HURT,CONSUME,DRINK,FALL // 양쪽 다 
};

class player : public gameNode
{
public:
	struct tagPlayer
	{
		image* img;
		RECT rc;
		RECT heatRC;
		bool ground;
		bool jump;
		int jumpCount;
		int hp;
		int coin;
		float x, y, radius,angle, speed, gravity, groundgrv;
		STATE state;
		missileM1*  bullet;
	};
	tagPlayer _player;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void keyControl(void);

	void setHP(int hp) { _player.hp = hp; }

	player();
	~player();
};

