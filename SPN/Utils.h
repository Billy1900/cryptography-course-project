

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <ctime>
#include <fstream>
#include <memory.h>
using namespace std;


void SPN_test();
void SPN(unsigned short hex_x, unsigned short &res, unsigned int Key);
void Key_arrange(int i,unsigned short &Kr,unsigned int Key);

void LinearAttack_test();
void Crack(unsigned short key_r16);
void Dif_Attack_test();

void SPN_Plus_test();
void SPN_Plus(unsigned long long x, unsigned long long &res);

void RandTest();

#endif //TEST_UTILS_H
