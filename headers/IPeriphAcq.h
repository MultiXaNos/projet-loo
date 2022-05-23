#ifndef _IPERIPHACQ_H_
#define _IPERIPHACQ_H_

#include "mbed.h"
class IPeriphAcq {

    public:
        virtual float getFreq();
    
    protected:
        virtual void readMesure();
        virtual void compute();
        virtual void record(float value);

};

#endif