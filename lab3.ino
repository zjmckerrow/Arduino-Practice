/*
 * 
 * Created by: Zach McKerrow
 * CWID: 11702949
 * ECE 486
 * Date: 2/20/20
 * 
 * Description: Takes serial input and displays it to the 7-segment display. The system resets when the
 *              watchdog timer passes 4 seconds.  Valid input is a decimal value between 0 and 15.
 * 
 */
 
#include <avr/wdt.h>              // include the watchdog-timer library.

void WatchdogSetup();      
void SegmentSetup();
void SegmentCount(int countArray[17][7], int count);
void BlinkDP();

volatile boolean resetFlag = false;   // flag to be set by the watchdog interrupt.

/*
 * Function name: setup()
 * Parameters: none
 * Description: initialize the pins being used, set up the 7-segment display, and start the watchdog timer.
 */
void setup() {
  
  Serial.begin(9600);  // initialize the serial monitor.
  SegmentSetup();      // put the 7-segment display in its default state.
  WatchdogSetup();     // configure the watchdog timer.
  wdt_reset();         // reset the watchdog timer.

}

/*
 * Function name: loop()
 * Parameters: none
 * Description: contains variable initialization and the main loop of the pragram..
 */
void loop() {

  wdt_reset();
  int countArray[17][7] = {       // initialize an array of the 7-segment display options.
    {0,0,0,0,0,0,1}, // 0
    {0,1,1,0,1,1,1}, // 1
    {0,0,1,1,0,0,0}, // 2
    {0,0,1,0,1,0,0}, // 3
    {0,1,0,0,1,1,0}, // 4
    {1,0,0,0,1,0,0}, // 5
    {1,0,0,0,0,0,0}, // 6
    {0,1,1,0,1,0,1}, // 7
    {0,0,0,0,0,0,0}, // 8
    {0,1,0,0,1,0,0}, // 9
    {0,1,0,0,0,0,0}, // A
    {1,0,0,0,0,1,0}, // B
    {1,0,0,1,0,0,1}, // C
    {0,0,1,0,0,1,0}, // D
    {1,0,0,1,0,0,0}, // E
    {1,1,0,1,0,0,0}, // F
    {1,0,1,1,1,0,0}  // Error
  };

  boolean valid = true;               // initialize the valid input flag.
  unsigned long previousMillis = 0;   // initialize  millis to 0.
  unsigned long currentMillis = 0;
  Serial.println("Booting up...");
  Serial.println("Enter a number from 0 to 15 in decimal.");
  wdt_reset();
  previousMillis = millis();          // set previousMillis to the current time.
  
  while(1) {                          // the main continuous loop of the program.

    if(resetFlag == true) {           // if the watchdog timed out and the resetFlag was turned on.
      BlinkDP();                      // call BlinkDP() function.
      previousMillis = millis();      // set previousMillis to the current time.
    } 

    if(Serial.available() > 0) {

      wdt_reset();                          // reset the watchdog timer after receiving input
      currentMillis = millis();             // set currentMillis to the time when input was received.
      String input = Serial.readString();   // read the serial input
      unsigned long reactionTime = (unsigned long)(currentMillis - previousMillis); // calculate the reaction time.
      int seconds = reactionTime / 1000;        // calculate the seconds.
      int milliSeconds = reactionTime % 1000;   // calculate the milliseconds. 
      input.trim();                             // trim the newline off of the input.
      wdt_reset();

      for(int i = 0; i < input.length(); i++) {   // check if there are non-numeric chars in the input.
        if(!isDigit(input.charAt(i))) {
          valid = false;                          // set input to invalid if not numbers.
        }
      }
      wdt_reset();

      if(valid == false) {                        // if input invalid.
        Serial.println("[Error] Invalid input... Input must be a decimal value.");
        Serial.print("reaction time = ");         // output reaction time.
        Serial.print(seconds);
        Serial.print(".");
        Serial.println(milliSeconds);
        SegmentCount(countArray, 16);             // set the 7-segment to the error display.
        wdt_reset();
        valid = true;                             // reset the valid input flag.
      }
      else if(valid == true) {                        // if the input is valid.
        int decimalInput = input.toInt();             // convert it to an int.
        if(decimalInput > 15 || decimalInput < 0) {   // check if the input is within the required range.
          Serial.println("[Error] Invalid input... Input must be between 0 and 15.");
          SegmentCount(countArray, 16);               // set the 7-segment to the error display.
        }
        else if(decimalInput >= 0 && decimalInput <= 15) {  // if the input is valid and within range.
          Serial.print("Input: ");                          // print the input.
          if(decimalInput < 10) {
            Serial.println(input);
          }
          else {
            if(decimalInput == 10) {
              Serial.println("A");
            }
            else if(decimalInput == 11) {
              Serial.println("B");
            }
            else if(decimalInput == 12) {
              Serial.println("C");
            }
            else if(decimalInput == 13) {
              Serial.println("D");
            }
            else if(decimalInput == 14) {
              Serial.println("E");
            }
            else if(decimalInput == 15) {
              Serial.println("F");
            }
          }
          Serial.print("reaction time = ");                 // print the reaction time.
          Serial.print(seconds);
          Serial.print(".");
          Serial.println(milliSeconds);
          SegmentCount(countArray, decimalInput);           // set the 7-segment display.
        }
      }
      wdt_reset();
      Serial.println("Enter a number from 0 to 15 in decimal.");
      previousMillis = millis();     // set previousMillis to the current time.
      wdt_reset();
    }
  }
   
}

