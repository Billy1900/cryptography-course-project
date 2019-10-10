#include "RSA_Utils.h"

void Montgomerie(BIGNUM *Result, const BIGNUM *Base, const BIGNUM *Pow, const BIGNUM *Mod){
    BIGNUM *Res = BN_new();
    BIGNUM *Two = BN_new();
    BIGNUM *Zero = BN_new();
    BIGNUM *Cup = BN_new();
    BIGNUM *Pow_backup = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    BN_one(Result);
    BN_set_word(Zero, 0);
    BN_set_word(Two, 2);
    BN_copy(Res, Base);
    BN_copy(Pow_backup, Pow);
    //
    while (BN_cmp(Pow_backup, Zero) == 1)//大于0循环
    {
        BN_mod(Cup, Pow_backup, Two, ctx);//计算Pow_backup与2的模，存入Cup(取余)

        if (BN_is_zero(Cup)) {
            BN_mod_sqr(Res, Res, Mod, ctx);//Res=Res^2(mod Mod)

            BN_div(Pow_backup, NULL, Pow_backup, Two, ctx);//Pow_backup=Pow_backup/2

        }
        else {
            BN_mod_mul(Result, Result, Res, Mod, ctx);//Result=Result*Res(mod Mod)

            BN_sub(Pow_backup, Pow_backup, BN_value_one());//Pow_backup=Pow_backup-1
        }
    }

    BN_CTX_free(ctx);
    BN_free(Pow_backup);
    BN_free(Cup);
    BN_free(Zero);
    BN_free(Two);
    BN_free(Res);
}

void Mont_Encrypt(const char* plain,BIGNUM *Cipher,const BIGNUM *n, const BIGNUM *e){
    BIGNUM *Plain_temp = BN_new();
    BN_bin2bn((unsigned char*)plain,strlen(plain),Plain_temp);
    Montgomerie(Cipher,Plain_temp,e,n);

    printf("Cipher:\n");
    printf("%s\n",BN_bn2hex(Cipher));

    BN_free(Plain_temp);
}

void Mont_Decrypt(const BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *d){
    BIGNUM *Plain_temp = BN_new();

    Montgomerie(Plain_temp,Cipher,d,n);

    printf("Plain:\n");
    printf("%s\n",BN_bn2hex(Plain_temp));

    BN_free(Plain_temp);
}
