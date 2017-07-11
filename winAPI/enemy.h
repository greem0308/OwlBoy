#pragma once
#include "gameNode.h"
#include "player.h"
#include "animation.h"
#include "bullets.h"

enum ENEMYACTION
{
	ENEMY_MOVE,
	ENEMY_ATTACK,
	ENEMY_DIE
};

class enemy : public gameNode
{
public:
	struct ENEMY
	{
		float x, y;
		float fireX, fireY;
		bool hitCheck;
		bool life;
		bool AI;
		int Count;
		int HP;
		RECT HPbar;
		RECT rc;
		image* image;
		bulletM1* _bullet;
		ENEMYACTION action;
	};

	ENEMY _enemy;
	animation* curAnimation;
	player* otus;

	bool Start;
	int Count;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void setPlayer(player* _player) { otus = _player; }

	enemy();
	~enemy();
};

