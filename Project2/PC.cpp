#include"PC.h"
#include"csma.h"
#include"frame.h"
#include<fstream>
using namespace std;
int choice;
//ensure consistence in transmission info
mutex one;
mutex two;
//ensure complete out info 
mutex out;
//data on transmission line
vector<int> transmitting_data1;
//free 0 busy 1 jam 2;
int transmission_status1;

vector<int> transmitting_data2;
int transmission_status2;
//receive buffer write and read mutex
mutex buffer[5];
PC::PC(int MAC,int ReceiveBufferSize,int port) {
	out.lock();
	cout << "PC" << MAC << " ready" << endl;
	out.unlock();
	mac = MAC;
	receive_buffer_size = ReceiveBufferSize;
	//get_send_frame();
	switch_port = port;
	collision_count = 0;
	status = HOST_LISTEN;
}
void  PC::listen_and_send() {
	status = 0;
	//define jam data
	vector<int> jam_data; 
	for (int i = 0; i < 32; i++) {
		jam_data.push_back(1);
	}
	ofstream fs1,fs2;
	if (choice) {
	string success_filename = "send_log"+ to_string(mac) + ".txt";
	string error_filename = "error_log" + to_string(mac) + ".txt";
	
		fs1.open(success_filename, ios::out);
		fs2.open(error_filename, ios::out);
	}
	while (true) {
		//Listening
		status = HOST_LISTEN;
		vector<int> transmitting_data;
		if (switch_port == 1)
			transmitting_data = transmitting_data1;
		else
			transmitting_data = transmitting_data2;
		if (transmitting_data.empty()) {
			status = HOST_SEND;
			//sending
			std::this_thread::sleep_for(std::chrono::milliseconds(10));	//simulate transmitting time
			//package frame
			get_send_frame();
			if (switch_port == 1) {
				one.lock();
				transmitting_data1 = send_frame;
				transmission_status1 = 1;
				one.unlock();
			}
			else {
				two.lock();
				transmitting_data2 = send_frame;
				transmission_status2 = 1;
				two.unlock();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));//simulate transmitting time

			if (switch_port == 1)
				transmitting_data = transmitting_data1;
			else
				transmitting_data = transmitting_data2;
			//success
			if (transmitting_data == send_frame) {

				
				//file
				if (choice) {
					out.lock();
					fs1 << "PC" << mac << " send out successfully" << endl;
					for (int i = 0; i < send_data.size(); i++)
						fs1 << send_data[i];
					fs1 << " from PC" << mac << " to PC" << receiver << endl;
					fs1 << endl;
					out.unlock();
				}
				//terminal
				else {
					out.lock();
					cout << "PC" << mac << " send out successfully" << endl;
					for (int i = 0; i < send_data.size(); i++)
						cout << send_data[i];
					cout << " from PC" << mac << " to PC" << receiver << endl;
					cout << endl;
					out.unlock();
				}
				
				
				//recover
				if (switch_port == 1) {
					one.lock();
					transmitting_data1.clear();
					transmission_status1 = 0;
					collision_count = 0;
					one.unlock();
				}
				else {
					two.lock();
					transmitting_data2.clear();
					transmission_status2 = 0;
					collision_count = 0;
					two.unlock();
				}
			}
			//collision
			else {
				status = HOST_WAIT;
				if (switch_port == 1) {
					one.lock();
					transmission_status1 = 2;//jam
					transmitting_data1 = jam_data;
					one.unlock();
				}
				else {
					two.lock();
					transmission_status2 = 2;//jam
					transmitting_data2 = jam_data;
					two.unlock();
				}
				collision_count++;
				
				if (choice) {
					//file
					out.lock();
					fs2 << "PC" << mac << " detected collision " << "\n";
					fs2 << "\n";
					out.unlock();
				}
				else {
					out.lock();
					//teminal
					cout << "PC" << mac << " detected collision " << endl;
					cout << endl;
					out.unlock();
				}
				
				if (switch_port == 1) {
					//recover
					one.lock();
					transmitting_data1.clear();
					transmission_status1 = 0;
					one.unlock();
				}
				else {
					two.lock();
					transmitting_data2.clear();
					transmission_status2 = 0;
					two.unlock();
				}
				//2 backoff
				std::this_thread::sleep_for(std::chrono::milliseconds(backoff(collision_count)));
				if (collision_count > 15) {
					
					if (choice) {
						//file
						out.lock();
						fs2 << "PC" << mac << " send failure" << "\n";
						fs2 << "\n";
						out.unlock();
					}
					else {
						//terminal
						out.lock();
						cout <<"PC"<< mac << " send failure" << endl;
						cout << endl;
						out.unlock();
					}
					//recover
					collision_count = 0;
					if (switch_port == 1) {
						one.lock();
						transmitting_data1.clear();
						transmission_status1 = 0;
						one.unlock();
					}
					else {
						two.lock();
						transmitting_data2.clear();
						transmission_status2 = 0;
						two.unlock();
					}
				}

			}

		}
		//1-persist
		else {
			//std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 20));
		}
	}
};

