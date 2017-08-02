#pragma once
#include "enemy.h"

enum tagGun
{
	gunIDLE,gunWAIT,gunSHOOT
};

class eventShip : public enemy
{
public:
	int frameCount; 
	int currentX;
	
	int hitCount; // 맞았냐?
	bool fire; // 총알 쐇냐

	bool anglefirst; // 처음에 트루되어있음. 
	bool angleReturn; // 처음에 펄스되어있는 불. 

	int playerToShipAngle; // 발견하면 불빛이 따라가는 변수
	int cannonFrameNum; // 캐논 프레임 앵글.. 

	int lightFollowFrame; // 따라가기전 텀. 
	
	RECT lightRC[4];

	int lightFrameNum; // 빛 찾는얘 앵글에 따른 프레임.
	int lightFrame64; // 빛 프레임 64...


	// 배 흔들림. 
	int shipShakeFrame;
	bool shipShakeUp;
	bool shipShakeDown;

	struct tagCannon
	{
		POINT center;
		POINT cannonEnd;
		POINT shootCenter;
		POINT shootCenterEnd;
		float shootAngle;
		int radius;
		int cannon;
		float angle; //라디안 값임
	};
	tagCannon _cannon;

	// 총구 애니메이션.  
	int gunState;
	int gunFrameCount;
	int gunCurrentX; // 가로. 즉. 6번..
	int gunCurrentY; // 여기가 각도에 따라서 선택해야할 y프레임. 
	
	float tempX;
	float tempY;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void shipShake(void);
	virtual void shipShoot(void);
	virtual void gunFrameFunc(void);

	eventShip();
	~eventShip();
};

