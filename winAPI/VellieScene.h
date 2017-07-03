#pragma once
#include "gameNode.h"
#include "player.h"

class VellieScene : public gameNode
{
public:

	player* _player;
	
	RECT rc;
	
	//ī�޶� �������Ѿ��ϹǷ� x,y,�� �ʿ���.
	struct structDoor
	{
		float otusX, otusY;
		RECT otusRC; //�������Ͽ콺�� ���� ��Ʈ.
	};
	structDoor door;

	image* _background;
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

