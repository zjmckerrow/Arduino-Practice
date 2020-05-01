/*
 * 
 * Created by: Zach McKerrow
 * CWID: 11702949
 * ECE 484
 * Date: 2/12/20
 * 
 * Description: Blinks 2 LEDs out-of-phase when the toggle switch is turned "on" and counts the number of state switches 
 *              between the "on" and "off" positions and displays the count on the 7-segment display.
 * 
 */

int StateChange(int count);
void SegmentCount(int countArray[16][7], int count);

/*
 * Function name: setup()
 * Parameters: none
 * Description: initialize the pins being used and set up the 7-segment display.
 */
void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);   // initialize digital pin LED_BUILTIN as an output.
  pinMode(11, OUTPUT);            // initialize the pin attached to the external LED as an output.
  pinMode(8, OUTPUT);             // initialize the "g" pin for the 7-segment display as an output.
  pinMode(7, OUTPUT);             // initialize the "a" pin for the 7-segment display as an output.
  pinMode(6, OUTPUT);             // initialize the "e" pin for the 7-segment display as an output.
  pinMode(5, OUTPUT);             // initialize the "c" pin for the 7-segment display as an output.
  pinMode(4, OUTPUT);             // initialize the "f" pin for the 7-segment display as an output.
  pinMode(3, OUTPUT);             // initialize the "d" pin for the 7-segment display as an output.
  pinMode(2, OUTPUT);             // initialize the "b" pin for the 7-segment display as an output.
  pinMode(12, INPUT_PULLUP);      // initialize the toggle switch as an input.
  digitalWrite(LED_BUILTIN, LOW); // turn the on-board LED off.
  digitalWrite(11, LOW);          // turn the off-board LED off.
  digitalWrite(8, HIGH);          // turn the 7-segment display off. 
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  Serial.begin(9600);
  Serial.println("Flip the toggle switch to the 'on' position to start blinking the LEDs...");

}

/*
 * Function name: loop()
 * Parameters: none
 * Description: the main, continuous loop of the program.
 */
void loop() {
  
  boolean state = false;          // initilaize the state of the program.
  int count = 0;                  // initialize the count for the number of state changes.
  int setting;                    // declare the input variable.
  int previousSetting = LOW;      // initialize the previous setting of the input to "off".
  int countArray[16][7] = {       // initialize an array of the 7-segment display options.
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
    {1,1,0,1,0,0,0}  // F
  };

  int initialSetting = digitalRead(12); // read from the input switch to see if the switch is already on.
  
  while(1) {
    
    setting = digitalRead(12);    // read the toggle switch input.

    if(setting == HIGH && previousSetting == LOW && initialSetting == LOW) {    // if the switch moves from the "off" to the "on" position.
      Serial.println("Switch turned on, blinking LEDs...");
      state = true;                                         // change the state of the program.
      count = StateChange(count);                           // increment and display the count value using the StateChange() function.
    }
    else if(setting == LOW && previousSetting == HIGH) {    // if the switch moves from the "on" to the "off" position.
      Serial.println("Switch turned off, LEDs stopped...");
      state = false;                                        // change the state of the program.
      count = StateChange(count);                           // increment and display the count value using the StateChange() function.
    }
    else if(setting == HIGH && initialSetting == HIGH) {    // if the switch starts in the "on" position.
      Serial.println("Switch turned on, blinking LEDs...");
      state = true;                                         // change the state of the program.
      initialSetting = LOW;                                 // turn off the initialSetting once no longer needed.
    }

    if(state == true) {                   // the "on" state of the blinking program.
      SegmentCount(countArray, count);    // set the 7-segment display using the SegmentCount() function.
      digitalWrite(LED_BUILTIN, HIGH);    // turn on the on-board LED.
      digitalWrite(11, LOW);              // turn off the off-board LED.
      delay(1000);                        // wait 1 second.
      digitalWrite(LED_BUILTIN, LOW);     // turn off the on-board LED.
      digitalWrite(11, HIGH);             // turn on the off-board LED.
      delay(1000);                        // wait 1 second.
    }

    if(state == false) {                  // the "off" state of the blinking program.
      SegmentCount(countArray, count);    // set the 7-segment display using the SegmentCount() function.
      digitalWrite(LED_BUILTIN, LOW);     // turn off the on-board LED
      digitalWrite(11, LOW);              // turn off the off-board LED
    }

    previousSetting = setting;    // set the previous setting variable to the current setting.
   
  }
   
}

/*
 * Function name: StateChange()
 * Parameters: int count (total state changes of the system)
 * Description: increments the count and prints it to the Serial monitor in hex and decimal.
 */
int StateChange(int count) {           

  count = count+1;                      // increment the count
  int hexCount = count % 16;            // find the hex count value by applying the modulo function.
  
  Serial.print("count = (decimal) ");   // output the total count in decimal and hex.  
  Serial.print(count);
  Serial.print(" (hex) ");
  
  if(hexCount <= 9) {            // if the hex count is less than 9 it will display as a number.
    Serial.println(hexCount);
  }
  else {                         // if the hex count is greater than 9 it will display as a letter.
    if(hexCount == 10) {
      Serial.println("A");
    }
    else if(hexCount == 11) {
      Serial.println("B");
    }
    else if(hexCount == 12) {
      Serial.println("C");
    }
    else if(hexCount == 13) {
      Serial.println("D");
    }
    else if(hexCount == 14) {
      Serial.println("E");
    }
    else if(hexCount == 15) {
      Serial.println("F");
    }
  }

  return count;             // return the count to the main while(1) loop.

}

/*
 * Function name: SegmentCount()
 * Parameters: int countArray[16][7] (2-D array holding the 7-segment LED signals)
 *             int count (total state changes of the system)
 * Description: assigns the "on" or "off" signal to each 7-segment LED.
 */
void SegmentCount(int countArray[16][7], int count) {
  
  int pin = 2;                            
  for(int i = 0; i < 7; i++) {                       // loop through the 7 signals in each spot in the array.
    digitalWrite(pin, countArray[count % 16][i]);    // set the 7-segment display to its correct count value.
    pin++;                                           // move to the next pin/segment in the display.
  }
  
}
