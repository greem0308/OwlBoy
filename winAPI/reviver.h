#pragma once
#include "enemy.h"

class reviver : public enemy
{
public:
	// �Դٰ��� 
	int moveFrameCount;
	bool moveRight;
	bool moveLeft;

	struct tagCannon
	{
		POINT cannonEnd; // ����.

		int radius;
		int Length;

		float angle; //���� ����
	};
	tagCannon _cannon;

	int frameCount;
	int currentX;
	
	int stopFrameCount;
	int stopCurrentX;
	bool stop;

	virtual HRESULT init(float x, float y, int length);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void frameFunc();
	
	reviver();
	~reviver();
};

