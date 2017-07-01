#pragma once
#include "gameNode.h"
#include "player.h"

class otusHouseScene :	public gameNode
{
public:
	player* _player;

	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	otusHouseScene();
	~otusHouseScene();
};

