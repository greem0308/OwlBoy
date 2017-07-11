#pragma once
#include "enemy.h"

class shooter : public enemy
{
public:
	float angle;
	int frameCount;
	int currentX;
	int hitCount;
	int attackCount;
	bool stop; //성앞에서면 스탑
	
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	shooter();
	~shooter();
};

