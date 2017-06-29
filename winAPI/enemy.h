#pragma once
#include "gameNode.h"
#include "player.h"
#include "animation.h"
#include "bullets.h"

class enemy : public gameNode
{
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	enemy();
	~enemy();
};

