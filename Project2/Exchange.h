#pragma once
#ifndef EXCHANGE_H
#define EXCHANGE_H


#include<vector>
#include<map>
#include<mutex>
using namespace std;
//交换机，每一个对象控制交换机上的一个端口，所以交换表需要设为静态成员,所有对象共享一份拷贝。
//运行时每一个端口占一个线程，mutex port确保两个端口共同访问同一总线时互斥
//同时两个端口需要共同的总线mutex，设为static
class Exchange {
public:
	Exchange(int);
	vector<int> data1;//与总线一交换的数据
	vector<int> data2;//与总线二交换的数据

	static vector<int*> table;//交换表 <mac,port>
	void exchange();//转发
	void printtable();
	//对象所控制的端口
	int port;
	//交换表
	static mutex tablemtx;
	static mutex port1;//互斥访问一号总线
	static mutex port2;//互斥访问二号总线

};
#endif