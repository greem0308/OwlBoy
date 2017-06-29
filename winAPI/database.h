#pragma once

#include "singletonbase.h"

#include <vector>
#include <map>

class elements
{
public: // private�� �ۺ����� Ǭ������.. 
	const char* name;
	float x;
	float y;
	int hp;
	int coin;

	elements(void) {};
	~elements(void) {};
};

class database : public singletonBase <database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;
	typedef map<string, elements> arrElement;    //Ű�� first,second����  
	typedef map<string, elements>::iterator iterElement; //�ʵ� ���ͷ����� �Ἥ 

private:
	arrElement _mTotalElement;

public:
	HRESULT init(void);
	void release(void);

	void loadDatabase(void);

	elements* getElementData(string str)
	{
		return &_mTotalElement.find(str)->second;
	}

	database(void);
	~database(void);
};

