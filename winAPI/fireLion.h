#pragma once
#include "enemy.h"

enum FlSTATE
{
	FL_NONE, FL_TURN, FL_WALK, FL_IDLE,
	FL_0A, FL_20A, Fl_40A, FL_60A, FL_80A, FL_90A, // 불뿜는 각도.
	FL_HURT, FL_DIE, FL_HURTDIE
};

class fireLion : public enemy
{
public:
	// 왔다갔다 
	int moveFrameCount;
	bool moveRight;
	bool moveLeft;

	FlSTATE flState;

	// 애니메이션
	int frameCount;
	int currentX;

	// fire 애니메이션
	int fireFrameCount;
	int fireCurrentX;

	// 사자와 플레이어의 앵글. 잡아서 불쏘려고.. 
	int lionAngle;

	// 때리는 렉트
	RECT hitRC;
	bool attack; 
	int attackFrameCount; //몇초 때릴거임?

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void frameFunc(void);
	virtual void fireFrameFunc(void);

	fireLion();
	~fireLion();
};

