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
	
	int hitCount; // �¾ҳ�?
	bool fire; // �Ѿ� �i��

	bool anglefirst; // ó���� Ʈ��Ǿ�����. 
	bool angleReturn; // ó���� �޽��Ǿ��ִ� ��. 

	int playerToShipAngle; // �߰��ϸ� �Һ��� ���󰡴� ����
	int cannonFrameNum; // ĳ�� ������ �ޱ�.. 

	int lightFollowFrame; // ���󰡱��� ��. 
	
	RECT lightRC[4];

	int lightFrameNum; // �� ã�¾� �ޱۿ� ���� ������.
	int lightFrame64; // �� ������ 64...


	// �� ��鸲. 
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
		float angle; //���� ����
	};
	tagCannon _cannon;

	// �ѱ� �ִϸ��̼�.  
	int gunState;
	int gunFrameCount;
	int gunCurrentX; // ����. ��. 6��..
	int gunCurrentY; // ���Ⱑ ������ ���� �����ؾ��� y������. 
	
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

