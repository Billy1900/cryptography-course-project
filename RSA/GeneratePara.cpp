#include <openssl/bn.h>
#include <cstdio>

void RSA_Parameter(){
    BIGNUM *p=BN_new();
    BIGNUM *q=BN_new();
    BIGNUM *n=BN_new();
    BIGNUM *d=BN_new();
    BIGNUM *e=BN_new();

    BIGNUM *p_Cutone = BN_new();
    BIGNUM *q_Cutone = BN_new();
    BIGNUM *exp = BN_new();
    BIGNUM *GCD = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    //generate p,q
    do{
        BN_generate_prime(p, 512, NULL, NULL, NULL, NULL, NULL);
    }while (!BN_is_prime(p,NULL,NULL,NULL,NULL));
    do{
        BN_generate_prime(q, 512, NULL, NULL, NULL, NULL, NULL);
    }while (!BN_is_prime(q,NULL,NULL,NULL,NULL));

    BN_mul(n,p,q,ctx);//n=p*q
    BN_sub(p_Cutone,p,BN_value_one()); //p_Cutone = p - 1
    BN_sub(q_Cutone,q,BN_value_one());//q_Cutone = q - 1
    BN_mul(exp,p_Cutone,q_Cutone,ctx);//exp = p_Cutone * q_Cutone

    do{
        BN_rand_range(e,exp);
        BN_gcd(GCD,e,exp,ctx);
    }while (BN_cmp(GCD,BN_value_one()));// e
    while (!BN_mod_inverse(d,e,exp,ctx));//d

    printf("private key:\n");
    printf("p:%s\n",BN_bn2hex(p));
    printf("q:%s\n",BN_bn2hex(q));
    printf("d:%s\n",BN_bn2hex(d));
    printf("Public key:\n");
    printf("N:%s\n",BN_bn2hex(n));
    printf("e:%s\n",BN_bn2hex(e));

    BN_CTX_free(ctx);
    BN_free(GCD);
    BN_free(q_Cutone);
    BN_free(p_Cutone);
    BN_free(exp);
}

