#pragma once
#include "enemy.h"

enum FlSTATE
{
	FL_NONE, FL_TURN, FL_WALK, FL_IDLE,
	FL_0A, FL_20A, Fl_40A, FL_60A, FL_80A, FL_90A, // �һմ� ����.
	FL_HURT, FL_DIE, FL_HURTDIE
};

class fireLion : public enemy
{
public:
	// �Դٰ��� 
	int moveFrameCount;
	bool moveRight;
	bool moveLeft;

	FlSTATE flState;

	// �ִϸ��̼�
	int frameCount;
	int currentX;

	// fire �ִϸ��̼�
	int fireFrameCount;
	int fireCurrentX;

	// ���ڿ� �÷��̾��� �ޱ�. ��Ƽ� �ҽ����.. 
	int lionAngle;

	// ������ ��Ʈ
	RECT hitRC;
	bool attack; 
	int attackFrameCount; //���� ��������?

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void frameFunc(void);
	virtual void fireFrameFunc(void);

	fireLion();
	~fireLion();
};

