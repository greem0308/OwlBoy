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

	//상점주인 렉트. 
	RECT NPCrc;
	// 상점 주인 애니메이션
	int NPCframeCount;
	int NPCcurrentX;


	// 상점창 열기
	bool storeWindow;
	

	// 배경 애니메이션
	int frameCount;
	int CurrentX;

	RECT rc; // 마을로 가는 문. 

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	static void cbItem1BuyBtn(void); //buyBtn 콜백 함수1.
	static void cbItem2BuyBtn(void); //buyBtn 콜백 함수2.
	static void cbItem3BuyBtn(void); //buyBtn 콜백 함수3.

	storeScene();
	~storeScene();
};

