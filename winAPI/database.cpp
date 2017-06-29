#include "StdAfx.h"
#include "database.h"


database::database(void)
{
}

database::~database(void)
{
}

HRESULT database::init(void)
{
	loadDatabase();

	return S_OK;
}

void database::release(void)
{

}

//데이터 로드..
void database::loadDatabase(void)
{
	arrElements vTemp;
	vTemp = TXTMANAGER->load("database.txt");

	elements ele; // 빈컵이고 //클래스 변수 선언 

	string str;

	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			str = vTemp[i + 1];

			// stl 맵.
			// 선언 방법 :  map<인덱스형식,데이터형식> 사용할이름;
			// 첫 번째 요소는 first로써 인덱스이고, 두 번째 요소는 second로써 데이터이다. 

			// 맵인데, insert는 백터의 푸시백 같은 역할(키값(스트링으로 넣는게 좋음), 세컨드값. )
			_mTotalElement.insert(make_pair(vTemp[i + 1], ele));

			ele.x = atoi(vTemp[i + 2].c_str());
			ele.y = atoi(vTemp[i + 3].c_str());
			ele.hp = atoi(vTemp[i + 4].c_str());
			ele.coin = atoi(vTemp[i + 5].c_str());

			//arrElements vElement;

			iterElement mIter = _mTotalElement.find(str);
			mIter->second = ele;  //벡터로 되어있던걸 클래스로.. 
			continue;
		}
		// mIter->second.push_back(vTemp[i]);
	}
	//_mTotalElement;
	vTemp.clear(); //데이터 남아서 
}
