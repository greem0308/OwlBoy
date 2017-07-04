#pragma once
#include "gameNode.h"
#include "player.h"

class eventBridgeScene : public gameNode
{
public:
	image* backgroundPink;
	image* backgroundBlue;
	image* behindBG;

	player* _player;

	// 배경 애니메이션
	int monk_frameCount;
	int monk_CurrentX;

	RECT monkRC;

	float offsetX; // loop 
	float offsetcloudX; // cloud loop 

	RECT rc;
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	eventBridgeScene();
	~eventBridgeScene();
};

