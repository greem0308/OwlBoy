#pragma once
#include "gameNode.h"
#include "player.h"
#include "animation.h"

class labScene : public gameNode
{
public:
	image* backgroundPink;
	image* backgroundBlue;

	player* _player;
	
	// ��� �ִϸ��̼�
	int frameCount;
	int CurrentX;

	int frameCount2;
	int CurrentX2;
	int frameCount3;
	int CurrentX3;
	int frameCount4;
	int CurrentX4;
	int frameCount5;
	int CurrentX5;
	int frameCount6;
	int CurrentX6;

	int frameCount7;
	int CurrentX7;

	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void frameFunc(void); // �����Ѱ͵� ������

	labScene();
	~labScene();
};

