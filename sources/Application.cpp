#include "Application.h"
#include <math.h>

void Application::Init()
{
    for(int i = 0; i < 4; i++)
    {
        circularLed.setLevel(24);
        ThisThread::sleep_for(250ms);
        circularLed.setLevel(0);
        ThisThread::sleep_for(250ms);
    }
}

void Application::Run()
{
    bool isAttached = false;
    float heartRate = 0;
    float pushFrequency = 0;
    float rate = 0;

    while(!isFinished) //Tant que le jeu n'est pas terminé
    {
        heartRate = heartRateSensor.getFreq(); //On lit la moyenne des fréquences cardiaques enregistrées
        pushFrequency = gameController.getFreq(); //On lit la fréquence d'appui sur le bouton pousssoir
        rate = (pushFrequency / heartRate) * 24; //On fait un ration qu'on multiplie par 24 pour savoir le nombre de LED à allumer

        if(rate > 24.5) //Si on dépasse le seuil maximal
        {
            rate -= 24; //On soustrait 24 pour avoir le nombre de LED à allumer
        }

        circularLed.setLevel(floor(rate)); //On allume le bon nombre de LED

        if(rate >= 24 && rate <= 24.5) //Si on est compris dans l'intervalle gagnant
        {
            if(!isAttached) //Si le timeout n'est pas déjà linké 
            {
                timeOut.attach(callback(this, &Application::finishGame), WINTIME); //On le link à la méthode correspondante
                isAttached = true; //On dit que le timeout est linké
            }
        }
        else //Sinon
        {
            if(isAttached) //Si le timeout est linké
            {
                timeOut.detach(); //On détache la méthode
                isAttached = false; //On dit que le timeout n'est pas linké
            }
        }
        
        ThisThread::sleep_for(100ms); //On endort le thread pendant 100 ms

    }
}

void Application::finishGame()
{
    isFinished = true; //On dit que le jeu est terminé
}