/*
 * Function name: ISR()
 * Parameters: WDT_vect (signal from watchdog telling that a timeout occured.
 * Description: Runs when the watchdog timer times out. Sets a flag indicating a timeout.
 */
ISR(WDT_vect) {

  resetFlag = true;   // set resetFlag indicating a timeout to the rest of the system.
  wdt_reset();
  
}

/*
 * Function name: BlinkDP()
 * Parameters: none
 * Description: Blinks the on-board LED every 0.5 seconds for 5 seconds and then
 *              resets the 7-segment to it's default.
 */
void BlinkDP() {
  wdt_reset();
  int pin = 2;                            
  for(int i = 0; i < 7; i++) {        // loop through the 7 signals in each spot in the array.
    digitalWrite(pin, 1);             // turn off the 7-segment display
    pin++;                            // move to the next pin/segment in the display.
  }
  wdt_reset();
  Serial.println("[Error] Watchdog timed out. Resetting system...");
  wdt_reset();
  unsigned long startTime = millis(); // initialize the start time for the 5 second delay.
  unsigned long endTime = startTime;  // initialize the end time for the 5 second delay.

  wdt_reset();
  while((endTime - startTime) <= 5000) {  // while the 5 seconds is not yet reached.
    digitalWrite(LED_BUILTIN, HIGH);      // turn on the on-board LED.
    delay(500);                           // delay for 0.5 seconds.
    digitalWrite(LED_BUILTIN, LOW);       // turn off the on-board LED.
    delay(500);                           // delay for 0.5 seconds.
    wdt_reset(); 
    endTime = millis();                   // set the new end time.
  }
  wdt_reset();

  digitalWrite(8, LOW);                   // set up the 7-segment dispaly for boot up 
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(LED_BUILTIN, LOW);          // turn off the dp LED
  
  wdt_reset();
  Serial.println("Booting up...");
  Serial.println("Enter a number from 0 to 15 in decimal.");
  wdt_reset();
  Serial.readString();                    // read input within the function to ignore it.
  resetFlag = false;                      // turn the resetFlag off.
}

/*
 * Function name: WatchdogSetup()
 * Parameters: none
 * Description: configure the watchdog timer to perform as desired for my program.
 */
void WatchdogSetup() {
  cli();          // disable interupts while configuring.
  wdt_reset();
  WDTCSR |= (1 << WDCE) | (1 << WDE);   // go into configuration mode.
  WDTCSR =  (1 << WDIE) | (0 << WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0); // configure
  sei();          // re-enable interrupts
}

/*
 * Function name: SegmentSetup()
 * Parameters: none
 * Description: Sets up the seven segment I/O pins and LEDs.
 */
void SegmentSetup() {
  pinMode(8, OUTPUT);             // initialize the "g" pin for the 7-segment display as an output.
  pinMode(7, OUTPUT);             // initialize the "a" pin for the 7-segment display as an output.
  pinMode(6, OUTPUT);             // initialize the "e" pin for the 7-segment display as an output.
  pinMode(5, OUTPUT);             // initialize the "c" pin for the 7-segment display as an output.
  pinMode(4, OUTPUT);             // initialize the "f" pin for the 7-segment display as an output.
  pinMode(3, OUTPUT);             // initialize the "d" pin for the 7-segment display as an output.
  pinMode(2, OUTPUT);             // initialize the "b" pin for the 7-segment display as an output.
  pinMode(LED_BUILTIN, OUTPUT);   // initialize on-board LED as an output.
  digitalWrite(8, LOW);           // set up the 7-segment dispaly for boot up 
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(LED_BUILTIN, LOW);   // turn off the LED representing the decimal point.
}

/*
 * Function name: SegmentCount()
 * Parameters: int countArray[17][7] (2-D array holding the 7-segment LED signals)
 *             int input (decimal user input)
 * Description: assigns the "on" or "off" signal to each 7-segment LED.
 */
void SegmentCount(int countArray[17][7], int input){
  
  int pin = 2;                            
  for(int i = 0; i < 7; i++) {                  // loop through the 7 signals in each spot in the array.
    digitalWrite(pin, countArray[input][i]);    // set the 7-segment display to its correct count value.
    pin++;                                      // move to the next pin/segment in the display.
  }
  
}
