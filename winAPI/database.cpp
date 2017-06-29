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

//������ �ε�..
void database::loadDatabase(void)
{
	arrElements vTemp;
	vTemp = TXTMANAGER->load("database.txt");

	elements ele; // �����̰� //Ŭ���� ���� ���� 

	string str;

	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			str = vTemp[i + 1];

			// stl ��.
			// ���� ��� :  map<�ε�������,����������> ������̸�;
			// ù ��° ��Ҵ� first�ν� �ε����̰�, �� ��° ��Ҵ� second�ν� �������̴�. 

			// ���ε�, insert�� ������ Ǫ�ù� ���� ����(Ű��(��Ʈ������ �ִ°� ����), �����尪. )
			_mTotalElement.insert(make_pair(vTemp[i + 1], ele));

			ele.x = atoi(vTemp[i + 2].c_str());
			ele.y = atoi(vTemp[i + 3].c_str());
			ele.hp = atoi(vTemp[i + 4].c_str());
			ele.coin = atoi(vTemp[i + 5].c_str());

			//arrElements vElement;

			iterElement mIter = _mTotalElement.find(str);
			mIter->second = ele;  //���ͷ� �Ǿ��ִ��� Ŭ������.. 
			continue;
		}
		// mIter->second.push_back(vTemp[i]);
	}
	//_mTotalElement;
	vTemp.clear(); //������ ���Ƽ� 
}
