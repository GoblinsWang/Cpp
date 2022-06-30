#include<iostream>
#include<string>
#include "Solider.h"
#include "Gun.h"

void test(){

    Solider sanduo("xusanduo");
    sanduo.addGun(new Gun("AK47"));
    sanduo.addBulletToGun(20);
    sanduo.fire();

}

int main(){
    cout <<"this is a test!"<<endl;
    test();

    return 0;
}