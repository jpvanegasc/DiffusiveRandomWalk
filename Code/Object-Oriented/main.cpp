#include<iostream>
#include<string>
#include<fstream>

#include"CA_RandomWalk.hpp"

int main(void){
    RWLattice CoffeeMug;
    std::ofstream File("hole.txt");
    int t_max = 4e5;

    CoffeeMug.initialize();
    
    for (int t=0; t<t_max; t++){
        File << t << '\t' << CoffeeMug.count() << '\n';
        CoffeeMug.propagate_with_hole();
        //if(CoffeeMug.check() == false) return 1;
    }
    
    File << std::endl; File.close();
    
    return 0;
}