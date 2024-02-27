#include<iostream>
#include"Exchange.h"
#include"csma.h"
#include"frame.h"

//两个线程互斥访问talbe
mutex Exchange::tablemtx;
//两个线程访问port时互斥 ，确保一个线程在端口1上接受时另一个线程不会向端口1发送数据
mutex Exchange::port1;
mutex Exchange::port2;
//交换表
vector<int*> Exchange::table;

//构造函数，设置端口
Exchange::Exchange(int tport) {
	out.lock();
	cout << "switcher ready" << endl;
	out.unlock();
	port = tport;
}
//print switch table
void Exchange::printtable() {
	for (int i = 0; i < table.size(); i++) {
		cout << "mac:" << table[i][0] << " port:" << table[i][1] << endl;
	}
}
//switcher work
void Exchange::exchange() {
	int flag;//if item found in table
	vector<int> jam_data;//jam_data in collision area
	//initialize jam_data all 1
	for (int i = 0; i < 32; i++) {
		jam_data.push_back(1);
	}
	//always at working
	while (true) {
		//received data from line
		vector<int> received_data;
		//decide which data to get by self port
		if (port == 1) {//get data from line connected to port1
			Exchange::port1.lock();
			received_data = transmitting_data1;
			Exchange::port1.unlock();
		}
		else {
			Exchange::port2.lock();
			received_data = transmitting_data2;
			Exchange::port2.unlock();
		}
		    std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//if there is data on line
		if (!received_data.empty()) {
			//if not jam warning
			if (received_data != jam_data) {
				//if is damaged
				if (!odd_check(received_data)) {
					out.lock();
					//terminal
					cout << "switch oddcheck frame is wrong! abandon" << endl;
					cout << endl;
					out.unlock();
				}
				else {//oddcheck right
					//reverse frame
					vector<int>reversed_data = reverseframe(received_data);
					//get receiver in frame
					int receiver = 0;
					receiver += reversed_data[8] * 4;
					receiver += reversed_data[9] * 2;
					receiver += reversed_data[10] * 1;
					//get source in frame
					int sender = 0;
					sender += reversed_data[11] * 4;
					sender += reversed_data[12] * 2;
					sender += reversed_data[13] * 1;

					tablemtx.lock();
					//first get data from other host   table empty now
					if (Exchange::table.size() == 0) {					
						int b[2];
						b[0] = sender;
						b[1] = port;
						//learn
						Exchange::table.push_back(b);
						out.lock();
						cout << "未知主机 添加PC"<<sender<<"进表" << endl;
						printtable();
						out.unlock();						
					}
					tablemtx.unlock();
					//search table
					flag = 0;
					tablemtx.lock();
					for (int i = 0; i < Exchange::table.size(); i++)
					{
						int* a = table[i];						
						//如果发送者在表内 if source in table
						if (a[0] == sender) {
							flag = 1;
							out.lock();
							cout << "switch receive data from PC" << sender<<endl;
							printtable();
							out.unlock();	
						}
					}
						//不在表内 learn
					if(!flag){
						int* b=new int[2];
						b[0] = sender;
						b[1] = port;
						table.push_back(b);
						out.lock();
						cout << "未知主机 添加PC" << sender << "进表" << endl;
						printtable();
						out.unlock();
					}
					tablemtx.unlock();

					//search table
					flag = 0;
					tablemtx.lock();
					for (int i = 0; i < Exchange::table.size(); i++)
					{
						int* a = table[i];
						//如果接收者在表内
						if (a[0] == receiver) {
							flag = 1;
							int tport = a[1];//接收者的port
							//如果接受者和发送者同一端口 ignore
							//port 为此swtich监听的端口
							if (tport == port) {								
								out.lock();
								cout << "接收者和发送者同一端口" << port << " 不转发" << endl;
								printtable();
								out.unlock();
							}
							//否则将数据送往另一端口(接收者的端口)
							else {
								if (port == 1) {
									Exchange::port2.lock();
									transmitting_data2 = received_data;
									Exchange::port2.unlock();
									out.lock();
									cout << "转发 port1 to port2 " << endl;
									printtable();
									out.unlock();
								}
								else {
									Exchange::port1.lock();
									transmitting_data1 = received_data;
									Exchange::port1.unlock();
									out.lock();
									cout << "转发 port2 to port1 " << endl;
									printtable();
									out.unlock();
								}
							}
							
						}
					}
					
						//接收者不在表内，扩散到其他端口
					if(!flag)
					{
						//port1端口则扩散到2口
						if (port == 1) {
							Exchange::port2.lock();
							transmitting_data2 = received_data;
							Exchange::port2.unlock();
							out.lock();
							cout << "接收者"<<receiver<<"不在表内，扩散" << endl;
							printtable();
							out.unlock();
						}
						else {
							Exchange::port1.lock();
							transmitting_data1 = received_data;
							Exchange::port1.unlock();
							out.lock();
							cout << "接收者" << receiver << "不在表内，扩散" << endl;
							printtable();
							out.unlock();
						}						
					}
					tablemtx.unlock();					
				}
			}					
			//if jam data abandon
			else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
				received_data.clear();
			}
		}
		else {
			//persist on listening
		}
	}
}