#pragma once
#include "gameNode.h"
#include "player.h"
#include "button.h"

class VellieScene : public gameNode
{
public:
	button* soundBtn;
	button* inventoryBtn;

	player* _player;
	
	RECT rc;
	
	image* _background;
	image* _backgroundCheck;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	static void cbSoundBtn(void);
	static void cbInventoryBtn(void);

	VellieScene();
	~VellieScene();
};

