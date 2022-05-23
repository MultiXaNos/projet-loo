#include "../headers/HeartRateSensor.h"

void HeartRateSensor::readMesure()
{
    queue.call(callback(this, &HeartRateSensor::record));
}

void HeartRateSensor::record()
{
    float value = sensor.getHeartRate();

    int i = 0;
    while(lastTenValues[i] == 0 || i < 10){
        i++;
    }
    if(lastTenValues[i] == 0){
        lastTenValues[i] = value;
    } 
    else 
    {
        for (int j = 0; j < 9; j++)
        {
            lastTenValues[j+1] = lastTenValues[j];
        }
        lastTenValues[0] = value;
    }
}

void HeartRateSensor::compute()
{
    float freqValue = 0;

    for(int i = 0; i < 10; i++)
    {
        freqValue += lastTenValues[i];
    }

    freq = freqValue / 10; //Calcul de la moyenne des fréquences remontées par le driver pour avoir une valeur stable au cours du temps

}
