#include "frame.h"
using namespace std;

//generate frame
vector<int> getframe(vector<int> data, int source, int receiver, vector<int> type) {
	vector<int> frame;
    int onecount = 0;//用于奇偶校验
    int bitcount = 0;//用于填充
    //Start of frame delimiter
    frame.push_back(0);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(0);
    onecount += 6;
    
    //set receiver mac address
    //A:000 B:001 C:010 D:011 E:100 F:101
    switch (receiver) {
    case 0:
        frame.push_back(0);
        frame.push_back(0);
        frame.push_back(0);
        break;
    case 1:
        frame.push_back(0);
        frame.push_back(0);
        frame.push_back(1);
        onecount = onecount + 1;
        break;
    case 2:
        frame.push_back(0);
        frame.push_back(1);
        frame.push_back(0);
        onecount = onecount + 1;
        break;
    case 3:
        frame.push_back(0);
        frame.push_back(1);
        frame.push_back(1);
        onecount = onecount + 2;
        break;
    case 4:
        frame.push_back(1);
        frame.push_back(0);
        frame.push_back(0);
        onecount = onecount + 1;
        break;
    case 5:
        frame.push_back(1);
        frame.push_back(0);
        frame.push_back(1);
        onecount = onecount + 2;
        break;
    }

    //set source address
    //
    switch (source) {
    case 0:
        frame.push_back(0);
        frame.push_back(0);
        frame.push_back(0);
        break;
    case 1:
        frame.push_back(0);
        frame.push_back(0);
        frame.push_back(1);
        onecount = onecount + 1;
        break;
    case 2:
        frame.push_back(0);
        frame.push_back(1);
        frame.push_back(0);
        onecount = onecount + 1;
        break;
    case 3:
        frame.push_back(0);
        frame.push_back(1);
        frame.push_back(1);
        onecount = onecount + 2;
        break;
    case 4:
        frame.push_back(1);
        frame.push_back(0);
        frame.push_back(0);
        onecount = onecount + 1;
        break;
    case 5:
        frame.push_back(1);
        frame.push_back(0);
        frame.push_back(1);
        onecount = onecount + 2;
        break;
    }
    //set type
    //IPv4（0x0800）, IPv6(0x86DD), ARP(0x0806)
    for (int i = 0; i < type.size(); i++ ) {
        frame.push_back(type[i]);
        if (type[i] == 1)
            onecount++;
    }

    //set data
    int count=0;//记录5个数据
    for (int i = 0; i < data.size(); i++) {
        int x = data[i];
        if (x == 1) {
            count++;
            onecount++;
        }
        else
            count = 0;
        frame.push_back(x);
        bitcount++;
        //每5个连续的1后加0
        if (count == 5){
            frame.push_back(0);
            count = 0;
        }
    }
    //set extra 60-39-0=21
    for (int i = 0; i < 21;i++)
        frame.push_back(0);
    //set FCS 奇校验

    if (onecount % 2 == 0)
        frame.push_back(1);
    else
        frame.push_back(0);
    //End of frame delimiter
    frame.push_back(0);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(1);
    frame.push_back(0);

    return frame;
}

vector<int> reverseframe(vector<int> frame) {
    vector<int> reversed_frame;
    for (int i = frame.size() - 1; i >= 0; i--)
    {
        int x = frame[i];
        reversed_frame.push_back(x);
    }
    return reversed_frame;
}

vector<int> decode(vector<int>frame) {
    vector<int> data;
    vector<int> rframe;
    int count=0;
    rframe = reverseframe(frame);
    for (int i = 30; i < rframe.size() - 30; i++) {
        if (rframe[i] == 1) {
            count++;
        }
        else {
            count == 0;
        }
        //每五个连续的1后的0去掉
        if (count == 5 && rframe[i] == 0) {
            count = 0;
            continue;
        }
        data.push_back(rframe[i]);
    }
    return data;
}

bool odd_check(vector<int> frame) {
   // bool result=0;
  //  for (int i = 0; i < frame.size(); i++) {
  //      result ^= frame[i];
   // }
   // return result;
    int count = 0;
    for (int i = 0; i < frame.size(); i++) {
        if (frame[i] == 1)
            count++;
    }
    if (count % 2 == 0)
        return false;
    return true;
}
int get_sender(vector<int> frame) {
    int sender=0;
    sender += frame[11] * 4;
    sender += frame[12] * 2;
    sender += frame[13] * 1;
    return sender;
}