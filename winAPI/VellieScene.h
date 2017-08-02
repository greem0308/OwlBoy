#pragma once
#include "gameNode.h"
#include "player.h"
#include "itemManager.h"

#define RINGMAX 30

class VellieScene : public gameNode
{
public:
	player* _player;
	itemManager* _im;

	// �������� �� ��ġ. 
	//ī�޶� �������Ѿ��ϹǷ� x,y,�� �ʿ���. 
	struct structDoor
	{
		// 0.����,1.��������. 2.����. 3.��, 4.�չ���, 5. �̺�Ʈ�긴��, 6.����
		float x,y;
		RECT rc;
	};
	structDoor door[7];

	RECT miniRC; // �÷��̾�
	RECT mapRC;

	// �÷��̾� ��ġ���� ������ �� �ϴù��
	image* skyBG; //�⺻ �ڿ� �Ķ�.
	image* skyLoopFront; 
	image* skyLoopMiddle;
	image* skyLoopBack;

	float skyLoopFront_offsetX;
	float skyLoopMiddle_offsetX;
	float skyLoopBack_offsetX;

	float cloud1_offsetX;
	float cloud2_offsetX;
	// ī�޶� ���������� �ʿ��� ����x,y
	float cloudX;
	float cloudY;

	image* _background;
	image* _backgroundBehind;

	image* _backgroundCheck;

	int cameraX;
	int cameraY;
	
	int posX;
	int posY;

	// �̴ϸ� ����Ʈ 
	int minimapFrameCout;
	int minimapCurrentX;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	
	virtual void doorPosInit(void);
	virtual void doorPosFunc(void);//�� �ٸ����� ���ٰ� ������ ��ǥ�� ����. 

	int itemShowFrame;
	virtual void Delete(void);
	virtual void Create(int Num);

	virtual void velliCameraMove();

	virtual void minimapUpdate();

	virtual void collision(void);

	// �� _______________________________________________________________________________________________��

	// ����
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
		bool eat;
		int eatCount;

		int frameCount;
		int currentX;

		int eatenFrameCount; // ���� �� �ִϸ��̼� �� ������ �ֱ� ���� ������. 
		RINGSTATE ringState;
	};
	tagRing ring[RINGMAX];

	virtual void ringInit();
	virtual void ringUpdate();
	virtual void ringRender();
	virtual void ringRenderBehind();
	virtual void ringFrameFunc();

	// ����
	enum WRINGSTATE
	{
		WRING_IDLE, WRING_EATEN
	};

	struct WtagRing
	{
		RECT rc;
		float x, y, w, h;
		bool life;
		bool eaten;
		bool eat;
		int eatCount;
		int frameCount;
		int currentX;

		int eatenFrameCount; // ���� �� �ִϸ��̼� �� ������ �ֱ� ���� ������. 
		WRINGSTATE wringState;
	};
	WtagRing wring[RINGMAX];


	virtual void WringInit();
	virtual void WringUpdate();
	virtual void WringRender();
	virtual void WringRenderBehind();
	virtual void WringFrameFunc();

	float sounaX,sounaY;
	int sounaFrameCount;
	int sounaCurrentX;
	bool souna;

	VellieScene();
	~VellieScene();
};

