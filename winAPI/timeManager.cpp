#include "StdAfx.h"
#include "timeManager.h"

timeManager::timeManager(void)
{
}

timeManager::~timeManager(void)
{
}

//�ʱ�ȭ
HRESULT timeManager::init(void)
{
	//Ÿ�̸� ���� �� �ʱ�ȭ
	_timer = new timer;
	_timer->init();

	return S_OK;
}

//������
void timeManager::release(void)
{
	SAFE_DELETE(_timer);
}

//������ ����
void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

//�ش� HDC�� ���� ����Ѵ�
void timeManager::render(HDC hdc)
{
	char str[256];
	std::string strFrame;

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

#ifdef _DEBUG
	if (_timer != NULL)
	{
		//������ ���
		sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	
		//���� Ÿ�� ���
		sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		//���� Tick �� ���
		sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else
	if (_timer != NULL)
	{
		//������ ���
		sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif
}
