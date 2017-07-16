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
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	follower();
	~follower();
};

