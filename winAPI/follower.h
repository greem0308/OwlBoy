#pragma once
#include "enemy.h"

// ������ �������..
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

