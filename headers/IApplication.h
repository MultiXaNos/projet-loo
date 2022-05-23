#include "mbed.h"

class IApplication {

    public:
        virtual void Init(); 
        virtual void Run(); 
};