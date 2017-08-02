#pragma once
#include "gameNode.h"
#include "player.h"

class pixelTestScene :public gameNode
{
public:
	player* _player;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	pixelTestScene();
	~pixelTestScene();
};

