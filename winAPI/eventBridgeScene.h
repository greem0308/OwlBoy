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

	RECT monkRC; // 수도승 렉트
	RECT rc; // 마을 문 렉트
	RECT eventRC; // 이벤트 문 렉트

	// 펄스이면 마을문렉트를 렌더. // 트루이면 이벤트 문 렉트를 렌더. 
	bool eventDoor; 

	float offsetX; // loop 
	float offsetcloudX; // cloud loop 

	bool talk; //talk.이미지 뜨게하기 위한 장치. 
	int talkFrame;
	int talkCurrentX;

	bool monkTalk; // 수도승이 말함. 
	int monkTalkFrame;// 말한 후 펄스되지전 2초 

	bool eventBridgeStart; // 가속도 붙는 거 실행. 
	bool eventBridgeEnd; // 가속도 떨어지는것 실행. 
	int nightAlpha; //가속도가 다 붙었다가 다시 원래대로 돌아오는 시간동안의 앞에 깔린 밤배경 알파블렌드. 점점 까매지게. 
	int nightskyFrame; // 정수값이라 너무 빨리 어두워져서 낮추기위한 카운트. 
	float accelX;
	float accelCoundX; 

	int clockFrame; // 시계회전. 
	int clockCurrentX; 

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void eventFunc(void);

	eventBridgeScene();
	~eventBridgeScene();
};

