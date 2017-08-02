#pragma once
#include "gameNode.h"
#include "player.h"
#include "effectManager.h"

class otusHouseScene :	public gameNode
{
public:
	image* backgroundPink;
	image* backgroundBlue;
	player* _player;
	effectManager* _efm;

	// 배경 애니메이션
	int frameCount;
	int CurrentX;

	int effectShowFrame;

	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void Delete(void);
	virtual void Create(void);

	otusHouseScene();
	~otusHouseScene();
};

