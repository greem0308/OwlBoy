#pragma once

#include "singletonbase.h"
#include <string>
#include <map>

class gameNode;

//=====================================================
//####### 2017.03.20 ������� sceneManager #############
//=====================================================

using namespace std;

class sceneManager : public singletonBase <sceneManager>
{
public:
	typedef map<string, gameNode*> mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene; //���� ��
	static gameNode* _readyScene; //��ü�غ����� ��

	mapSceneList _mSceneList; //������Ʈ

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�� �߰��Ѵ�
	gameNode* addScene(string sceneName, gameNode* scene);

	//�� ü����
	HRESULT changeScene(string sceneName);

	sceneManager(void) {};
	virtual ~sceneManager(void) {};

	//friend DWORD CALLBACK loadingThread(LPVOID prc);
};

