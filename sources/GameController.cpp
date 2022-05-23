#include "../headers/GameController.h"

void GameController::readMesure()
{
    queue.call(callback(this, &GameController::record));
}

void GameController::compute()
{
    int i = 9;
    while(lastTenValues[i] == 0 || i >= 0)
    {
        i--;
    }

    int indexDiff = i + 1;

    freq = 1 / ((lastTenValues[0] - lastTenValues[i]) / indexDiff); //Calcul de la fréquence d'appui
}

void GameController::record()
{
    float value = Kernel::get_ms_count();

    int i = 0;
    while(lastTenValues[i] == 0 || i < 10){
        i++;
    }
    if(lastTenValues[i] == 0)
    {
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
