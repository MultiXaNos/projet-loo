#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_

#include "IPeriphAcq.h"

class GameController : IPeriphAcq {

    public:
        GameController(PinName pinName) : btnInterrupt(pinName), queue(32 * EVENTS_EVENT_SIZE)
        {
            btnInterrupt.rise(callback(this, &GameController::readMesure));
        };
        ~GameController();
        float getFreq() { return this->freq; };
    
    private:
        InterruptIn btnInterrupt;
        EventQueue queue;
        float lastTenValues[10] = {0};
        float freq;
        void readMesure();
        void compute();
        void record();
};

#endif