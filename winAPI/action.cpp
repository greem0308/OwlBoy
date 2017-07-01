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

	_rewind = false; //�� ������..

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

	count++; // �������� ���� ī��Ʈ

	float elpasedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = (elpasedTime / _time) * _travelRange;
	//�ʴ� _travelRange��ŭ �̵��Ѵ�...

	// ������
	//http://autosave.tistory.com/57 ����. 
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

	//�̵�...
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
		//�̹��� �ּҰ� �޾ƿ´�
		_image = img;

		//�̵� ��������
		_startX = img->getX();
		_startY = img->getY();

		//���� ����
		_endX = endX;
		_endY = endY;

		//�̵��Ÿ� ���Ѵ�
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//���� �����ش�
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//���� Ÿ�� �����Ѵ�
		_worldTimeCount = TIMEMANAGER->getWorldTime();
	
		//�̵��ð�
		_time = time;

		_isMoving = true;
	}
}

// ���� �߰��� �����ε� �ݹ��Լ�
void action::moveTo(image* img, float endX, float endY, float time, CALLBACK_FUNCTION cbFuntion)
{
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFuntion);
	moveTo(img,endX,endY,time);
}

// �����ε� �ǵ��ƿ��� �Լ�. 
void action::moveTo(image* img, float endX, float endY, float time, bool rewind)
{
	if (!_isMoving )
	{
		_rewind = rewind;

		//�̹��� �ּҰ� �޾ƿ´�
		_image = img;

		//�̵� ��������
		_startX = img->getX();
		_startY = img->getY();

		//���� ����
		_endX = endX;
		_endY = endY;

		//�̵��Ÿ� ���Ѵ�
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//���� �����ش�
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//���� Ÿ�� �����Ѵ�
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//�̵��ð�
		_time = time;

		_isMoving = true;
	}
}

//�⺻ ������ � �Լ�
void action::moveToBezier(image* img, float endX,float endY, float time)
{
	if (!_isMoving && !_bezier)
	{
		//�̹��� �ּҰ� �޾ƿ´�
		_image = img;


		//�̵� ��������
		_startX = img->getX();
		_startY = img->getY();

		//���� ����
		_endX = endX;
		_endY = endY;

		//�̵� ��������
		P0x = _startX;
		P0y = _startY;

		// ����° ����. 
		P1x = _startX;
		P1y = _startY - (_endX - _startX);

		//���� ����
		P2x = endX;
		P2y = _startY - (_endX - _startX);

		//���� ����
		P3x = _endX;
		P3y = _endY;

		float elpasedTime = TIMEMANAGER->getElapsedTime();

		//���� Ÿ�� �����Ѵ�
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//�̵��ð�
		_time = time;

		_isMoving = true;
		_bezier = true;
	}

}


// ��� ������ ��� �������ִ� �Լ�. 
void action::moveToBezier(image* img, float twoX,float twoY, float threeX, float threeY, float endX, float endY, float time)
{
	if (!_isMoving && !_bezier)
	{
		//�̹��� �ּҰ� �޾ƿ´�
		_image = img;

		//�̵� ��������
		_startX = img->getX();
		_startY = img->getY();

		//���� ����
		_endX = endX;
		_endY = endY;

		//�̵� ��������
		P0x = _startX;
		P0y = _startY;
		
		// �ι�° ����. 
		P1x = twoX;
		P1y = twoY;

		//����° ����
		P2x = threeX;
		P2y = threeY;

		//���� ����
		P3x = _endX;
		P3y = _endY;

		float elpasedTime = TIMEMANAGER->getElapsedTime();

		//���� Ÿ�� �����Ѵ�
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		//�̵��ð�
		_time = time;

		_isMoving = true;
		_bezier = true;
	}

}