#pragma once

#include "gamenode.h"
#include "test.h"

class gameStudy : public gameNode
{
private:
	
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);


	gameStudy(void);
	virtual ~gameStudy(void);
};

