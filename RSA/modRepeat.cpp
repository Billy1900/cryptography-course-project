#include "RSA_Utils.h"

void ModRepeatSquare(BIGNUM *Result,const BIGNUM *b,const BIGNUM *Mod,const BIGNUM *Pow){
    BIGNUM *DivRes = BN_new();
    BIGNUM *Cup_b = BN_new();
    BIGNUM *Cup_Pow = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *a_new = BN_new();
    BIGNUM *b_sqr = BN_new();
    BIGNUM *Rem = BN_new();
    BIGNUM *Two = BN_new();

    BN_CTX *ctx = BN_CTX_new();
    BN_copy(Cup_b,b);
    BN_copy(Cup_Pow,Pow);

    BN_set_word(a_new,1);
    BN_set_word(Two,2);
    //(b^pow)mod(mod)=result
    //将Pow转换为二进制的算法：位上为1是an=an-1*bn,bn=bn-1*bn-1;位上为0时是an=an-1,bn=bn-1*bn-1
    do {
        BN_div(DivRes, Rem, Cup_Pow, Two, ctx);//计算Cup_Pow与Two的商，值储存在DivRes中，余数储存在rem中。
        if (BN_is_one(Rem))
            BN_mod_mul(a, a_new, Cup_b, Mod, ctx);//计算a_new与Cup_b的积，再模mod，值储存在a中。
        else BN_copy(a, a_new);

        BN_mod_sqr(b_sqr, Cup_b, Mod, ctx);//计算Cup_b^2，再模mod，值储存在b_sqr中;


        BN_copy(a_new, a);
        BN_copy(Cup_Pow, DivRes);
        BN_copy(Cup_b, b_sqr);

    } while (!BN_is_zero(DivRes));//余数为0跳出循环

    BN_copy(Result,a);

    BN_CTX_free(ctx);
    BN_free(Two);
    BN_free(Rem);
    BN_free(b_sqr);
    BN_free(a_new);
    BN_free(a);
    BN_free(Cup_Pow);
    BN_free(Cup_b);
    BN_free(DivRes);
}

void ModRepeat_Encrypt(BIGNUM *n,BIGNUM *e,const char *Plain,BIGNUM *Cipher){
    BIGNUM *Plain_temp = BN_new();

    BN_bin2bn((unsigned char*)Plain, strlen(Plain), Plain_temp);
    ModRepeatSquare(Cipher, Plain_temp, n, e);

    printf("Cipher:\n");
    printf("%s\n",BN_bn2hex(Cipher));

    BN_free(Plain_temp);
}

void ModRepeat_Decryption(BIGNUM *n,BIGNUM *d,BIGNUM *Cipher){
    BIGNUM *Plain_temp = BN_new();

    ModRepeatSquare(Plain_temp,Cipher,n,d);

    printf("Plain:\n");
    printf("%s\n",BN_bn2hex(Plain_temp));

    BN_free(Plain_temp);
}
