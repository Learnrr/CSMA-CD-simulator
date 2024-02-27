#include"PC.h"
#include<thread>
#include"Exchange.h"
using namespace std;
//if write log file
extern int choice;

//5 hosts and a switch which has 2 ports
//each host has 3 threads, listen&send frame  receive frame  decode frame
//switch has 2 ports; each port runs on a thread
int main() {
	
	cout << "if write log yes(1) OR no(0)" << endl;	
	cin >> choice;

	PC PC0(0,3,1);
	PC PC1(1,3,1);
	PC PC2(2,3,1);
	PC PC3(3,3,2);
	PC PC4(4,3,2);
	Exchange switcher1(1);
	Exchange switcher2(2);

	//warning use 'ref' to ensure unique object
	thread port1(&Exchange::exchange, ref(switcher1));
	thread port2(&Exchange::exchange, ref(switcher2));
	//每台主机三个线程 发送帧 接受帧 解包帧
	thread sender0(&PC::listen_and_send,ref(PC0));
	thread receiver0(&PC::receive_frame, ref(PC0));
	thread decoder0(&PC::decode_frame, ref(PC0));

	thread sender1(&PC::listen_and_send, ref(PC1));
	thread receiver1(&PC::receive_frame, ref(PC1));
	thread decoder1(&PC::decode_frame, ref(PC1));

	thread sender2(&PC::listen_and_send, ref(PC2));
	thread receiver2(&PC::receive_frame,ref(PC2));
	thread decoder2(&PC::decode_frame, ref(PC2));

	thread sender3(&PC::listen_and_send, ref(PC3));
	thread receiver3(&PC::receive_frame, ref(PC3));
	thread decoder3(&PC::decode_frame, ref(PC3));

	thread sender4(&PC::listen_and_send, ref(PC4));	
	thread receiver4(&PC::receive_frame, ref(PC4));
	thread decoder4(&PC::decode_frame, ref(PC4));

	port1.join();
	port2.join();
	decoder0.join();
	decoder1.join();
	decoder2.join();
	decoder3.join();
	decoder4.join();
	receiver0.join();
	receiver1.join();
	receiver2.join();
	receiver4.join();
	receiver3.join();
	sender0.join();	
	sender1.join();
	sender2.join();
	sender3.join();
	sender4.join();
}