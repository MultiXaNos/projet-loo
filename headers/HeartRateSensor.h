#ifndef _HEART_RATE_SENSOR_H_
#define _HEART_RATE_SENSOR_H_

#include "IPeriphAcq.h"
#include "GroveEarbudSensor.h"

class HeartRateSensor : IPeriphAcq {

    public:
        HeartRateSensor(PinName pinName) : sensor(pinName), queue(32 * EVENTS_EVENT_SIZE){
            ticker.attach(callback(this, &HeartRateSensor::readMesure), std::chrono::milliseconds(10));
        };
        ~HeartRateSensor();
        float getFreq() { return this->freq; };
    
    private:
        Ticker ticker;
        EventQueue queue;
        GroveEarbudSensor sensor;
        float lastTenValues[10] = {0};
        float freq;
        void readMesure();
        void compute();
        void record(float value);
};

#endif