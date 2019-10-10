#include "Utils.h"

void Write(){
    fstream fin("C:\\Users\\PC\\Desktop\\crypto\\test.txt",ios::binary|ios::out);
    //write into file with hex
    char *temp=new char[10*1024*1024];
    memset(temp,0,10*1024*1024);
    fin.write(temp,10*1024*1024);
    fin.close();
}

void Rand(){
    fstream fin("C:\\Users\\PC\\Desktop\\crypto\\test.txt",ios::binary|ios::in);
    ofstream fout("C:\\Users\\PC\\Desktop\\crypto\\Output.txt",ios::binary|ios::out);

    unsigned long long Iv = 0x0000000000000000;
    char *buffer = new char[10*1024*1024];
    fin.read(buffer,10*1024*1024);
    unsigned long long temp_x,temp_res = 0x0000000000000000;
    char t[8];

    for (int i = 0,count = 0; i <10*1024*1024; ++i) {
        temp_x |= (unsigned long long) ((unsigned char)buffer[i]) << (64 - 8 * (count + 1));//8 bits per buffer[i]
        count++;
        if(count == 7){
            temp_x = temp_x ^ Iv;
            SPN_Plus(temp_x,temp_res);
            Iv = temp_res;
            //cout<<hex<<temp_res<<endl;
            //output
            unsigned long long mask = (unsigned long long)(0xff)<<56;
            for (int j = 0; j < 8; ++j) {
                t[j] = (mask&temp_res)>>((7-j)*8);
                mask = mask>>8;
            }
            fout.write(t,8);

            count = 0;
        }
    }

    fin.close();
    fout.close();
}

void RandTest(){
    Write();
    Rand();
}

