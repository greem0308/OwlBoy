#pragma once
#include "gameNode.h"
#include "player.h"
#include "itemManager.h"

#define RINGMAX 4

class bossBridge :	public gameNode
{
public:
	player* _player;
	itemManager* _im;

	float layer1_offsetX;
	float layer2_offsetX;

	// 보스가는 문
	struct tagToBoss
	{
		RECT rc;
		float x;
		float y;
	};
	tagToBoss toBoss;



	// 링 _______________________________________________________________________________________________링

	enum RINGSTATE
	{
		RING_IDLE, RING_EATEN
	};

	struct tagRing
	{
		RECT rc;
		float x, y, w, h;
		bool life;
		bool eaten;

		int frameCount;
		int currentX;

		bool eat;
		int eatCount;

		int eatenFrameCount; // 먹을 때 애니메이션 될 여지를 주기 위한 프레임. 
		RINGSTATE ringState;
	};
	tagRing ring[RINGMAX];
	
	virtual void ringInit();
	virtual void ringUpdate();
	virtual void ringRender();
	virtual void ringRenderBehind();
	virtual void ringFrameFunc();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);	

	// 돌 _______________________________________________________________________________________________돌 ROCK

	struct tagRock
	{
		// 올라올 돌
		float x, y, w, h;
		int frameCount;

		RECT rc;
		//이 돌하고 부딧치면 밑의 돌이 올라옴. 
		float cx, cy, cw, ch;
		RECT checkRC;
		bool lock;
		bool checkRCshow;
	};
	tagRock rock;

	virtual void rockInit();
	virtual void rockUpdate();
	virtual void rockRender();

	int itemShowFrame;
	virtual void Create(void);
	virtual void Delete(void);

	bossBridge();
	~bossBridge();
};

