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

	// ��� �ִϸ��̼�
	int monk_frameCount;
	int monk_CurrentX;

	RECT monkRC; // ������ ��Ʈ
	RECT rc; // ���� �� ��Ʈ
	RECT eventRC; // �̺�Ʈ �� ��Ʈ

	// �޽��̸� ��������Ʈ�� ����. // Ʈ���̸� �̺�Ʈ �� ��Ʈ�� ����. 
	bool eventDoor; 

	float offsetX; // loop 
	float offsetcloudX; // cloud loop 

	bool talk; //talk.�̹��� �߰��ϱ� ���� ��ġ. 
	int talkFrame;
	int talkCurrentX;

	bool monkTalk; // �������� ����. 
	int monkTalkFrame;// ���� �� �޽������� 2�� 

	bool eventBridgeStart; // ���ӵ� �ٴ� �� ����. 
	bool eventBridgeEnd; // ���ӵ� �������°� ����. 
	int nightAlpha; //���ӵ��� �� �پ��ٰ� �ٽ� ������� ���ƿ��� �ð������� �տ� �� ���� ���ĺ���. ���� �������. 
	int nightskyFrame; // �������̶� �ʹ� ���� ��ο����� ���߱����� ī��Ʈ. 
	float accelX;
	float accelCoundX; 

	int clockFrame; // �ð�ȸ��. 
	int clockCurrentX; 

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void eventFunc(void);

	eventBridgeScene();
	~eventBridgeScene();
};

