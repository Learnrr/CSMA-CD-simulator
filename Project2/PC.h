#pragma once
#ifndef   PC_H       
#define   PC_H      
#include<iostream>
#include <chrono>
#include<vector>
#include<queue>
#define HOST_NUMBER 5
#define HOST_LISTEN 0
#define HOST_SEND 1
#define HOST_WAIT 2
using namespace std;

class PC {
public:
	//���캯����ͨ��Ψһ��MAC����host
	PC(int MAC,int ReceiveBufferSize,int);
	//���֡�ṹ
	void get_send_frame();
	//����֡��ѭCSMA/CDЭ��
	void listen_and_send();
	//����֡
	void receive_frame();
	//���֡
	void decode_frame();
	//������ǰ״̬ listen 0 send 1 wait 2
	int status;
	//Ҫ���͵�֡
	vector<int> send_frame;
	//Ҫ���͵�����
	vector<int> send_data;
	//ָ���Ľ�����
	int receiver;
	//��ͻ����
	int collision_count;
	//MAC��ַ
	int mac;
	//���ջ�����
	queue<vector<int>> receive_buffer;
	//���ջ�������С
	int receive_buffer_size;
	//port
	int switch_port;
};

#endif 





