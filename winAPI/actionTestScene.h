#pragma once

#include "gamenode.h"
#include "action.h"

class actionTestScene : public gameNode
{
private:
	image* _image;
	action* _action1;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	static void callback(void);

	actionTestScene(void);
	~actionTestScene(void);
};

