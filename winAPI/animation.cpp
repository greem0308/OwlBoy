#include "StdAfx.h"
#include "animation.h"

animation::animation(void)
	:_frameWidth(0),
	_frameHeight(0),
	_loop(false),
	_frameUpdateSec(0.0f),
	_elapsedSec(0.0f),
	_nowPlayIdx(0),
	_isPlay(false)
{
}

animation::~animation(void)
{
}

//�ʱ�ȭ
HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//���� ������ �� ���Ѵ�
	_frameWidth = frameW;
	int frameNumWidth = totalW / _frameWidth;

	//���� ������ �� ���Ѵ�
	_frameHeight = frameH;
	int frameNumHeight = totalH / _frameHeight;

	//�� ������ ���� ���Ѵ�
	_frameNum = frameNumWidth * frameNumHeight;

	_frameList.clear();
	//������ ��ġ ����Ʈ ����
	for (int i = 0; i < frameNumHeight; i++)
	{
		for (int j = 0; j < frameNumWidth; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			//������ ��ġ ����Ʈ�� �߰��Ѵ�
			_frameList.push_back(framePos);
		}
	}

	//�⺻���� ����Ʈ��...
	setDefPlayFrame();
		
	return S_OK;
}

void animation::release(void)
{
}

//ó������ ������ �ִϸ��̼�... (����Ʈ ��)
void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	//�Դٸ� ���ٸ���?
	if (reverse)
	{
		//������?
		if (loop)
		{
			//���� ������..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//�ö� ������
			for (int i = _frameNum - 2; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			//���� ������..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//�ö� ������
			for (int i = _frameNum - 2; i >= 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		//���� ������..
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}
}

//���ϴ� �ε��� �ִϸ��̼�...
void animation::setPlayFrame(int* arr, int arrLen, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(arr[i]);
	}
}

//���� �ִϸ��̼�...
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	//���۰� ���� ������.......
	if (start == end)
	{
		_playList.push_back(start);
		return;
	}

	//������������ ���������Ӻ��� ū ���
	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i <= start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	//��ŸƮ < ����
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i > start; i--)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i <= end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}

//�ʴ� ���� Ƚ����...
void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

//������ ������Ʈ...
void animation::frameUpdate(float elapsedTime)
{
	if (_isPlay)
	{
		_elapsedSec += elapsedTime;

		//������ ������Ʈ �ð��� �Ǿ�����....
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;
			if (_nowPlayIdx == _playList.size())
			{
				//�����̳�?
				if (_loop)
				{
					_nowPlayIdx = 0;
				}
				else
				{
					_nowPlayIdx--;
					_isPlay = false;
				}
			}
		}
	}
}

//�ִϸ��̼� ����...
void animation::start(void)
{
 	if (_isPlay) return;
	_isPlay = true;
	_nowPlayIdx = 0;
}

//�ִϸ��̼� ����...
void animation::stop(void)
{
	_isPlay = false;
	_nowPlayIdx = 0;
}

//�ִϸ��̼� �Ͻ� ����...
void animation::pause(void)
{
	_isPlay = false;
}

//�ִϸ��̼� �Ͻ��������� �ٽ� ����.....
void animation::resume(void)
{
	_isPlay = true;
}