#pragma once
#include "gameNode.h"
#include "player.h"

class BossScene : public gameNode
{
public:
	player* _player;

	RECT rc;

	// ship 애니메이션. 
	int frameCount;
	int currentX;

	int cameraX;
	int cameraY;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void bossCameraMove();

	BossScene();
	~BossScene();
};

