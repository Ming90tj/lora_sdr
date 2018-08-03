//
// Created by hzm on 27/07/18.
//
#include <fstream>
#include <cmath>
#include <iostream>
#include <cstring>
#include <complex>
#include <vector>
#include <stdio.h>
#include <iomanip>
#include </home/hzm/CLionProjects/lora_test/ChirpGenerator.h>
#include </home/hzm/CLionProjects/lora_test/cmake-build-debug/Data2File.h>
using namespace std;

int main()
{
//    Data2File occ;
//    vector<complex<float>> upChirp;
//    auto entry = polar(-M_PI,1.0);
//    upChirp.emplace_back(complex<float> (entry));
//    entry = polar(1.0,M_PI_2);
//    upChirp.emplace_back(complex<float> (entry));
//    char st1[] = "test1.csv";
//    occ.Data2Csv(st1 ,upChirp.size(), upChirp.data());
//    double phase = -M_PI/10;
//    cout <<setprecision(10)<< phase <<endl;
//    float ph = (float) phase;
//    cout << ph <<endl;
    enum LoraDemodState
    {
        STATE_WAITINPUT,
        STATE_FRAMESYNC,
        STATE_SYNCWORD0,
        STATE_SYNCWORD1,
        STATE_DOWNCHIRP0,
        STATE_DOWNCHIRP1,
        STATE_QUARTERCHIRP,
        STATE_DATASYMBOLS,
        STATE_END,
    };
    const int N = 1<<6;
    int ovs = 1;
    const int NN = N * ovs;
    float ampl = 1.0;
    float accum;
    int i = 0;
    int counter;
    unsigned char sync = 0x12;
    LoraDemodState state;
    std::vector<std::complex<float >> samps(N);
    std::complex<float >* sps = samps.data();
    state = STATE_WAITINPUT;
    while(state != STATE_END) {
        switch (state) {
            ////////////////////////////////////////////////////////////////
            case STATE_WAITINPUT:
                ////////////////////////////////////////////////////////////////
            {
                counter = 10;
                accum = 0.0;
                state = STATE_FRAMESYNC;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_FRAMESYNC:
                ////////////////////////////////////////////////////////////////
            {
                counter--;
                i = genChirp(sps, N, ovs, NN, 0.0f, false, ampl, accum);
                if (counter == 0) state = STATE_SYNCWORD0;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_SYNCWORD0:
                ////////////////////////////////////////////////////////////////
            {
                const int sw0 = (sync >> 4) * 8;
                const auto freq = (float) ((2 * M_PI * sw0) / NN);
                i = genChirp(sps, N, ovs, NN, freq, false, ampl, accum);
                state = STATE_SYNCWORD1;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_SYNCWORD1:
                ////////////////////////////////////////////////////////////////
            {
                const int sw1 = (sync & 0xf) * 8;
                const auto freq = (float) ((2 * M_PI * sw1) / NN);
                i = genChirp(sps, N, ovs, NN, freq, false, ampl, accum);
                state = STATE_DOWNCHIRP0;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_DOWNCHIRP0:
                ////////////////////////////////////////////////////////////////
            {
                i = genChirp(sps, N, ovs, NN, 0.0f, true, ampl, accum);
                state = STATE_DOWNCHIRP1;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_DOWNCHIRP1:
                ////////////////////////////////////////////////////////////////
            {
                i = genChirp(sps, N, ovs, NN, 0.0f, true, ampl, accum);
                state = STATE_QUARTERCHIRP;
            }
                ////////////////////////////////////////////////////////////////
            case STATE_QUARTERCHIRP:
                ////////////////////////////////////////////////////////////////
            {
                i = genChirp(sps, N, ovs, NN / 4, 0.0f, true, ampl, accum);
                state = STATE_DATASYMBOLS;
                counter = 0;
            }
                ///////////////////////////////////////////////////////////////
            case STATE_DATASYMBOLS:
                //////////////////////////////////////////////////////////
            {
                const int sym = 10;
                const auto freq = (float) ((2 * M_PI * sym) / NN);
                i = genChirp(sps, N, ovs, NN, freq, false, ampl, accum);
                state = STATE_END;
            }
        }
    }
    cout<<"end of generate chirp signal"<<endl;
    return 0;
}
