#pragma once
#include "gameNode.h"
#include "animation.h"

class effect : public gameNode
{
public:
	struct tagEffect
	{
		image* image;
		RECT rc;
		float x, y;
		int frameCount;
		int currentX;
		int currentY;
		bool life;
		animation* currentAni;
	};
	tagEffect fx;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	
	effect();
	~effect();
};

