#pragma once
#include "gameNode.h"

class startScene : public gameNode
{
public:
	RECT startRC;

	struct tagSoundRC
	{
		RECT rc; //Ŭ���� rc
	};
	tagSoundRC soundRC;

	//�Ǵٸ� ��Ʈâ���� �߰�, �����ͺ��̽��� �����Ǵ� ���� ������ ��������. 
	// �ٵ� ��� ������ �� �����ߵǴϱ�... 
	// �� ��Ʈ��Ʈ�� ���� �� ������ϳ�.. �ݹ��Լ��� �������ұ�..? 
	// �ݹ龲�� �ǰڴ�. �ݹ鿡 �̹����� ����..? Ŭ������ ��ӽ��Ѿ��ϳ�..

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	startScene();
	~startScene();
};

