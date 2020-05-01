/*
 * 
 * Created by: Zach McKerrow
 * CWID: 11702949
 * ECE 486
 * Date: 2/26/20
 * 
 * Description: Takes analog voltage input from an off-board potentiometer and
 *              converts this analog voltage to a digital value from 0 to 1023
 *              30 times when 'c' is provided via the serial monitor.
 * 
 */
 
#include <avr/wdt.h>              // include the watchdog-timer library.

void WatchdogSetup();             // declare the function used to configure the watchdog.

/*
 * Function name: setup()
 * Parameters: none
 * Description: initialize the pins being used and start the watchdog timer.
 */
void setup() {
  
  Serial.begin(9600);  // initialize the serial monitor.
  pinMode(A0, INPUT);  // initalize the A0 pin as an input.
  WatchdogSetup();     // configure the watchdog timer.
  wdt_reset();         // reset the watchdog timer.

}

/*
 * Function name: loop()
 * Parameters: none
 * Description: contains variable initialization and the main loop of the pragram..
 */
void loop() {

  Serial.println();
  Serial.println("Board was reset.");
  Serial.println("Enter 'c' to start a set of conversions.");
  wdt_reset();                        // reset the watchdog after the serial prompt.
  
  while(1) {                          // the main continuous loop of the program.

    if(Serial.available() > 0) {

      wdt_reset();                          // reset the watchdog timer after receiving input.
      String input = Serial.readString();   // read the serial input
      input.trim();                         // trim the newline off of the input.

      if(input != "c") {                    // if input is invalid.
        
        Serial.println("Error: invalid user input - the only valid user input is 'c'.");
        Serial.println();
        
      }
      else if(input == "c") {       // if the input is valid.

        Serial.println("Starting a set of conversions:");
        convert();                  // call the convert() function to do 30 conversions.
          
      }

      Serial.println("Enter 'c' to start a set of conversions.");
      wdt_reset();
      
    }
    
  }
   
}

/*
 * Function name: convert()
 * Parameters: none
 * Description: reads an analog value and converts it to digital 30 times.
 */
void convert() {

  int totalTime = 0;            // initialize the total time for all conversions.
  
  for(int i = 1; i < 31; i++) {
    unsigned long previousMillis = micros();  // store the begin time.
    int analog = analogRead(A0);              // read and convert the analog voltage.
    unsigned long currentMillis = micros();   // store the end time.
    unsigned long conversionTime = (unsigned long)(currentMillis - previousMillis); // calculate the conversion time 
    totalTime = totalTime + conversionTime;   // add to total time.
    Serial.print("#");  
    Serial.print(i);
    Serial.print(": digital value = ");
    if(analog < 0x100) {                      // print leading 0s if necessary
      if(analog < 0x10) {
        Serial.print("0");
      }
      Serial.print("0");
    }
    Serial.print(analog, HEX);              // print digital value in hex         
    Serial.print("  Time = ");
    Serial.print(conversionTime);
    Serial.println(" usecs");
    Serial.readString();                    // read input to ignore during iterations.
    wdt_reset();
    delay(500);                             // allow time to change potentiometer value   
  }

  float avg = (float)(totalTime / 30);      // calculate avg conversion time.
  Serial.println();
  Serial.print("avg conversion time = ");
  Serial.print(avg);
  Serial.println(" usecs");
  Serial.println();
  wdt_reset();
          
}

/*
 * Function name: WatchdogSetup()
 * Parameters: none
 * Description: configure the watchdog timer to perform as desired for the program.
 */
void WatchdogSetup() {
  cli();          // disable interupts while configuring.
  wdt_reset();
  WDTCSR |= (1 << WDCE) | (1 << WDE);   // go into configuration mode.
  WDTCSR =  (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0); // configure
  sei();          // re-enable interrupts
}
