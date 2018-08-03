//#include <cmath>
//#include <iostream>
//#include <complex>
//#include <cstring>
//#include <fstream>
//#include <vector>
//#include "LoRaDetector.hpp"
//#include <Pothos/Framework.hpp>
//
//enum LoraDemodState
//{
//    STATE_FRAMESYNC,
//    STATE_DOWNCHIRP0,
//    STATE_DOWNCHIRP1,
//    STATE_QUARTERCHIRP,
//    STATE_DATASYMBOLS,
//};
//using namespace std;
//
//
//int main (){
//    ofstream ocunt;
//    std::string _id;
//    int _freqError;
//    const size_t N = 1 << 6;
//    const size_t _fineSteps = 128;
//    LoRaDetector<float> _detector(N);
//    unsigned char _sync = 0x12;
//    double _thresh;
//    int _fineTuneIndex;
//    float _finefreqError = 0.0;
//    _thresh = -30.0;
//    size_t _mtu = 256;
//    LoraDemodState _state;
//    std::complex<float> * inBuff;
//    std::complex<float> *_chirpTable;
//    std::vector<std::complex<float>> rawBuff;
//    std::vector<std::complex<float>> decBuff;
//    std::vector<std::complex<float>> _upChirpTable;
//    std::vector<std::complex<float>> _downChirpTable;
//    std::vector<std::complex<float>> _fineTuneTable;
//    std::vector<std::complex<float>> _fftBuff(N);
//    std::complex<float> *fftBuff = _fftBuff.data();
//    //init
//    double phase = -M_PI;
//    double phaseAccum = 0.0;
//    for (size_t i = 0; i < N; i++)    // N = 2**SF
//    {
//        phaseAccum += phase;
//        auto entry = std::polar(1.0, phaseAccum);
//        _upChirpTable.emplace_back(std::conj(entry));
//        _downChirpTable.emplace_back(std::complex<float>(entry));
//        phase += (2*M_PI)/N;
//    }
////    //output upchirp file
////    ocunt.open("upchirp.csv");
////    ocunt << _upChirpTable[0].real();
////    if(_chirpTable[0].imag() < 0){
////        ocunt << _upChirpTable[0].imag() << "i";
////    }else{
////        ocunt << "+" << _upChirpTable[0].imag() << "i";
////    }
////    for (int i = 1; i < _upChirpTable.size(); ++i) {
////        ocunt << "," <<_upChirpTable[i].real();
////        if(_upChirpTable[i].imag() <0){
////            ocunt << _upChirpTable[i].imag() << "i";
////        }else{
////            ocunt << "+" << _upChirpTable[i].imag() << "i";
////        }
////    }
////    ocunt.close();
////    //output downchirp file
////    ocunt.open("downchirp.csv");
////    ocunt << _downChirpTable[0].real();
////    if(_downChirpTable[0].imag() < 0){
////
////        ocunt << _downChirpTable[0].imag() << "i";
////    }else{
////        ocunt << "+" << _downChirpTable[0].imag() << "i";
////    }
////    for (int i = 1; i < _downChirpTable.size(); ++i) {
////        ocunt << "," <<_downChirpTable[i].real();
////        if(_downChirpTable[i].imag() <0){
////            ocunt << _downChirpTable[i].imag() << "i";
////        }else{
////            ocunt << "+" << _downChirpTable[i].imag() << "i";
////        }
////    }
////    ocunt.close();
//    // fineSteps 是细调的参数,是在2^SF的基础上再做了_finesteps的划分(2^_finesteps)
//    phaseAccum = 0.0;
//    phase = 2.0 * M_PI / (N * _fineSteps);
//    for (size_t i = 0; i < N * _fineSteps; i++){
//        phaseAccum += phase;
//        auto entry = std::polar(1.0, phaseAccum);
//        _fineTuneTable.emplace_back(std::complex<float>(entry));
//    }
//
//    _fineTuneIndex = 0;
//    _state = STATE_FRAMESYNC;
//    _chirpTable = _upChirpTable.data();
//    // input
//     inBuff = _upChirpTable.data();
//     // work space
//     size_t total = 0;
//    for (size_t i = 0; i < N; i++){
//        auto samp = inBuff[i];
//        auto decd = samp*_chirpTable[i] * _fineTuneTable[_fineTuneIndex];
//        _fineTuneIndex -= _finefreqError * _fineSteps;
//        if (_fineTuneIndex < 0) _fineTuneIndex += N * _fineSteps;
//        else if (_fineTuneIndex >= int(N * _fineSteps)) _fineTuneIndex -= N * _fineSteps;
//        rawBuff.emplace_back(std::complex<float>(samp));	//只是采样没用做dechirp
//        decBuff.emplace_back(std::complex<float>(decd));	//dechirp信号给decter,送到fft向量中
//        _detector.feed(i, decd);
//    }
//    //output downchirp file
//    ocunt.open("dechirp.csv");
//    ocunt << decBuff[0].real();
//    if(decBuff[0].imag() < 0){
//        ocunt << decBuff[0].imag() << "i";
//    }else{
//        ocunt << "+" << decBuff[0].imag() << "i";
//    }
//    for (int i = 1; i <decBuff.size(); ++i) {
//        ocunt << "," <<decBuff[i].real();
//        if(decBuff[i].imag() <0){
//            ocunt << decBuff[i].imag() << "i";
//        }else{
//            ocunt << "+" << decBuff[i].imag() << "i";
//        }
//    }
//    ocunt.close();
//    float power = 0.0;
//    float powerAvg = 0.0;
//    float snr;
//    float fIndex = 0.0;
//    short _prevValue =0;
//    auto value = _detector.detect(power, powerAvg, fIndex, fftBuff);	//value is maxindex
//    snr = power - powerAvg;
//    const bool squelched = (snr < _thresh);
//    //output fftBuff file
//    ocunt.open("fft.csv");
//    ocunt << _fftBuff[0].real();
//    if(_fftBuff[0].imag() < 0){
//        ocunt << _fftBuff[0].imag() << "i";
//    }else{
//        ocunt << "+" << _fftBuff[0].imag() << "i";
//    }
//    for (int i = 1; i <_fftBuff.size(); ++i) {
//        ocunt << "," <<_fftBuff[i].real();
//        if(_fftBuff[i].imag() <0){
//            ocunt << _fftBuff[i].imag() << "i";
//        }else{
//            ocunt << "+" << _fftBuff[i].imag() << "i";
//        }
//    }
//    switch (_state)
//    {
//        ////////////////////////////////////////////////////////////////
//        case STATE_FRAMESYNC:
//            ////////////////////////////////////////////////////////////////
//        {
//            //format as observed from inspecting RN2483
//            bool syncd = not squelched and (_prevValue+4)/8 == 0;	//prev-maxIndex is (0 3)
//            bool match0 = (value+4)/8 == unsigned(_sync>>4);	//maxIndex 's range (4 11)
//            bool match1 = false;
//
//            //if the symbol matches sync word0 then check sync word1 as well
//            //otherwise assume its the frame sync and adjust for frequency error
//            if (syncd and match0)
//            {
//                int ft = _fineTuneIndex;
//                for (size_t i = 0; i < N; i++)
//                {
//                    auto samp = inBuff[i + N];
//                    auto decd = samp*_chirpTable[i] * _fineTuneTable[ft];
//                    ft -= _finefreqError * _fineSteps;
//                    if (ft < 0) ft += N * _fineSteps;
//                    else if (ft >= int(N * _fineSteps)) ft -= N * _fineSteps;
//                    rawBuff[i+N] = samp;
//                    decBuff[i+N] = decd;
//                    _detector.feed(i, decd);
//                }
//                auto value1 = _detector.detect(power,powerAvg,fIndex);
//                //format as observed from inspecting RN2483
//                match1 = (value1+4)/8 == unsigned(_sync & 0xf);
//            }
//            if (syncd and match0 and match1)
//            {
//                total = 2*N;
//                _state = STATE_DOWNCHIRP0;
//                _chirpTable = _downChirpTable.data();
//                _id = "SYNC";
//            }
//
//                //otherwise its a frequency error
//            else if (not squelched)
//            {
//                total = N - value;
//                _finefreqError += fIndex;
//                std::stringstream stream;
//                stream.precision(4);
//                stream << std::fixed << "P " << fIndex;
//                _id = stream.str();
//                //               _id = "P " + std::to_string(fIndex);
//            }
//
//                //just noise
//            else
//            {
//                total = N;
//                _finefreqError = 0;
//                _fineTuneIndex = 0;
//                _id = "";
//            }
//
//        } break;
//
//            ////////////////////////////////////////////////////////////////
//        case STATE_DOWNCHIRP0:
//            ////////////////////////////////////////////////////////////////
//        {
//            _state = STATE_DOWNCHIRP1;
//            total = N;
//            _id = "DC";
//            int error = value;
//            if (value > N/2) error -= N;
//            //std::cout << "error0 " << error << std::endl;
//            _freqError = error;
//        } break;
//
////            ////////////////////////////////////////////////////////////////
////        case STATE_DOWNCHIRP1:
////            ////////////////////////////////////////////////////////////////
////        {
////            _state = STATE_QUARTERCHIRP;
////            total = N;
////            _chirpTable = _upChirpTable.data();
////            _id = "";
////            _outSymbols = Pothos::BufferChunk(typeid(int16_t), _mtu);
////
////            int error = value;
////            if (value > N/2) error -= N;
////            //std::cout << "error1 " << error << std::endl;
////            _freqError = (_freqError + error)/2;
////
////            this->emitSignal("error", _freqError);
////            this->emitSignal("power", power);
////            this->emitSignal("snr", snr);
////        } break;
////
////            ////////////////////////////////////////////////////////////////
////        case STATE_QUARTERCHIRP:
////            ////////////////////////////////////////////////////////////////
////        {
////            _state = STATE_DATASYMBOLS;
////
////            total = N/4 + (_freqError / 2);
////            _finefreqError += (_freqError / 2);
////
////            _symCount = 0;
////            _id = "QC";
////        } break;
////
////            ////////////////////////////////////////////////////////////////
////        case STATE_DATASYMBOLS:
////            ////////////////////////////////////////////////////////////////
////        {
////            total = N;
////            _outSymbols.as<int16_t *>()[_symCount++] = int16_t(value);
////            if (_symCount >= _mtu or squelched)
////            {
////                //for (size_t j = 0; j < _symCount; j++)
////                //    std::cout << "demod[" << j << "]=" << _outSymbols.as<const uint16_t *>()[j] << std::endl;
////                Pothos::Packet pkt;
////                pkt.payload = _outSymbols;
////                pkt.payload.length = _symCount*sizeof(int16_t);
////                this->output(0)->postMessage(pkt);
////                _finefreqError = 0;
////                _state = STATE_FRAMESYNC;
////            }
////            std::stringstream stream;
////            stream.precision(4);
////            stream << std::fixed << "S" << _symCount << " " << fIndex;
////            _id = stream.str();
////            //_id = "S" + std::to_string(_symCount) + " " + std::to_string(fIndex);
////
////            // _finefreqError += fIndex;
////
////        } break;
//
//    }
//    _prevValue = value;
//    return 0;
//}