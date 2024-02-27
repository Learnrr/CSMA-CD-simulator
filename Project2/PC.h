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
	//构造函数，通过唯一的MAC建立host
	PC(int MAC,int ReceiveBufferSize,int);
	//打包帧结构
	void get_send_frame();
	//发送帧遵循CSMA/CD协议
	void listen_and_send();
	//接收帧
	void receive_frame();
	//解包帧
	void decode_frame();
	//主机当前状态 listen 0 send 1 wait 2
	int status;
	//要发送的帧
	vector<int> send_frame;
	//要发送的数据
	vector<int> send_data;
	//指定的接收者
	int receiver;
	//冲突次数
	int collision_count;
	//MAC地址
	int mac;
	//接收缓冲区
	queue<vector<int>> receive_buffer;
	//接收缓冲区大小
	int receive_buffer_size;
	//port
	int switch_port;
};

#endif 





