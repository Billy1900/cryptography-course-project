#include "Utils.h"

unsigned long long Substi_box8[16][16] = {{0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},
                                          {0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},
                                          {0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},
                                          {0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},
                                          {0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},
                                          {0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},
                                          {0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},
                                          {0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},
                                          {0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},
                                          {0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},
                                          {0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},
                                          {0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},
                                          {0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},
                                          {0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},
                                          {0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},
                                          {0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16}
};

int PerMap_plus[64] = {1,9,17,25,33,41,49,57,
                       2,10,18,26,34,42,50,58,
                       3,11,19,27,35,43,51,59,
                       4,12,20,28,36,44,52,60,
                       5,13,21,29,37,45,53,61,
                       6,14,22,30,38,46,54,62,
                       7,15,23,31,39,47,55,63,
                       8,16,24,32,40,48,56,64};

unsigned long long Key_Plus[] = {0x92,0xd7,0x8b,0x0c,0x3f,0x46,0x5e,0x9a,0x67,0xcd,0x26,0xb7,0x3a,0x94,0xd6,0x3f};//128 bits

void Key_arrange_Plus(int index, unsigned long long &Kr, unsigned long long* key){
    index -= 1;
    unsigned long long temp = 0x0000000000000000;//64 bits
    for (int j = 1; j <= 8; j++,index++)
        temp |= (key[index] << (8 * (8 - j)));
    Kr = temp;
}

void Substi_Plus(unsigned long long Ur, unsigned long long &Vr){
    unsigned long long mask_x = 0xf000000000000000;//64 bits
    unsigned long long mask_y = 0x0f00000000000000;
    unsigned long long temp = 0x0000000000000000;//64 bits
    unsigned short x,y = 0x0;
    for (int i = 0; i < 8; i++) {
        x = (mask_x & Ur)>>(64-4*(2*i+1));//4 bits
        y = (mask_y & Ur)>>(64-4*(2*i+1+1));
        temp |= (Substi_box8[x][y] << (64 - 8 * (i + 1)));
        mask_x = mask_x>>8;
        mask_y = mask_y>>8;
    }
    Vr = temp;
}

void Permutation_Plus(unsigned long long Vr, unsigned long long &w){
    unsigned long long mask = 0x8000000000000000;//64 bits
    unsigned long long temp,res = 0x0000000000000000;
    for (int k = 0; k < 64; ++k) {
        temp = (Vr & mask)>>(63-k);
        res |= (temp<<(64-PerMap_plus[k]));
        mask = mask>>1;
    }
    w = res;
}

void SPN_Plus(unsigned long long x, unsigned long long &res){
    unsigned long long w = x;
    unsigned long long Kr,Ur,Vr = 0x0000000000000000;//64 bits
    int Nr = 8;
    for (int r = 1; r <= Nr-1; ++r) {
        Key_arrange_Plus(r,Kr,Key_Plus);
        //cout<<"Kr: "<<hex<<Kr<<endl;
        Ur = Kr ^ w;
        //cout<<"Ur:"<<hex<<Ur<<endl;
        Substi_Plus(Ur,Vr);
        //cout<<"Vr: "<<hex<<Vr<<endl;
        Permutation_Plus(Vr,w);
        //cout<<"w:"<<hex<<w<<endl;
    }
    Key_arrange_Plus(Nr,Kr,Key_Plus);
    Ur = Kr ^ w;
    Substi_Plus(Ur,Vr);
    Key_arrange_Plus(Nr+1,Kr,Key_Plus);
    res = Vr ^ Kr;
}

void SPN_Plus_test(){
    unsigned long long x = 0x3f465e92d71a8b0c;//64 bits
    unsigned long long res;
    SPN_Plus(x,res);
    cout<<"The SPN_Plus result is:"<<hex<<res<<endl;
}
