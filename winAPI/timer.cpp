#include "StdAfx.h"
#include "timer.h"
#include <MMSystem.h>

//TimeGetTime �Լ��� ����ϱ� ���� ���̺귯��!!!
#pragma comment(lib, "winmm.lib")

//�ʱ�ȭ
HRESULT timer::init(void)
{
	//Ÿ�̸�(����?) ������ �Ǵ��� üũ�Ѵ�
	//������ �����ϸ� _periodFrequency ���� �ʴ� �ľ��� �� �ִ�
	//�ð��� ���� ���´�!!
	//�ʴ� �и����� ���� ��!!
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		//�ʴ� �ð��� ����� �� �ִ� ��ġ
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	_frameRate = 0;		
	_FPSFrameCount = 0;	
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0.0f;		

	return S_OK;
}

//���� �ð��� ���
void timer::tick(float lockFPS)
{
	//������ �Ǹ� ���� �ð��� ���´�
	if (_isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else 
	{
		//�������� ������ time �Լ��� �̿�!!
		_curTime = timeGetTime();
	}

	//������ �ð��� ���� �ð��� ������� �����Ѵ�!!!!!!!
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//���� ������ ó�����ش� (�������� �����ӽ�Ű��)
	if (lockFPS >= 0.0f)
	{
		//���� �������� �ð��� �����Ҷ� ���� ���� ����!!
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			//������ �ð��� ���� �ð��� ������� �����Ѵ�!!!!!!!
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	//������ �ð��� ����Ѵ�
	_lastTime = _curTime;

	//�ʴ� ������ ī��Ʈ ������Ų��
	_FPSFrameCount++;

	//�ʴ� ������ �ð� ����� ����
	_FPSTimeElapsed += _timeElapsed;

	//��ü �ð� ����� ����!!
	_worldTime += _timeElapsed;

	//������ �ʱ�ȭ (1�ʸ���!!)
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

//���� FPS
unsigned long timer::getFrameRate(char* str) const
{
	if (str != NULL)
	{
		sprintf(str, "FPS : %d", _frameRate);
	}
	return _frameRate;
}
