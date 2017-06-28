#include "StdAfx.h"
#include "keyManager.h"


keyManager::keyManager(void)
{
}


keyManager::~keyManager(void)
{
}

HRESULT keyManager::init(void)
{
	//�ʱ�ȭ
	for (int i = 0; i < KEYMAX; i++)
	{
		this->setKeyUp(i, false);
		this->setKeyDown(i, false);
	}

	return S_OK;
}

void keyManager::release(void)
{
	
}

//�ѹ��� ��������... (�ѹ�!)
bool keyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeyDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

//��������... (�ѹ�!!)
bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else 
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}
	return false;
}

//��� ������...
bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//���~~~~~ ����ġ.... ���� 0 ���� 1 ���� 0 �ݺ�....
bool keyManager::isToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}
