#pragma once
#include "gameNode.h"
#include "player.h"
#include "animation.h"

class item : public gameNode
{
public:
	enum itemState
	{
		STAY, THROWED, CONSUMED // °¡¸¸È÷ÀÖÀ½, ´øÁ®Áü, ¸ÔÈû. 
	};

	struct tagItem
	{
		image* image;
		RECT rc;
		itemState temState;
		float x, y;
		int coinUp;
		int hpUp;

		bool hitCheck; // glassObject 
		bool life;

		//for jump, gravity___________________________
		bool ground;
		float gravity;
		float groundgrv;
		float radius, angle, speed;
	};
	tagItem tem;

	player* otus;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void setPlayer(player* _player) { otus = _player; }

	item();
	~item();
};

