#pragma once
#include "enemy.h"

enum shooterSTATE
{
	SHOOTER_IDLE,
	SHOOTER_MOVE,
	SHOOTER_WAIT,
	SHOOTER_ATTACK,
	SHOOTER_DIE
};

class shooter : public enemy
{
public:
	float angle;
	int frameCount;
	int currentX;
	int hitCount;
	int attackCount;
	bool stop; //성앞에서면 스탑
	int shooterState;

	int waitFrameCount;
	
	// 왔다갔다 
	int moveFrameCount;
	bool moveRight;
	bool moveLeft;

	bool attack;
	int attackFrameCount;

	bool waitbool;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	shooter();
	~shooter();
};

