#pragma once
#include "item.h"
class glassObject2 : public item
{
public:
	enum GlassSTATE2
	{
		glass_MOVE2, glass_IDLE2
	};

	float x, y;
	RECT rc; // �÷��̾�� �ε�ġ�� ������ ��Ʈ
	int frameCount;
	int currentX;
	int glassState;
	int hitCount;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	glassObject2();
	~glassObject2();
};

