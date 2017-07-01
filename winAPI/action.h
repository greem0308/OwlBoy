#pragma once

#include "gamenode.h"

//=====================================================
//############ 2017.06.22 ������� action #############
//=====================================================

typedef void(*CALLBACK_FUNCTION)(void);

class action : public gameNode
{
private:
	image* _image;			//�ش� �̹���...
	float _startX, _startY; //���� x, y
	float _endX, _endY;		//���� x, y
	float P0x, P0y;
	float P1x, P1y;
	float P2x,P2y;
	float P3x, P3y;

	float _angle;			//����
	float _travelRange;		//�̵��Ÿ�
	float _worldTimeCount;	//����Ÿ��
	float _time;

	float Q0x;
	float Q0y;

	float Q1x;
	float Q1y;

		float Q2x;
		float Q2y;

		//R0 = Q0
		float R0x;
		float R0y;

		float R1x;
		float R1y;

		float Bx;;
		float By;;

	bool _isMoving;
	bool _bezier;
	bool _rewind; //�� ������..
	int count;

	CALLBACK_FUNCTION _callbackFunction;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void move(void);

	virtual void moveTo(image* img, float endX, float endY, float time);

	// �ǵ��ƿ��� �Լ�
	virtual void moveTo(image* img, float endX, float endY, float time,bool rewind);

	// ������ �Լ��� �����ε� �ؼ� �ݹ��Լ��� �߰��ߴ�. 
	virtual void moveTo(image* img, float endX, float endY, float time, CALLBACK_FUNCTION cbFuntion);

	// Bezier 
	virtual void moveToBezier(image* img, float endX, float endY, float time);

	// Bezier make
	virtual void moveToBezier(image* img, float twoX, float twoY, float threeX, float threeY, float endX, float endY, float time);

	bool getIsMoving(void) { return _isMoving; }

	action(void);
	virtual ~action(void);
};

