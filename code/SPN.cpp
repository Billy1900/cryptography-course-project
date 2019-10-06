#include <iostream>
using namespace std;

//int KEY[] = {0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1};
//int HexKey[] = {0x3,0xa,0x9,0x4,0xd,0x6,0x3,0xf};
unsigned int HexKey = 0x3a94d63f;

unsigned short Sub_map_value[] = {0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};

unsigned short Per_map_value[] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};

void Key_arrange(int i,unsigned short &Kr){
    unsigned int mask = 0xffff0000;
    i -= 1;
    mask = mask>>(4*i);
    Kr = (HexKey & mask)>>(4-i)*4;
}

void Substi(unsigned short Ur, unsigned short &Vr){
    unsigned short mask[] = {0xf000,0x0f00,0x00f0,0x000f};
    unsigned short res;
    unsigned short temp = 0x0000;
    for (int i = 0; i < 4; ++i) {
        res = (Ur & mask[i])>>(4*(3-i));
        temp |= (Sub_map_value[res]<<(4*(3-i)));
    }
    Vr = temp;
}

void Permutation(unsigned short Vr, unsigned short &w){
    unsigned int mask = 0x10000;
    unsigned short temp;
    unsigned short res = 0x0000;
    for (int i = 0; i < 16; ++i) {
        mask = mask>>1;
        temp = (Vr & mask)>>(15-i);
        res = res | (temp<<(16-Per_map_value[i]));
    }
    w = res;
}

void SPN(unsigned short hex_x){
    unsigned short w = hex_x;
    unsigned short Kr,Ur,Vr = 0x0000;
    int Nr = 4;
    for (int r = 1; r <= Nr-1; ++r) {
        Key_arrange(r,Kr);
        cout<<"Kr:"<<hex<<Kr<<endl;

        Ur = Kr ^ w;
        cout<<"Ur:"<<Ur<<endl;

        Substi(Ur, Vr);
        cout<<"Vr:"<<Vr<<endl;

        Permutation(Vr,w);
        cout<<"w:"<<w<<endl;

        cout<<"-----"<<endl;
    }

    Key_arrange(Nr,Kr);
    cout<<"w: "<<w<<endl;
    cout<<"K4: "<<Kr<<endl;
    Ur = Kr ^ w;
    cout<<"Ur: "<<Ur<<endl;
    Substi(Ur,Vr);

    Key_arrange(Nr+1,Kr);
    unsigned short y = Vr ^ Kr;

    cout<<"y:"<<y;
}

int main(){
    unsigned short hex_x = 0x26b7;
    SPN(hex_x);
}
