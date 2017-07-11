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
	bool stop; //���տ����� ��ž
	
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	shooter();
	~shooter();
};

