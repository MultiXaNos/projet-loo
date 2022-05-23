#include "../headers/GameController.h"

void GameController::readMesure()
{
    float value = 0;
    queue.call(callback(this, &GameController::record), value);
}

void GameController::compute()
{
    float freqValue = 0;
    for(int iArray = 0; iArray < 10; iArray++)
    {
        freqValue += lastTenValues[iArray];
    }
    freqValue /= 10;

    freq = freqValue;
}

void GameController::record(float value)
{
    int i = 0;
    while(lastTenValues[i] == 0 || i < 10){
        i++;
    }
    if(lastTenValues[i] == 0){
        lastTenValues[i] = 0.0;
    } else {
        for ( int j = 0; j < 9; j++){
            lastTenValues[j] = lastTenValues[j+1];
        }
        lastTenValues[9] = 0.0;
    }
}
