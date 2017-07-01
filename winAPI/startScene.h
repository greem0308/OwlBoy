#pragma once
#include "gameNode.h"
#include "button.h"

class startScene : public gameNode
{
public:
	button* startBtn;
	
	// ȭ�� ������ �ʿ���.
	enum DIREC
	{
		RIGHT,LEFT
	};

	int frameCount;
	int currentX;
	int startDirec;

	//�Ǵٸ� ��Ʈâ���� �߰�, �����ͺ��̽��� �����Ǵ� ���� ������ ��������. 
	// �ٵ� ��� ������ �� �����ߵǴϱ�... 
	// �� ��Ʈ��Ʈ�� ���� �� ������ϳ�.. �ݹ��Լ��� �������ұ�..? 
	// �ݹ龲�� �ǰڴ�. �ݹ鿡 �̹����� ����..? Ŭ������ ��ӽ��Ѿ��ϳ�..

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void rotate(void);

	static void cbStartBtn(void);

	startScene();
	~startScene();
};

