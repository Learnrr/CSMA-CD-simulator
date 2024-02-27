#pragma once
#ifndef   CSMA_H       
#define   CSMA_H   
#include<mutex>
#include<vector>
#define SLOT 5
using namespace std;

//            structure
// -----------------------------------------
//  PC0----------1 SWITCH 2-------PC3       |
//          |                |              |
//  PC1-----|                |-----PC4		|
//          |								|
//  PC2-----|								|
//------------------------------------------
//
//data on transmission line
extern vector<int> transmitting_data1;
//free 0 busy 1 jam 2;
extern int transmission_status1;
//line2
extern vector<int> transmitting_data2;
extern int transmission_status2;
//ensure consistence in transmission1 info
extern mutex one;
//ensure consistence in transmission2 info
extern mutex two;
//ensure complete window/file print info 
extern mutex out;

//get minimum 
int min(int a, int b);
//2 backoff
int backoff(int collision_count);
#endif

