#include "RSA_Utils.h"

int main(){
    BIGNUM *p=BN_new();
    BIGNUM *q=BN_new();
    BIGNUM *n=BN_new();
    BIGNUM *d=BN_new();
    BIGNUM *e=BN_new();
    BIGNUM *Cipher = BN_new();
    const char* Plain = "1100";

    clock_t start,end;
    start = clock();

    //generate parameter
    RSA_Parameter(p,q,n,d,e);

    //Modrepeat
//    ModRepeat_Encrypt(n,e,Plain,Cipher);
//    ModRepeat_Decryption(n,d,Cipher);

    //CRT
    //CRT_Encrypt(Plain,Cipher,p,q,e);
    //CRT_Decrypt(Cipher,p,q,d);

    //Mont
    //Mont_Encrypt(Plain,Cipher,n,e);
    //Mont_Decrypt(Cipher,n,d);


    end = clock();
    printf("the time is:%g ms\n",double(end-start));
}
