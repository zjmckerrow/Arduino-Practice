/*
 * 
 * Created by: Zach McKerrow
 * CWID: 11702949
 * ECE 484
 * Date: 2/3/20
 * 
 * Description: Blinks 2 LEDs out-of-phase when 'g' is entered into the serial monitor and turns the LEDs off when 's' is entered.
 *              The on-board LED turns on for 2 seconds and then turns off for 1 while the off-board LED does the opposite.
 * 
 */


const int externalLED = 8;
boolean state = false;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output.
  pinMode(externalLED, OUTPUT); // initialize the pin attached to the external LED as an output.
  digitalWrite(LED_BUILTIN, LOW); // turn both LEDs off.
  digitalWrite(8, LOW);
  state = false; //initialize the state of the LEDs.
  Serial.begin(9600);
  Serial.println("Enter a command: 'g' to go, or 's' to stop.");

}

void loop() {

  if(Serial.available() > 0) {
    String command = Serial.readString(); // read the command input
    command.trim();
    Serial.println("Command entered: ");
    Serial.println(command);
    
    if(command == "g") { // if the input was 'g' then start blinking the LEDs.
      if(state == false) {
        Serial.println("Turning on LEDs...");
      }
      else {
        Serial.println("LEDs already on...");
      }
      state = true;
    }
    if(command == "s") { // if the input was 's' then stop blinking the LEDs.
      if(state == true) {
        Serial.println("Turning off LEDs...");
      }
      else {
        Serial.println("LEDs already off...");
      }
      state = false;
    }
  }

  if(state == true) { // the "on" state of the blinking program.
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(externalLED, LOW);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(externalLED, HIGH);
    delay(1000);
  }

  if(state == false) { // the "off" state of the blinking program.
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(externalLED, LOW);
  }
  
}
