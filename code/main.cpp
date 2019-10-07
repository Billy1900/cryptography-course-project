#include "Utils.h"

int main(){
    //SPN_test();
    clock_t start,end;
    start = clock();
    LinearAttack_test();
    end = clock();
    cout<<(double)(end-start)<<"ms";
}
