/* Copyright C2014 ARM, MIT License
 *
 * Author: Doug Anson (doug.anson@arm.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files the "Software", to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
 #ifndef _GROVE_EARBUD_SENSOR_H_
 #define _GROVE_EARBUD_SENSOR_H_
 
 
 #include "mbed.h"
 
 // ********* BEGIN Tunables *****************
 
 #define NUM_SLOTS              6           // set higher for greater accuracy (but slower callback frequency).. 
 #define HEARTPULSE_DUTY        2000        // Change to follow your system's request. System returns error if the duty overtrips by 2 seconds. (in MS)
 
 #define HEARTRATE_OFF          0           // earbud sensor is offline
 #define HEARTRATE_MIN          10          // min heartrate
 #define HEARTRATE_MAX          250         // max heartrate
 
 // *********  END Tunables  *****************
  
 // Callback function signature
 typedef void (GroveEarbudSensorCallback)(float,void *);
 
 /**
 * GroveEarbudSensor 
 * GroveEarbudSensor a simple API to receive heartrate telemetry from the Grove Earbud Sensor
 *
 * Based upon/Credit: http://www.seeedstudio.com/wiki/Grove_-_Ear-clip_Heart_Rate_Sensor
 *
 * Example Project: http://mbed.org/users/ansond/code/grove-earbud-sensor-sample/ 
 *
 * @code
 *
 
 #include "mbed.h"
 
// Blinky
DigitalOut led(LED1);
 
// Our sensor as an InterruptIn
InterruptIn sensor(D0);
 
// Grove Earbud Sensor include
#include "GroveEarbudSensor.h"
 
// callback for receiving heartrate values
void heartrateCallback(float heartrate,void *data) {
    printf("Callback: heartrate = %.1f\r\n",heartrate);
}
 
int main()
{   
    // announce
    printf("Grove Earbud Sensor Example v1.0.0\r\n");
    
    // allocate the earbud sensor
    printf("Allocating earbud sensor instance...\r\n");
    GroveEarbudSensor earbud(&sensor); 
    
    // register our callback function
    printf("registering callback...\r\n");
    earbud.registerCallback(heartrateCallback);
    
    // begin main loop
    printf("Beginning main loop...\r\n");
    while (true) {
        // blink... 
        led = !led; 
        wait(0.5);
        
        // we can also call directly 
        //printf("Direct: heartrate = %.1f\r\n",earbud.getHeartRate());
    }
}
 
 * @endcode
 *
 */    
 
 class GroveEarbudSensor {
     private:
        SerialBase                 *m_pc;
        InterruptIn               *m_rx;
        volatile unsigned long     m_temp[NUM_SLOTS];
        volatile unsigned long     m_sub;
        volatile unsigned char     m_counter;
        volatile bool              m_data_effect;
        float                      m_heartrate;
        GroveEarbudSensorCallback *m_cb_fn;
        void                      *m_cb_data;
        Timer                     *m_timer;
        bool                       m_internal_interrupt_instance;
         
     public:
        /**
        Default constructor
        @param rx input InterruptIn instance
        @param pc input RawSerial instance for debugging (if NULL, no debugging output will occur in the library)
        */
        GroveEarbudSensor(InterruptIn *rx, SerialBase *pc = NULL);
        
        /**
        constructor for internalized InterruptIn usage
        @param rx interrupt_pin InterruptIn pin name
        @param pc input RawSerial instance for debugging (if NULL, no debugging output will occur in the library)
        */
        GroveEarbudSensor(PinName interrupt_pin, SerialBase *pc = NULL);
        
        /**
        Default destructor
        */
        virtual ~GroveEarbudSensor();
        
        /**
        registerCallback - Register callback function 
        @param cb_fn - callback function of type GroveEarbudSensorCallback
        @param cb_data - optional callback data to provide upon callback invocation (default - NULL)
        */
        void registerCallback(GroveEarbudSensorCallback *cb_fn,void *cb_data = NULL);
        
        /**
        getHeartRate - get the last sampled heartrate
        @return heartrate - the last calculated heartrate (may also be one of HEARTRATE_OFF, HEARTRATE_MIN, or HEARTRATE_MAX)
        */
        float getHeartRate(void);
        
        /**
        interrupt() - interrupt handler for our instance - not normally invoked manually
        */
        void interrupt(void);
        
    protected:
        void initSummationArray(void);
        void sumAndInvokeCallback(void);
 };
 
 #endif // _GROVE_EARBUD_SENSOR_H_