#pragma once
#ifndef EXCHANGE_H
#define EXCHANGE_H


#include<vector>
#include<map>
#include<mutex>
using namespace std;
//��������ÿһ��������ƽ������ϵ�һ���˿ڣ����Խ�������Ҫ��Ϊ��̬��Ա,���ж�����һ�ݿ�����
//����ʱÿһ���˿�ռһ���̣߳�mutex portȷ�������˿ڹ�ͬ����ͬһ����ʱ����
//ͬʱ�����˿���Ҫ��ͬ������mutex����Ϊstatic
class Exchange {
public:
	Exchange(int);
	vector<int> data1;//������һ����������
	vector<int> data2;//�����߶�����������

	static vector<int*> table;//������ <mac,port>
	void exchange();//ת��
	void printtable();
	//���������ƵĶ˿�
	int port;
	//������
	static mutex tablemtx;
	static mutex port1;//�������һ������
	static mutex port2;//������ʶ�������

};
#endif