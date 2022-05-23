#include "../headers/HeartRateSensor.h"

void HeartRateSensor::readMesure()
{
    freq = sensor.getHeartRate();
    queue.call(callback(this, &HeartRateSensor::record), freq);
}

void HeartRateSensor::record(float value)
{
    int i = 0;
    while(lastTenValues[i] == 0 || i < 10){
        i++;
    }
    if(lastTenValues[i] == 0){
        lastTenValues[i] = value;
    } else {
        for ( int j = 0; j < 9; j++){
            lastTenValues[j] = lastTenValues[j+1];
        }
        lastTenValues[9] = value;
    }
}

void HeartRateSensor::compute()
{

}
