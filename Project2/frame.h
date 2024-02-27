#pragma once
#ifndef   FRAME_H       
#define   FRAME_H   
#include<vector>
using namespace std;
//package
vector<int> getframe(vector<int>data,int source,int receiver,vector<int>type);
//send reverse
vector<int> reverseframe(vector<int> frame);
//decode data
vector<int> decode(vector<int> frame);
//get sender
int get_sender(vector<int> frame);
//FCS check
bool odd_check(vector<int> frame);

#endif