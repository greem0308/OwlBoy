#pragma once

#include "singletonbase.h"

#include <vector>
#include <map>

class elements
{
public: // private을 퍼블릭으로 푼다음에.. 
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
	typedef map<string, elements> arrElement;    //키값 first,second으로  
	typedef map<string, elements>::iterator iterElement; //맵도 이터레이터 써서 

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

