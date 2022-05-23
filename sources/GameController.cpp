#include "../headers/GameController.h"


void GameController::callbackPush()
{
    queue.call(callback(this, &GameController::readMesure));
}

void GameController::readMesure()
{
    //Récupérer valeur 
    GameController::compute();
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
