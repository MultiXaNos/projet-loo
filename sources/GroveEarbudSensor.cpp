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
 
 #include "../headers/GroveEarbudSensor.h"
 
 // Console logging
 //#define LOG_CONSOLE(...) { if (this->m_pc != NULL) this->m_pc->printf(__VA_ARGS__); }
 #define LOG_CONSOLE(...) { ; }
 
 // Our instance
 GroveEarbudSensor *_grove_earbud_sensor_instance = NULL;
 
 // interrupt function
 void __grove_earbud_sensor_interrupt() { if (_grove_earbud_sensor_instance != NULL) _grove_earbud_sensor_instance->interrupt(); }
 
 // constructor
 GroveEarbudSensor::GroveEarbudSensor(InterruptIn *rx,SerialBase *pc) {
     _grove_earbud_sensor_instance       = this;
     this->m_rx                          = rx;
     this->m_pc                          = pc;
     this->m_sub                         = 0;
     this->m_counter                     = 0;
     this->m_data_effect                 = true;
     this->m_cb_fn                       = NULL;
     this->m_cb_data                     = NULL;
     this->m_heartrate                   = HEARTRATE_OFF;
     this->m_timer                       = new Timer();
     this->m_internal_interrupt_instance = false;
     
     // register the interrupt handler
     if (this->m_rx != NULL) this->m_rx->rise(&__grove_earbud_sensor_interrupt);
          
     // start the timer and initialize the summation array
     if (this->m_timer != NULL) {
         // start the timer
         this->m_timer->start();
     
         // initialize the summation array
         this->initSummationArray();
     }
 }
 
 // constructor
 GroveEarbudSensor::GroveEarbudSensor(PinName interrupt_pin,SerialBase *pc) {
     _grove_earbud_sensor_instance       = this;
     this->m_rx                          = new InterruptIn(interrupt_pin);
     this->m_pc                          = pc;
     this->m_sub                         = 0;
     this->m_counter                     = 0;
     this->m_data_effect                 = true;
     this->m_cb_fn                       = NULL;
     this->m_cb_data                     = NULL;
     this->m_heartrate                   = HEARTRATE_OFF;
     this->m_timer                       = new Timer();
     this->m_internal_interrupt_instance = true;
     
     // register the interrupt handler
     if (this->m_rx != NULL) this->m_rx->rise(&__grove_earbud_sensor_interrupt);
          
     // start the timer and initialize the summation array
     if (this->m_timer != NULL) {
         // start the timer
         this->m_timer->start();
     
         // initialize the summation array
         this->initSummationArray();
     }
 }
 
 // destructor
 GroveEarbudSensor::~GroveEarbudSensor() {
     if (this->m_timer != NULL) delete this->m_timer;
     if (this->m_internal_interrupt_instance == true && this->m_rx != NULL) delete this->m_rx;
 }
 
 // initialize the summation array
 void GroveEarbudSensor::initSummationArray(void) {
    for(int i=0;i<(NUM_SLOTS-1);++i) this->m_temp[i]=0;
    this->m_temp[NUM_SLOTS-1] = this->m_timer->read_ms();
 }
 
 // register callback 
 void GroveEarbudSensor::registerCallback(GroveEarbudSensorCallback *cb_fn,void *cb_data) {
     this->m_cb_fn = cb_fn;
     this->m_cb_data = cb_data;
 }
 
 // get the current heartrate
 float GroveEarbudSensor::getHeartRate(void) { return this->m_heartrate; }
 
 // summation method + internal callback to fire any registered callback fns
 void GroveEarbudSensor::sumAndInvokeCallback(void) {
   if(this->m_data_effect) {
      // summation
      int tmp = 60 * (NUM_SLOTS-1) * 1000;
      this->m_heartrate = tmp/(this->m_temp[NUM_SLOTS-1]-this->m_temp[0]);
      
      // DEBUG/Log
      if (this->m_heartrate > 0) LOG_CONSOLE("heartrate: %d bpm\r\n",this->m_heartrate);
      
      // invoke any callbacks we might have
      if (this->m_cb_fn != NULL) {
          // invoke the callback
          LOG_CONSOLE("invoking callback with heartrate = %d bpm\r\n",this->m_heartrate);
          (*this->m_cb_fn)(this->m_heartrate,this->m_cb_data);
      }
   }
   this->m_data_effect = 1;  //sign bit
}
 
 // interrupt() method for earbud
 void GroveEarbudSensor::interrupt() {
    this->m_temp[this->m_counter] = this->m_timer->read_ms();
    switch(this->m_counter) {
        case 0:
            this->m_sub=this->m_temp[this->m_counter]-this->m_temp[NUM_SLOTS-1];
            break;
        default:
            this->m_sub=this->m_temp[this->m_counter]-this->m_temp[this->m_counter-1];
            break;
    }
    if(this->m_sub > HEARTPULSE_DUTY) { 
        this->m_data_effect = 0; //sign bit
        this->m_counter = 0;
        LOG_CONSOLE("heartrate measure error. Restarting timer..\r\n");
        this->initSummationArray();
        this->m_timer->stop();
        this->m_timer->start();
    }
    if (this->m_counter >= (NUM_SLOTS-1) && this->m_data_effect) {
        this->m_counter = 0;
        this->sumAndInvokeCallback();
    }
    else if(this->m_counter < (NUM_SLOTS-1) && this->m_data_effect) {
        this->m_counter++;
    }
    else {
        this->m_counter = 0;
        this->m_data_effect = 1;
    }
 }