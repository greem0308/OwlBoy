#pragma once
#include "gameNode.h"
#include "player.h"

class labScene : public gameNode
{
public:
	image* backgroundPink;
	image* backgroundBlue;

	player* _player;

	// ��� �ִϸ��̼�
	int frameCount;
	int CurrentX;

	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	labScene();
	~labScene();
};

