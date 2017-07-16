#pragma once
#include "enemy.h"

class reviver : public enemy
{
public:
	// 왔다갔다 
	int moveFrameCount;
	bool moveRight;
	bool moveLeft;

	struct tagCannon
	{
		POINT cannonEnd; // 끝점.

		int radius;
		int Length;

		float angle; //라디안 값임
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

