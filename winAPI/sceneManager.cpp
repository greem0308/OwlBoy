#include "StdAfx.h"
#include "sceneManager.h"
#include "gameNode.h"

gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_readyScene = NULL;

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void sceneManager::release(void)
{
	//�ش�Ű �˻��ϰ�..
	mapSceneIter iter = _mSceneList.begin();

	//�� ��ü�� ���鼭 �����ش�
	for (; iter != _mSceneList.end();)
	{
		//�������� �ݺ��� ������Ű�� �ʴ´�
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else 
		{
			//�ݺ��� ����..
			++iter;
		}
	}

	_mSceneList.clear();
}

void sceneManager::update(void)
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render(void)
{
	if (_currentScene) _currentScene->render();
}

//�� �߰��Ѵ�
gameNode* sceneManager::addScene(string sceneName, gameNode* scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

//�� ü����
HRESULT sceneManager::changeScene(string sceneName)
{
	//�ٲ� �� ã��...
	mapSceneIter find = _mSceneList.find(sceneName);

	//��ã���� �޽�
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲ� �� �ʱ�ȭ
	if (SUCCEEDED(find->second->init()))
	{
		//���� ���� ������ ������
		if (_currentScene) _currentScene->release();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}
