#pragma once
#include "gameNode.h"
#include "player.h"
#include "button.h"

class storeScene : public gameNode
{
public:
	button* buyItem1Btn;
	button* buyItem2Btn;
	button* buyItem3Btn;

	image* backgroundPink;
	image* backgroundBlue;

	player* _player;

	//�������� ��Ʈ. 
	RECT NPCrc;
	// ���� ���� �ִϸ��̼�
	int NPCframeCount;
	int NPCcurrentX;


	// ����â ����
	bool storeWindow;
	

	// ��� �ִϸ��̼�
	int frameCount;
	int CurrentX;

	RECT rc; // ������ ���� ��. 

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	static void cbItem1BuyBtn(void); //buyBtn �ݹ� �Լ�1.
	static void cbItem2BuyBtn(void); //buyBtn �ݹ� �Լ�2.
	static void cbItem3BuyBtn(void); //buyBtn �ݹ� �Լ�3.

	storeScene();
	~storeScene();
};

