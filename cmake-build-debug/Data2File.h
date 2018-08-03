//
// Created by hzm on 30/07/18.
//
#include <fstream>
#include <complex>
#include <vector>
#include <libaio.h>
#include <cstring>
#ifndef LORA_TEST_DATA2FILE_H
#define LORA_TEST_DATA2FILE_H
class Data2File
{
public:
    Data2File(void);
    void Data2Csv(char* name ,const unsigned long N,std::complex<float>* output);
};

#endif //LORA_TEST_DATA2FILE_H
