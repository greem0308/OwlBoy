#pragma once
#include "gameNode.h"
#include "player.h"

class VellieScene : public gameNode
{
public:

	player* _player;
	
	//ī�޶� �������Ѿ��ϹǷ� x,y,�� �ʿ���.
	struct structDoor
	{
		// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
		float x,y;
		RECT rc;
	};
	structDoor door[7];

	// �÷��̾� ��ġ���� ������ �� �ϴù��
	image* skyBG; //�⺻ �ڿ� �Ķ�.
	image* skyLoopFront; 
	image* skyLoopMiddle;
	image* skyLoopBack;

	image* _background;
	image* _backgroundBehind;

	image* _backgroundCheck;

	int cameraX;
	int cameraY;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void velliCameraMove();

	VellieScene();
	~VellieScene();
};

