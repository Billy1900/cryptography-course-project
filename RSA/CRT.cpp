#include "RSA_Utils.h"

//x=y^pow (mod(p*q))
void CRT(BIGNUM *Result,const BIGNUM *y, const BIGNUM *p, const BIGNUM *q,const BIGNUM *Pow){
    BIGNUM *Cp = BN_new();
    BIGNUM *Cq = BN_new();
    BIGNUM *c1 = BN_new();
    BIGNUM *c2 = BN_new();
    BIGNUM *Pow1 = BN_new();
    BIGNUM *Pow2 = BN_new();
    BIGNUM *x11 = BN_new();
    BIGNUM *x12 = BN_new();
    BIGNUM *x21 = BN_new();
    BIGNUM *x22 = BN_new();
    BIGNUM *p_CutOne = BN_new();
    BIGNUM *q_CutOne = BN_new();
    BIGNUM *Mod = BN_new();

    BN_CTX *ctx = BN_CTX_new();

    BN_mul(Mod, p, q, ctx);//mod=n=p*q

    BN_sub(p_CutOne, p, BN_value_one());//p-1
    BN_sub(q_CutOne, q, BN_value_one());//q-1

    BN_nnmod(Pow1, Pow, p_CutOne, ctx);//计算Pow与p-1的模，小于0就加上p-1，存在Pow1中;
    BN_nnmod(Pow2, Pow, q_CutOne, ctx);

    ModRepeatSquare(Cp, y, p, Pow1);//Cp=y^Pow1(mod p)
    ModRepeatSquare(Cq, y, q, Pow2);//Cq=y^Pow2(mod q)

    BN_mod_inverse(c1,q,p,ctx);//模逆，((c*q)%p==1)
    BN_mod_inverse(c2,p,q,ctx);

    BN_mul(x11, Cp, c1, ctx);//x11=Cp*c1
    BN_mul(x12, x11, q, ctx);//x12=x11*q
    BN_mul(x21, Cq, c2, ctx);//x21=Cq*c2
    BN_mul(x22, x21, p, ctx);//x22=x21*p

    BN_mod_add(Result, x12, x22, Mod, ctx);//x=(x12+x22)mod Mod

    BN_CTX_free(ctx);

    BN_free(q_CutOne);
    BN_free(p_CutOne);
    BN_free(x22);
    BN_free(x21);
    BN_free(x12);
    BN_free(x11);
    BN_free(Pow2);
    BN_free(Pow1);
    BN_free(c2);
    BN_free(c1);
    BN_free(Cq);
    BN_free(Cp);
}

void CRT_Encrypt(const char *Plain,BIGNUM *Cipher,const BIGNUM *p,const BIGNUM *q,const BIGNUM *e){
    BIGNUM *Plain_temp = BN_new();

    BN_bin2bn((unsigned char*)Plain, strlen(Plain), Plain_temp);
    CRT(Cipher,Plain_temp,p,q,e);

    printf("Cipher:\n");
    printf("%s\n",BN_bn2hex(Cipher));

    BN_free(Plain_temp);
}

void CRT_Decrypt(const BIGNUM *Cipher,const BIGNUM *p,const BIGNUM *q,const BIGNUM *d){
    BIGNUM *Plain_temp = BN_new();

    CRT(Plain_temp,Cipher,p,q,d);

    printf("Plain:\n");
    printf("%s\n",BN_bn2hex(Plain_temp));

    BN_free(Plain_temp);
}
