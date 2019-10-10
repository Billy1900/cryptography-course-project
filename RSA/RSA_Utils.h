
#ifndef RSA_RSA_UTILS_H
#define RSA_RSA_UTILS_H

#include <openssl/bn.h>
#include <cstdio>
#include <cstring>
#include <ctime>

void RSA_Parameter(BIGNUM *p, BIGNUM *q, BIGNUM *n, BIGNUM *d, BIGNUM *e);

void ModRepeatSquare(BIGNUM *Result,const BIGNUM *b,const BIGNUM *Mod,const BIGNUM *Pow);
void ModRepeat_Encrypt(BIGNUM *n,BIGNUM *e,const char *Plain,BIGNUM *Cipher);
void ModRepeat_Decryption(BIGNUM *n,BIGNUM *d,BIGNUM *Cipher);

void CRT(BIGNUM *x,const BIGNUM *y, const BIGNUM *p, const BIGNUM *q,const BIGNUM *Pow);
void CRT_Encrypt(const char *Plain,BIGNUM *Cipher,const BIGNUM *p,const BIGNUM *q,const BIGNUM *e);
void CRT_Decrypt(const BIGNUM *Cipher,const BIGNUM *p,const BIGNUM *q,const BIGNUM *d);

void Montgomerie(BIGNUM *Result, const BIGNUM *Base, const BIGNUM *Pow, const BIGNUM *Mod);
void Mont_Encrypt(const char* plain,BIGNUM *Cipher,const BIGNUM *n, const BIGNUM *e);
void Mont_Decrypt(const BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *d);

#endif //RSA_RSA_UTILS_H