void PC:: get_send_frame() {
	vector<int> data;
	//or set data length
	//default protocol 0x0800
	vector<int> protocol_type;
	{
		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);

		protocol_type.push_back(1);
		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);

		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);

		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);
		protocol_type.push_back(0);
	}
	//set data 		
	int data_size = rand() % 10 + 1;
	int bit;
	for (int i = 0; i < data_size; i++) {
		bit = rand() % 2;
		data.push_back(bit);
	}

	//set receiver
	int RandomReceiver;
	RandomReceiver = rand() % 5; //0-4

	vector<int> frame = getframe(data, mac, RandomReceiver, protocol_type);
	receiver = RandomReceiver;
	send_data = data;
	send_frame = reverseframe(frame);
	status = 1;
};

void PC::receive_frame() {
	ofstream fs;
	if (choice) {
		string error_filename = "error_log" + to_string(mac) + ".txt";
		fs.open(error_filename, ios::out);
	}
	vector<int> jam_data;
	for (int i = 0; i < 32; i++) {
		jam_data.push_back(1);
	}
	//always at receiving
	while (true) {
		vector<int> received_data;
		if(switch_port==1)
			received_data = transmitting_data1;
		else
			received_data = transmitting_data2;
		                                                std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//if data detected
		if (!received_data.empty()) {
			//if not jam : receive frame
			if (received_data != jam_data) {			
				buffer[mac].lock();
				int currentBufferSize = receive_buffer.size();
				buffer[mac].unlock();
				//buffer out of range
				if (currentBufferSize >= receive_buffer_size) {
					if (choice) {
						out.lock();
						//file
						fs << "PC" << mac << " receive buffer full! abandon" << "\n";
						fs << "\n";
						out.unlock();
					}
					else{
						out.lock();
						//terminal
						cout << "PC" << mac << " receive buffer full! abandon" << endl;
						cout << endl;
						out.unlock();
					}
						received_data.clear();
					
				}
				//there is buffer space
				else {
					//odd check -- frame is wrong
					if (!odd_check(received_data)) {
						//abandon
						if (choice) {
							out.lock();
							//file
							fs << "PC" << mac << " oddcheck frame is wrong! abandon" << "\n";
							fs << "\n";
							out.unlock();
						}
						else {
							out.lock();
							//terminal
							cout << "PC" << mac << " oddcheck frame is wrong! abandon" << endl;
							cout << endl;
							out.unlock();
						}
					}
					//frame data is intact
					else {
						//data = decode(received_data);
						vector<int>reversed_data = reverseframe(received_data);
						int receiver = 0;
						receiver += reversed_data[8] * 4;
						receiver += reversed_data[9] * 2;
						receiver += reversed_data[10] * 1;

						//receiver is self
						if (receiver == mac) {
							buffer[mac].lock();
							receive_buffer.push(received_data);
							buffer[mac].unlock();
							reversed_data.clear();
							received_data.clear();
						}
						else {
							//abandon this frame
							received_data.clear();
							reversed_data.clear();
						}
					}
				}
			}
			//abandon jam 
			else {
				//abandon
				if (choice) {
					out.lock();
					//file
					fs << "PC" << mac << " received jam signal! abandon" << "\n";
					fs << "\n";
					out.unlock();
				}
				else {
					//terminal
					out.lock();
					cout << "PC" << mac << " received jam signal! abandon" << endl;
					cout << endl;
					out.unlock();
				}
				received_data.clear();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else{
			//continue detectting
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void PC::decode_frame() {
	vector<int> received_data;
	vector<int> reversed_data;
	vector<int> data;
	ofstream fs;
	if (choice) {
		string filename = "received_log" + to_string(mac) + ".txt";
		fs.open(filename, ios::out);
	}
	//always at decoding
	while (true) {	
		buffer[mac].lock();
		int bfsize = receive_buffer.size();
		buffer[mac].unlock();
		if (bfsize > 0) {
			buffer[mac].lock();
			received_data = receive_buffer.front();		
			receive_buffer.pop();			
			buffer[mac].unlock();
			reversed_data = reverseframe(received_data);
			data = decode(received_data);
			int sender = get_sender(reversed_data);
		//file
			if (choice) {
				out.lock();
				fs << "PC" << mac << " received data:";
				for (int i = 0; i < data.size(); i++) {
					fs << data[i];
				}
				fs << " from PC" << sender;
				fs << " spared space:" << receive_buffer_size - bfsize + 1 << endl;
				fs << '\n';
				//fs.close();
				out.unlock();
			}
			//terminal
			else {
				out.lock();
				cout << "PC" << mac << " received data:";
				for (int i = 0; i < data.size(); i++) {
					cout << data[i];
				}
				buffer[mac].lock();
				cout << " spared space:" << receive_buffer_size - bfsize + 1 << endl;
				buffer[mac].unlock();
				cout << endl;
				out.unlock();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		else {
			////used for simulation of inconsistent sending and receiving speed
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		received_data.clear();
		data.clear();
	}
}
