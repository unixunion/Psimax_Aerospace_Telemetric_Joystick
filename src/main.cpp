/*

SPI 595
https://forum.arduino.cc/index.php?topic=149470.0

Schematic:
http://jumptuck.com/wp-content/uploads/2011/11/shift-register-hardware1.png

pinOutputReguster:
https://forum.arduino.cc/index.php?topic=75962.0

portRegster:
https://www.arduino.cc/en/Reference/PortManipulation

https://www.arduino.cc/en/Reference/SPI

**/

// #define POLOLU32U4
// #define ARDUIO

// arduino
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <Arduino.h>
#endif

#if defined(__AVR_ATmega32U4__)
#include <AStar32U4.h>
#endif

#include <SPI.h>
#include <avr/pgmspace.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h> 

// custom includes
#include <Button.h>
#include <ButtonController.h>
// #include <FastShiftOut.h>
#include <KSPIO.h>

// defines
#define MOSI 51 // ---> Shift Register Serial Data Input / DS (14)
#define SCK 52 // ---> Shift Register Clock Input / SHCP (11)
#define SHIFTOUT1 50 // slave select ---> STCP (12)

// Fast Shiftout
// FastShiftOut fo = FastShiftOut(SCK, MOSI);
long shiftUpdateTime;

// button to action mapping
const uint8_t const_numButtons = 4;
ButtonToAction buttonAction[const_numButtons] = {
    {VirtualSwitch(5), ATSAS},
    {Momentary(4), ATRCS},
    {Button(10), ATGear},
    {VirtualSwitch(7), ATLight},
};

// blinky
#define LED_BUILTIN 13
#define BLINK_INTERVAL 1000


// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// tasks
void TaskBlink( void *pvParameters );
void TaskButtons( void *pvParameters );
void TaskKSPIO( void *pvParameters );

// put your setup code here, to run once:
void setup() {

    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(SHIFTOUT1, OUTPUT);

    // pinMode(BLINK_LEDPIN, OUTPUT);
    Serial.begin(9600);

    // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
    // because it is sharing a resource, such as the Serial port.
    // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
    if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
    {
      xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
      if ( ( xSerialSemaphore ) != NULL )
        xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
    }

    // start SPI
    SPI.begin();

    // Tasls
    xTaskCreate(
      TaskBlink
      ,  (const portCHAR *)"Blink"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  3    // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );
      
    xTaskCreate(
      TaskButtons
      ,  (const portCHAR *)"Buttons"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );

    xTaskCreate(
      TaskKSPIO
      ,  (const portCHAR *)"KSPIO"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );
    

}

// shift bytes out onto the bus, make sure to
// latch the correct device before caling this.
void updateShiftOut(uint8_t value) {
  long t = millis();
  digitalWrite(SHIFTOUT1, LOW);
  // fo.ShiftOut(value);
  SPI.transfer(value);
  // shiftOut(MOSI, SCK, MSBFIRST, value);
  digitalWrite(SHIFTOUT1, HIGH);
  shiftUpdateTime = millis() - t;
}

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    updateShiftOut(B00000001);
    vTaskDelay( BLINK_INTERVAL / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    updateShiftOut(B00000000);
    vTaskDelay( BLINK_INTERVAL / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskButtons(void *pvParameters)
{
  (void) pvParameters;

  // the button controller
  ButtonController bc = ButtonController();
  bc.init(buttonAction, const_numButtons);

  for (;;)
  {
    ButtonToAction * bta = bc.read();
    for (int i = 0; i<bc.numberButtons(); i++)
    {
      int state = bta[i].btn.read();
      // int state = 0;
      ActionType action = bta[i].type;

      switch(action) {
        case ATNONE:
          break;
        default:
          // See if we can obtain or "Take" the Serial Semaphore.
          // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
          if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
          {
            // We were able to obtain or "Take" the semaphore and can now access the shared resource.
            // We want to have the Serial Port for us alone, as it takes some time to print,
            // so we don't want it getting stolen during the middle of a conversion.
            // print out the state of the button:
            Serial.print(millis());
            Serial.print(" action: ");
            Serial.print(action);
            Serial.print(" reading: ");
            Serial.print(state);
            Serial.print(" shiftTime: ");
            Serial.println(shiftUpdateTime);

            xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
          }
          break;
      };
    }
    vTaskDelay(1);
  }

  
}

void TaskKSPIO(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital LED_BUILTIN on pin 13 as an output.
  KSPIO kspio = KSPIO();

  for (;;) // A Task shall never return or exit.
  {
    kspio.input();
    kspio.output();
    vTaskDelay( 10 / portTICK_PERIOD_MS ); // wait for one second
  }
}

// no loop
void loop() {};
