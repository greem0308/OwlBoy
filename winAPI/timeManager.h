#pragma once

#include "singletonbase.h"
#include "timer.h"

//====================================================
//####### 2017.05.01 ������� image class #############
//====================================================
class timeManager : public singletonBase <timeManager>
{
private:
	timer* _timer;

public:
	//�ʱ�ȭ
	HRESULT init(void);

	//������
	void release(void);

	//������ ����
	void update(float lock = 0.0f);

	//�ش� HDC�� ���� ����Ѵ�
	void render(HDC hdc);

	//��ü �ð� �����´�
	inline float getWorldTime(void) const {return _timer->getWorldTime();}

	//������ ����ð�
	inline float getElapsedTime(void) const {return _timer->getElapsedTime();}

	timeManager(void);
	~timeManager(void);
};

