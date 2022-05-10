#include "PeriphAcq.h"
#include "mbed.h"

class GameController : PeriphAcq {

    public:
        GameController();
        ~GameController();
        float getFreq();
    
    private:
        InterruptIn btnInterrupt;
        EventQueue queue;
        Thread eventThread;
        void readMesure();
        void compute();
        void record();

};