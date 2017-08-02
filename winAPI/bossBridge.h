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

	// �������� ��
	struct tagToBoss
	{
		RECT rc;
		float x;
		float y;
	};
	tagToBoss toBoss;



	// �� _______________________________________________________________________________________________��

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

		int eatenFrameCount; // ���� �� �ִϸ��̼� �� ������ �ֱ� ���� ������. 
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

	// �� _______________________________________________________________________________________________�� ROCK

	struct tagRock
	{
		// �ö�� ��
		float x, y, w, h;
		int frameCount;

		RECT rc;
		//�� ���ϰ� �ε�ġ�� ���� ���� �ö��. 
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

