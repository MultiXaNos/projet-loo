#include "PeriphAcq.h"

class GameController : PeriphAcq {

    public:
        virtual float getFreq();
    
    protected:
        virtual void readMesure();
        virtual void compute();
        virtual void record();

};