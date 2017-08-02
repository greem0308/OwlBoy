#pragma once
#include "item.h"
class glassObject :
	public item
{
public:
	enum GlassSTATE
	{
		glass_MOVE,glass_IDLE
	};

	float x, y;
	RECT rc; // 플레이어와 부딧치면 반응할 렉트
	int frameCount;
	int currentX;
	int glassState;
	int hitCount;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	glassObject();
	~glassObject();
};
