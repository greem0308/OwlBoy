#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"

class eventScene : public gameNode
{
public:
	player* _player;
	enemyManager* _em;
	int shipShowFrame; // �ѹ��� ��Ÿ���� �ϴ� ����.

	//ī�޶� �������Ѿ��ϹǷ� x,y,�� �ʿ���.
	struct structDoor
	{
		// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
		float x, y;
		RECT rc;
	};
	structDoor door[7];

	// �÷��̾� ��ġ���� ������ �� �ϴù��
	image* skyBG; //�⺻ �ڿ� �Ķ�.
	image* skyLoopFront;
	image* skyLoopMiddle;
	image* skyLoopBack;

	float skyLoopFront_offsetX;
	float skyLoopMiddle_offsetX;
	float skyLoopBack_offsetX;

	float cloud1_offsetX;
	float cloud2_offsetX;
	// ī�޶� ���������� �ʿ��� ����x,y
	float cloudX;
	float cloudY;

	image* _background;
	image* _backgroundBehind;

	image* _backgroundCheck;

	int cameraX;
	int cameraY;

	// �ǰ� �ѹ��� ��Բ�. 
	bool hurt;
	int hitCount; 

	// �̴ϸ� ����Ʈ 
	int minimapFrameCout;
	int minimapCurrentX;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void doorPosInit(void);
	virtual void doorPosFunc(void);//�� �ٸ����� ���ٰ� ������ ��ǥ�� ����. 

	virtual void Delete(void);
	virtual void Create(int Num);

	virtual void collisionFunc(void);

	RECT miniRC; // �÷��̾�
	RECT mapRC;
	virtual void minimapUpdate();

	virtual void velliCameraMove();

	eventScene();
	~eventScene();
};

