#pragma once
#include "gameNode.h"
#include "player.h"

class VellieScene : public gameNode
{
public:

	player* _player;
	
	RECT rc;
	
	//카메라에 연동시켜야하므로 x,y,가 필요함.
	struct structDoor
	{
		float otusX, otusY;
		RECT otusRC; //오투스하우스로 들어가는 렉트.
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

