#pragma once
#include "gameNode.h"
#include "player.h"

class VellieScene : public gameNode
{
public:
	player* _player;
	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	VellieScene();
	~VellieScene();
};

