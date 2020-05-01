/*
 * 
 * Created by: Zach McKerrow
 * CWID: 11702949
 * ECE 486
 * Date: 4/01/20
 * 
 * Description: Takes analog voltage input from an off-board potentiometer and
 *              converts this analog voltage to a digital value from 0 to 1023
 *              30 times.  
 *              If 'a' is entered the AnalogRead() function is used.
 *              If 'b' is entered then polling and port manipulation is used.
 *              If 'c' is entered then interrupts are used.
 * 
 */
 
#include <avr/wdt.h>              // include the watchdog-timer library.

void WatchdogSetup();             // declare the function used to configure the watchdog.
void convertAnalogRead();         // declare the function used for the "a", AnalogRead option.
void convertPolling();            // declare the function used for the "b", Polling option.
void convertInterrupts();         // declare the function used for the "c", Interrupts option.

unsigned short interruptData;         // declare the data global used during the interrupts.
unsigned long interruptCurrentTime;   // declare the timing global used during the interrupts.

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
  Serial.println("Select a type of conversion to perform ('a' for AnalogRead; 'b' for polling; 'c' for interrupts)");
  wdt_reset();                        // reset the watchdog after the serial prompt.
  
  while(1) {                          // the main continuous loop of the program.

    if(Serial.available() > 0) {

      wdt_reset();                          // reset the watchdog timer after receiving input.
      String input = Serial.readString();   // read the serial input
      input.trim();                         // trim the newline off of the input.

      if(input != "a" && input != "b" && input != "c") {     // if input is invalid.
        
        Serial.println("Error: invalid user input - the only valid user inputs are 'a', 'b', 'c'.");
        Serial.println();
        
      }
      else if(input == "a") {   // if 'a' is entered

        Serial.println("Starting a set of conversions using AnalogRead:");
        convertAnalogRead();    // call the AnalogRead(0 conversion function.
        
      }
      else if(input == "b") {   // if 'b' is entered

        Serial.println("Starting a set of conversions using polling and port manipulation:");
        convertPolling();       // call the polling conversion function.
        
      }
      else if(input == "c") {   // if 'c' is entered

        Serial.println("Starting a set of conversions using interrupts:");
        convertInterrupts();    // call the interrupt-driven conversion function.
          
      }

      Serial.println("Select a type of conversion to perform ('a' for AnalogRead; 'b' for polling; 'c' for interrupts)");
      wdt_reset();
      
    }
    
  }
   
}

/*
 * Function name: convertAnalogRead()
 * Parameters: none
 * Description: reads an analog value and converts it to digital 30 times
 *              using the analogRead() function.
 */
void convertAnalogRead() {

  int totalTime = 0;            // initialize the total time for all conversions.
  
  for(int i = 1; i < 31; i++) {
    unsigned long previousMillis = micros();  // store the begin time.
    int analog = analogRead(A0);              // read and convert the analog voltage.
    unsigned long currentMillis = micros();   // store the end time.
    unsigned long conversionTime = (unsigned long)(currentMillis - previousMillis); // calculate the conversion time 
    totalTime = totalTime + conversionTime;   // add to total time.
    Serial.print("#");  
    Serial.print(i);
    Serial.print(":");
    if(i < 10) {
      Serial.print(" ");
    }
    Serial.print(" digital value = ");
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
  Serial.print("AnalogRead avg conversion time = ");
  Serial.print(avg);
  Serial.println(" usecs");
  Serial.println();
  wdt_reset();
          
}

/*
 * Function name: convertPolling()
 * Parameters: none
 * Description: reads an analog value and converts it to digital 30 times 
 *              using polling and port manipulation.
 */
void convertPolling() {

  int totalTime = 0;            // initialize the total time for all conversions.
  ADMUX = 0b01000000;           // set up the ADC's reference voltage, bits, and input channel.
  ADCSRA = 0b10000111;          // set up the ADC's clock prescalar, and enable the ADC.

  for(int i = 1; i < 31; i++) {

    unsigned long previousMillis = micros();    // store the begin time of the conversion.
    bitWrite(ADCSRA, 6, 1);                     // initiate a conversion by flipping the ADSC to 1.
    while(ADCSRA == 0b11000111);                // wait for the ADSC bit to flip back to 0.
    unsigned short analog = ADC;                // read the data from the ADC register.
    unsigned long currentMillis = micros();     // store the end time of the conversion.
    unsigned long conversionTime = (unsigned long)(currentMillis - previousMillis);   // calculate the conversion time.
    totalTime = totalTime + conversionTime;     // add conversion time to the total.
    Serial.print("#");  
    Serial.print(i);
    Serial.print(":");
    if(i < 10) {
      Serial.print(" ");
    }
    Serial.print(" digital value = ");
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
  Serial.print("Polling avg conversion time = ");
  Serial.print(avg);
  Serial.println(" usecs");
  Serial.println();
  wdt_reset();
  
}

/*
 * Function name: convertInterrupts()
 * Parameters: none
 * Description: reads an analog value and converts it to digital 30 times 
 *              using interrupts.
 */
void convertInterrupts() {

  int totalTime = 0;        // intitialize the total time for all conversions.
  ADMUX = 0b01000000;       // set up the ADC's reference voltage, bits, and input channel.
  ADCSRA = 0b10001111;      // set up the ADC's clock prescalar, and enable the ADC and interrupts.
  sei();

  for(int i = 1; i < 31; i++) {

    unsigned long previousMillis = micros();    // store the begin time of the conversion.
    bitWrite(ADCSRA, 6, 1);                     // initiate a conversion by flipping the ADSC to 1. 
    Serial.print("#");  
    Serial.print(i);
    Serial.print(":");
    if(i < 10) {
      Serial.print(" ");
    }
    Serial.print(" digital value = ");
    if(interruptData < 0x100) {                 // print leading 0s if necessary
      if(interruptData < 0x10) {
        Serial.print("0");
      }
      Serial.print("0");
    }
    unsigned long conversionTime = (unsigned long)(interruptCurrentTime - previousMillis);  // calculate the conversion time.
    totalTime = totalTime + conversionTime;        // add conversion time to the total.
    Serial.print(interruptData, HEX);              // print digital value in hex         
    Serial.print("  Time = ");
    Serial.print(conversionTime);
    Serial.println(" usecs");
    Serial.readString();                    // read input to ignore during iterations.
    wdt_reset();
    delay(500);                             // allow time to change potentiometer value
       
  }

  float avg = (float)(totalTime / 30);      // calculate avg conversion time.
  Serial.println();
  Serial.print("Interrupt-driven avg conversion time = ");
  Serial.print(avg);
  Serial.println(" usecs");
  Serial.println();
  wdt_reset();
  
}

/*
 * Function name: ISR()
 * Parameters: ADC_vect
 * Description: ISR triggered by the ADC_vect when a conversion is finished.
 */
ISR(ADC_vect) {

  interruptData = ADC;              // store the converted value from the ADC register.
  interruptCurrentTime = micros();  // store the end time of the conversion.
  
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
