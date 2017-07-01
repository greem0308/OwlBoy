#pragma once
#include "gameNode.h"
#include "button.h"

class startScene : public gameNode
{
public:
	button* startBtn;
	
	// 화면 돌릴때 필요함.
	enum DIREC
	{
		RIGHT,LEFT
	};

	int frameCount;
	int currentX;
	int startDirec;

	//또다른 랜트창들이 뜨고, 데이터베이스랑 연동되는 볼륨 변수가 있을것임. 
	// 근데 모든 곳에서 다 열려야되니까... 
	// 이 스트럭트를 전부 다 써줘야하나.. 콜백함수로 만들어야할까..? 
	// 콜백쓰면 되겠다. 콜백에 이미지도 들어가게..? 클래스를 상속시켜야하나..

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void rotate(void);

	static void cbStartBtn(void);

	startScene();
	~startScene();
};

