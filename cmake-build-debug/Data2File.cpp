//
// Created by hzm on 30/07/18.
//
#include <fstream>
#include <iostream>
#include "Data2File.h"
using namespace std;

Data2File::Data2File() {
    cout << "Please type the name of file" <<endl;
}

void Data2File::Data2Csv(char* name,const unsigned long N,std::complex<float>* output) {
    ofstream ocunt;
    ocunt.open(name);
    ocunt << output[0].real();
    if(output[0].imag() < 0){
        ocunt << output[0].imag() << "i";
    }else{
        ocunt << "+" << output[0].imag() << "i";
    }
    for (int i = 1; i < N; ++i) {
        ocunt << "," <<output[i].real();
        if(output[i].imag() < 0){
            ocunt << output[i].imag() << "i";
        }else{
            ocunt << "+" << output[i].imag() << "i";
        }
    }
    ocunt.close();
}
