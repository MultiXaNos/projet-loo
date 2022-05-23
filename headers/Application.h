#include "IApplication.h"
#include "GameController.h"
#include "HeartRateSensor.h"
#include "Circular_LED.h"

#define WINTIME 5s

class Application : IApplication {

    public:
        Application() : heartRateSensor(PA_3), gameController(PA_2), circularLed(PA_10, PB_3){};
        void Init(); 
        void Run();

    private:
        volatile bool isFinished;
        void finishGame();
        Timeout timeOut;
        HeartRateSensor heartRateSensor;
        GameController gameController;
        Circular_LED circularLed;
};