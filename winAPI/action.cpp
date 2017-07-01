#include "StdAfx.h"
#include "action.h"

action::action(void)
	:
	_callbackFunction(NULL)
{
}

action::~action(void)
{
}

HRESULT action::init(void)
{
	gameNode::init();
	_worldTimeCount = 0.0f;
	_travelRange = 0.0f;
	_time = 0.0f;
	_angle=0.0f;
	_isMoving = false;
	_bezier = false;
	_startX = 0;
	_startY = 0;

	_rewind = false; //맨 앞으로..

	count = 0;

	P0x = 0;
	P0y = 0;
	P1x = 0;
	P1y = 0;
	P2x = 0;
	P2y = 0;
	P3x = 0;
	P3y = 0;

	Q0x = 0;
	Q0y = 0;

	Q1x=0;
	Q1y=0;

	Q2x=0;
	Q2y=0;

	
	R0x=0;
	R0y=0;

	R1x=0;
	R1y=0;

	Bx=0;
	By=0;

	return S_OK;
}

void action::release(void)
{
	gameNode::release();
}

void action::update(void)
{
	gameNode::update();

	move();
}

void action::render(void)
{
}

void action::move(void)
{
	if (!_isMoving) return;

	count++; // 베지어곡선을 위한 카운트

	float elpasedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = (elpasedTime / _time) * _travelRange;
	//초당 _travelRange만큼 이동한다...

	// 베지어
	//http://autosave.tistory.com/57 참고. 
	 Q0x = P0x + (P1x - P0x) * ((elpasedTime / _time)*count);
	 Q0y = P0y + (P1y - P0y) * ((elpasedTime / _time)*count);

	 Q1x = P1x + (P2x - P1x) * ((elpasedTime / _time)*count);
	 Q1y = P1y + (P2y - P1y) * ((elpasedTime / _time)*count);

	 Q2x = P2x + (P3x - P2x) * ((elpasedTime / _time)*count);
	 Q2y = P2y + (P3y - P2y) * ((elpasedTime / _time)*count);

	 R0x = Q0x + (Q1x - Q0x) * ((elpasedTime / _time)*count);
	 R0y = Q0y + (Q1y - Q0y) * ((elpasedTime / _time)*count);

	 R1x = Q1x + (Q2x - Q1x) * ((elpasedTime / _time)*count);
	 R1y = Q1y + (Q2y - Q1y) * ((elpasedTime / _time)*count);

	 Bx = R0x + (R1x - R0x)* ((elpasedTime / _time)*count);
	 By = R0y + (R1y - R0y)* ((elpasedTime / _time)*count);

	//이동...
	 if (_bezier)
	 {
		 _image->setX(Bx);
		 _image->setY(By);
	 }
	 else
	 {
		 _image->setX(_image->getX() + cosf(_angle) * moveSpeed);
		 _image->setY(_image->getY() + (-sinf(_angle)) * moveSpeed);
	 }

	float time = TIMEMANAGER->getWorldTime();
	if (_time + _worldTimeCount <= TIMEMANAGER->getWorldTime())
	{
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		if (_rewind)
		{
			_image->setX(_endX = _startX);
			_image->setY(_endY = _startY);
		}
		else 
		{
			_image->setX(_endX);
			_image->setY(_endY);
		}
		_isMoving = false;
		_bezier = false;
		_rewind = false;

		count = 0;
		if (_callbackFunction != NULL) _callbackFunction();
	}
}

void action::moveTo(image* img, float endX, float endY, float time)
{
	if (!_isMoving)
	{
		//이미지 주소값 받아온다
		_image = img;

		//이동 시작지점
		_startX = img->getX();
		_startY = img->getY();

		//도착 지점
		_endX = endX;
		_endY = endY;

		//이동거리 구한다
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//각도 구해준다
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//월드 타임 저장한다
		_worldTimeCount = TIMEMANAGER->getWorldTime();
	
		//이동시간
		_time = time;

		_isMoving = true;
	}
}

// 내가 추가한 오버로딩 콜백함수
void action::moveTo(image* img, float endX, float endY, float time, CALLBACK_FUNCTION cbFuntion)
{
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFuntion);
	moveTo(img,endX,endY,time);
}

// 오버로딩 되돌아오는 함수. 
void action::moveTo(image* img, float endX, float endY, float time, bool rewind)
{
	if (!_isMoving )
	{
		_rewind = rewind;

		//이미지 주소값 받아온다
		_image = img;

		//이동 시작지점
		_startX = img->getX();
		_startY = img->getY();

		//도착 지점
		_endX = endX;
		_endY = endY;

		//이동거리 구한다
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//각도 구해준다
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//월드 타임 저장한다
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//이동시간
		_time = time;

		_isMoving = true;
	}
}

//기본 베지어 곡선 함수
void action::moveToBezier(image* img, float endX,float endY, float time)
{
	if (!_isMoving && !_bezier)
	{
		//이미지 주소값 받아온다
		_image = img;


		//이동 시작지점
		_startX = img->getX();
		_startY = img->getY();

		//도착 지점
		_endX = endX;
		_endY = endY;

		//이동 시작지점
		P0x = _startX;
		P0y = _startY;

		// 세번째 지점. 
		P1x = _startX;
		P1y = _startY - (_endX - _startX);

		//도착 지점
		P2x = endX;
		P2y = _startY - (_endX - _startX);

		//도착 지점
		P3x = _endX;
		P3y = _endY;

		float elpasedTime = TIMEMANAGER->getElapsedTime();

		//월드 타임 저장한다
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//이동시간
		_time = time;

		_isMoving = true;
		_bezier = true;
	}

}


// 가운데 두점을 모두 설정해주는 함수. 
void action::moveToBezier(image* img, float twoX,float twoY, float threeX, float threeY, float endX, float endY, float time)
{
	if (!_isMoving && !_bezier)
	{
		//이미지 주소값 받아온다
		_image = img;

		//이동 시작지점
		_startX = img->getX();
		_startY = img->getY();

		//도착 지점
		_endX = endX;
		_endY = endY;

		//이동 시작지점
		P0x = _startX;
		P0y = _startY;
		
		// 두번째 지점. 
		P1x = twoX;
		P1y = twoY;

		//세번째 지점
		P2x = threeX;
		P2y = threeY;

		//도착 지점
		P3x = _endX;
		P3y = _endY;

		float elpasedTime = TIMEMANAGER->getElapsedTime();

		//월드 타임 저장한다
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//이동시간
		_time = time;

		_isMoving = true;
		_bezier = true;
	}

}