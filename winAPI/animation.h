#pragma once

#include <vector>

//=====================================================
//########## 2017.03.20 ������� animation #############
//=====================================================

class animation
{
public:
	typedef std::vector<POINT> vFrameList; //������ ����Ʈ
	typedef std::vector<int> vPlayList;	//��������� ����Ʈ
	
private:
	vFrameList _frameList;
	vPlayList _playList;

	int _frameWidth;		//������ ����ũ��
	int _frameHeight;		//������ ����ũ��

	int _frameNum;			//������ ����

	bool _loop;				//�ݺ�?
	float _frameUpdateSec;	//������ ������Ʈ �ð�
	float _elapsedSec;		//������ �ð��� ���� �ֱ�

	DWORD _nowPlayIdx;		//���� �ִϸ��̼� �ε���
	bool _isPlay;			//�����?

public:
	//�ʱ�ȭ
	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release(void);

	//ó������ ������ �ִϸ��̼�... (����Ʈ ��)
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);

	//���ϴ� �ε��� �ִϸ��̼�...
	void setPlayFrame(int* arr, int arrLen, BOOL loop = FALSE);

	//���� �ִϸ��̼�...
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);

	//�ʴ� ���� Ƚ����...
	void setFPS(int framePerSec);

	//������ ������Ʈ...
	void frameUpdate(float elapsedTime);

	void start(void);		//�ִ� ����
	void stop(void);		//�ִ� ����
	void pause(void);		//�ִ� �Ͻ� ���� 
	void resume(void);		//�ִ� �ٽ� ����

	//�÷��̳�?
	inline BOOL isPlay(void) {return _isPlay;}

	//������ ��ġ ���´�
	inline POINT getFramePos(void) {return _frameList[_playList[_nowPlayIdx]];}

	//������ ���� ũ�� ���´�
	inline int getFrameWidth(void) {return _frameWidth;}

	//������ ���� ũ�� ���´�
	inline int getFrameHeight(void) {return _frameHeight;}


	animation(void);
	~animation(void);
};

