#include "Utils.h"

short Reverse_SubMap[] = {14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
extern unsigned int HexKey;
const int pairs = 32000;

void generator(unsigned short* Input,unsigned short* Output){
    unsigned short Input_x;
    unsigned short Output_y;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < pairs; ++i) {
        Input_x = rand() % 0xffff;
        SPN(Input_x,Output_y,HexKey);
        Input[i] = Input_x;
        Output[i] = Output_y;
        //cout<<hex<<Input_x<<" "<<Output_y<<endl;
    }
}

void LinearAttack(unsigned short* Input, unsigned short* Output,unsigned short &key_r16){
    short count_24[16][16] = {0};
    short count_13[16][16] = {0};
    unsigned short Y1,Y2,Y3,Y4,L1,L2,L3,L4,V_41,V_42,V_43,V_44,U_41,U_42,U_43,U_44 = 0x0;
    for (int i = 0; i < pairs; ++i) {
        Y1 = (Output[i] & 0xf000)>>12;
        Y2 = (Output[i] & 0x0f00)>>8;
        Y3 = (Output[i] & 0x00f0)>>4;
        Y4 = (Output[i] & 0x000f);
        for (short j = 0; j < 16; ++j) {
            L4 = j;
            L1 = L4;
            for (short k = 0; k < 16; ++k) {
                L2 = k;
                L3 = L2;
                V_41 = Y1 ^ L1;
                V_42 = Y2 ^ L4;
                V_43 = Y3 ^ L3;
                V_44 = Y4 ^ L2;
                U_41 = Reverse_SubMap[V_41];
                U_42 = Reverse_SubMap[V_42];
                U_43 = Reverse_SubMap[V_43];
                U_44 = Reverse_SubMap[V_44];
                short Z_24 = ((Input[i] & 0x0800)>>11) ^ ((Input[i] & 0x0200)>>9) ^ ((Input[i] & 0x0100)>>8)
                        ^ ((U_42 & 0x4)>>2) ^ (U_42 & 0x1) ^ ((U_44 & 0x4)>>2) ^ (U_44 & 0x1);
                if (Z_24 == 0)
                    count_24[j][k]++;
                short Z_13 = ((Input[i] & 0x0400)>>10) ^ ((Input[i] & 0x0200)>>9) ^ ((Input[i] & 0x0100)>>8)
                        ^ ((U_43 & 0x4)>>2) ^ (U_43 & 0x1) ^ ((U_41 & 0x4)>>2) ^ (U_41 & 0x1);
                if(Z_13 == 0)
                    count_13[j][k]++;
            }
        }
    }

    short max_24 = -1;
    short max_13 = -1;
    unsigned short Max_1,Max_2,Max_3,Max_4 = 0x0;
    for (short i = 0; i < 16; ++i) {
        for (short j = 0; j < 16; ++j) {
            count_24[i][j] = abs(count_24[i][j] - pairs/2);
            count_13[i][j] = abs(count_13[i][j] - pairs/2);
            if(count_24[i][j] > max_24){
                max_24 = count_24[i][j];
                Max_2 = i;
                Max_4 = j;
            }
            if(count_13[i][j] > max_13){
                max_13 = count_13[i][j];
                Max_1 = i;
                Max_3 = j;
            }
        }
    }
    key_r16 = (Max_1<<12) | (Max_2<<8) | (Max_3<<4) | Max_4;
    //cout<<hex<<Max_1<<" "<<Max_2<<" "<<Max_3<<" "<<Max_4<<endl;
    cout<<hex<<key_r16<<endl;
}

void Crack(unsigned short key_r16){
    unsigned short hex_x = 0x26b7;
    unsigned short res,temp;
    SPN(hex_x,res,HexKey);//result of hex_x
    unsigned short key_f16 = 0x0000;
    unsigned int Key_new = 0x00000000;
    bool flag = true;
    while(flag){
        Key_new = (key_f16<<16) | key_r16;
        SPN(hex_x,temp,Key_new);
        if(temp == res)
            flag = false;
        else
            key_f16 += 1;
    }
    cout<<hex<<Key_new<<endl;
}

void LinearAttack_test(){
    unsigned short* Input = new unsigned short[pairs+1];
    unsigned short* Output = new unsigned short[pairs+1];
    unsigned short key_r16;
    generator(Input,Output);
    LinearAttack(Input,Output,key_r16);
    Crack(key_r16);
}
