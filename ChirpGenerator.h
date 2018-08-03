//
// Created by hzm on 02/08/18.
//
#include <complex>
#include <cmath>
#ifndef LORA_TEST_CHIRPGENERATOR_H
#define LORA_TEST_CHIRPGENERATOR_H

int genChirp(std::complex<float > *samps, int N, int ovs, int NN, float f0, bool down, const float ampl, float &phaseAccum)
{
    const float fMin = (float)-M_PI / ovs;
    const float fMax = (float)M_PI / ovs;
    const float fStep = (float)(2 * M_PI) / (N * ovs * ovs);
    float f = fMin + f0;
    int i;
    for (i = 0; i < NN; i++) {
        f += fStep;
        if (f > fMax) f -= (fMax - fMin);
        down ? (phaseAccum -= f) : (phaseAccum += f);
        samps[i] = std::polar(ampl, phaseAccum);
    };
    phaseAccum -= floor(phaseAccum / (2 * M_PI)) * 2 * M_PI;

    return i;
}

#endif //LORA_TEST_CHIRPGENERATOR_H
