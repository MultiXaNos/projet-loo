#include "IApplication.h"
#include "GameController.h"
#include "HeartRateSensor.h"

class Application : IApplication {

    public:
        Application() : heartRateSensor(PA_3), gameController(PA_2){};
        void Init(); 
        void Run();

    private:
        HeartRateSensor heartRateSensor;
        GameController gameController;

};