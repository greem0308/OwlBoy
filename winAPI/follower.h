#pragma once
#include "enemy.h"

// 프레임 순서대로..
enum followerSTATE 
{   
	FOLLOWER_FOLLOW,
	FOLLOWER_SLEEP,
	FOLLOWER_WAKEUP,
	FOLLOWER_DIE
};

class follower : public enemy
{
public:

	followerSTATE fstate;
	int frameCount;
	int currentX;

	bool wakeup; //한번만 깨우기 위해. 
	bool follow; 
	bool hurt;

	int wakeupFrameCount;
	int wakeupCurrentX;

	int hitCount;

	float speedX;
	float speedY;

	int dieFrameCount;
	int dieCurrentX;

	int hurtFrameCount;
	int hurtCurrentX;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void frameFunc(void);

	// effect 
	struct tagFX
	{
		RECT rc;
		bool fire;
		int frameCount;
		int currentX;
	};
	tagFX fx0;

	virtual void fx0Init(void);
	virtual void fx0Update(void);
	virtual void fx0Render(void);

	follower();
	~follower();
};

