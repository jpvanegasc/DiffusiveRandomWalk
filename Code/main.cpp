#include<iostream>
#include<string>

#include"CA_RandomWalk.hpp"

int main(void){
    RWLattice CoffeeMug;

    CoffeeMug.initialize();
    CoffeeMug.save("coffee.txt");
    
    return 0;
}