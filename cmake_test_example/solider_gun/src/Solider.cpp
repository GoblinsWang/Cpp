#include "Solider.h"

Solider::Solider(string name){
    this->_name = name;
    this->_ptr_gun = nullptr;
}

void Solider::addGun(Gun* ptr_gun){

    this->_ptr_gun = ptr_gun;
}

void Solider::addBulletToGun(int num){

    _ptr_gun->addBullet(num);
}

bool Solider::fire(){

    return this->_ptr_gun->shoot();
}

Solider::~Solider(){

    if (this->_ptr_gun==nullptr){
        return;
    }else{
        delete _ptr_gun;
    }
    
}