#pragma once

#include "gamenode.h"

//=====================================================
//############ 2017.06.22 헤더파일 action #############
//=====================================================

typedef void(*CALLBACK_FUNCTION)(void);

class action : public gameNode
{
private:
	image* _image;			//해당 이미지...
	float _startX, _startY; //시작 x, y
	float _endX, _endY;		//도착 x, y
	float P0x, P0y;
	float P1x, P1y;
	float P2x,P2y;
	float P3x, P3y;

	float _angle;			//각도
	float _travelRange;		//이동거리
	float _worldTimeCount;	//월드타임
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
	bool _rewind; //맨 앞으로..
	int count;

	CALLBACK_FUNCTION _callbackFunction;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void move(void);

	virtual void moveTo(image* img, float endX, float endY, float time);

	// 되돌아오는 함수
	virtual void moveTo(image* img, float endX, float endY, float time,bool rewind);

	// 기존의 함수에 오버로딩 해서 콜백함수를 추가했다. 
	virtual void moveTo(image* img, float endX, float endY, float time, CALLBACK_FUNCTION cbFuntion);

	// Bezier 
	virtual void moveToBezier(image* img, float endX, float endY, float time);

	// Bezier make
	virtual void moveToBezier(image* img, float twoX, float twoY, float threeX, float threeY, float endX, float endY, float time);

	bool getIsMoving(void) { return _isMoving; }

	action(void);
	virtual ~action(void);
};

