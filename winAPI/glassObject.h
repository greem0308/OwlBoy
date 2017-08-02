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
	RECT rc; // �÷��̾�� �ε�ġ�� ������ ��Ʈ
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
