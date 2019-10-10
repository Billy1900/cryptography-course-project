#include "Utils.h"

const int Di_pairs = 8000;
extern unsigned int HexKey;
extern short Reverse_SubMap[];
unsigned int Ruler = 0x0b00;
unsigned short Ruler_24 = 0x6;
unsigned short Ruler_13 = 0x6;

void gene_Di(unsigned short* Input, unsigned short* Output,unsigned short* _Input_,unsigned short* _Output_){
    unsigned short x,y,_x_,_y_;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < Di_pairs; ++i) {
        x = rand() % 0xffff;
        SPN(x,y,HexKey);
        _x_ = x ^ Ruler;
        SPN(_x_,_y_,HexKey);
        Input[i] = x;
        Output[i] = y;
        _Input_[i] = _x_;
        _Output_[i] = _y_;
        //cout<<hex<<"x: "<<x<<" y: "<<y<<" x*: "<<_x_<<" y*: "<<_y_<<endl;
    }
}

void Dif_Attack(unsigned short* Output,unsigned short* _Output_, unsigned short &key_r16){
    short count_24[16][16] = {0};
    short count_13[16][16] = {0};
    unsigned short y1,_y1_,y2,_y2_,y3,_y3_,y4,_y4_,L1,L2,L3,L4,
    V41,V42,V43,V44,U41,U42,U43,U44,_V41_,_V42_,_V43_,_V44_,
    _U41_,_U42_,_U43_,_U44_,uu41,uu42,uu43,uu44;
    for (short i = 0; i < Di_pairs; ++i) {
        y1 = (Output[i] & 0xf000)>>12;
        _y1_ = (_Output_[i] & 0xf000)>>12;
        y2 = (Output[i] & 0x0f00)>>8;
        _y2_ = (_Output_[i] & 0x0f00)>>8;
        y3 = (Output[i] & 0x00f0)>>4;
        _y3_ = (_Output_[i] & 0x00f0)>>4;
        y4 = (Output[i] & 0x000f);
        _y4_ = (_Output_[i] & 0x000f);
        //2,4
        if (y1 == _y1_ && y3 == _y3_) {
            for (short j = 0; j < 16; ++j) {
                L2 = j;
                for (short k = 0; k < 16; ++k) {
                    L4 = k;
                    V42 = L2 ^ y2;
                    V44 = L4 ^ y4;
                    U42 = Reverse_SubMap[V42];
                    U44 = Reverse_SubMap[V44];
                    _V42_ = L2 ^ _y2_;
                    _V44_ = L4 ^ _y4_;
                    _U42_ = Reverse_SubMap[_V42_];
                    _U44_ = Reverse_SubMap[_V44_];
                    uu42 = U42 ^ _U42_;
                    uu44 = U44 ^ _U44_;
                    if (uu42 == Ruler_24 && uu44 == Ruler_24)
                        count_24[j][k]++;
                }
            }
        }
        //1,3
        if (y2 == _y2_ && y4 == _y4_){
            for (short i = 0; i < 16; ++i) {
                L1 = i;
                for (short j = 0; j < 16; ++j) {
                    L3 = j;
                    V41 = y1 ^ L1;
                    V43 = y3 ^ L3;
                    U41 = Reverse_SubMap[V41];
                    U43 = Reverse_SubMap[V43];
                    _V41_ = _y1_ ^ L1;
                    _V43_ = _y3_ ^ L3;
                    _U41_ = Reverse_SubMap[_V41_];
                    _U43_ = Reverse_SubMap[_V43_];
                    uu41 = U41 ^ _U41_;
                    uu43 = U43 ^ _U43_;
                    if (uu41 == Ruler_13 && uu43 == Ruler_13)
                        count_13[i][j]++;
                }
            }
        }
    }

    short max_24,max_13 = -1;
    unsigned short max_1,max_2,max_3,max_4;
    for (short i = 0; i < 16; ++i) {
        for (short j = 0; j < 16; ++j) {
            if (count_24[i][j] > max_24){
                max_24 = count_24[i][j];
                max_2 = i;
                max_4 = j;
            }
            if (count_13[i][j] > max_13){
                max_13 = count_13[i][j];
                max_1 = i;
                max_3 = j;
            }
        }
    }
    key_r16 = (max_1<<12) | (max_2<<8) | (max_3<<4) | (max_4);
    cout<<hex<<key_r16<<endl;
    //cout << hex <<max_1<<" "<< max_2 <<" "<<max_3<<" "<< max_4 <<endl;
}

void Dif_Attack_test(){
    unsigned short* Input = new unsigned short[Di_pairs+1];
    unsigned short* _Input_ = new unsigned short[Di_pairs+1];
    unsigned short* output = new unsigned short[Di_pairs+1];
    unsigned short* _output_ = new unsigned short[Di_pairs+1];
    unsigned short key_r16 = 0x0000;
    gene_Di(Input,output,_Input_,_output_);
    Dif_Attack(output,_output_, key_r16);
    Crack(key_r16);
}
