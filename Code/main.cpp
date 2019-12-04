#include<iostream>
#include<string>

#include"CA_RandomWalk.hpp"

int main(void){
    RWLattice CoffeeMug(1);
    int t_max = 1e1;

    CoffeeMug.initialize();
    for (int i=0; i<t_max; i++){
        CoffeeMug.propagate();
        //if(CoffeeMug.check() == false) return 1;
    }
    
    CoffeeMug.save("coffee.txt");
    
    return 0;
}