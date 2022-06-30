#pragma once
#include<iostream>
#include<string>
#include "Gun.h"
using namespace std;

class Solider
{
public:
    Solider(string name);
    ~Solider();
    void addBulletToGun(int num);
    void addGun(Gun* ptr_gun);
    bool fire();

private:
    string _name;
    Gun* _ptr_gun;

};