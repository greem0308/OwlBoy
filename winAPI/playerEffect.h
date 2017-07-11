#pragma once
#include "effect.h"

class playerEffect :public effect
{
public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	int frame;
	playerEffect();
	~playerEffect();
};

