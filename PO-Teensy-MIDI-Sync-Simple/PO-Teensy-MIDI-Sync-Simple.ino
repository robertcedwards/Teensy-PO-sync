byte tick = 0; 
byte PO_value; 
byte nano_value; 

const byte clock_byte = 248; 
const byte start_byte = 250; 
const byte continue_byte = 251; 
const byte stop_byte = 252; 

const byte LED_pin = 11;
const byte SYNC_pin = 0;
int DDRF;
int PORTF;

void setup() {
  usbMIDI.setHandleRealTimeSystem(doRealTime); 
  DDRB = 0xFF; 
  DDRF = 0xFF; 
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, HIGH);    
  pinMode(SYNC_pin, OUTPUT);
  digitalWrite(SYNC_pin, HIGH);   
}

void loop() {
  usbMIDI.read(); 
}

void doRealTime(byte value) {
  if(value == start_byte || value == continue_byte) {
    tick = 0; 
  }
  
  if(value == stop_byte) {
    PORTB = 0; 
    PORTF = 0;  
  } 
    
  
  if(value == clock_byte) {
    tick++; 
    
    if(tick == 24) {
      tick = 0; 
    }
    
   // PORTB = 255 * (1 - ((tick / 6) & 1)); 
   // PORTF = 255 * (1 - ((tick / 2) & 1));
   // digitalWriteFast(LED_pin, 1 - ((tick / 12) & 1));
    digitalWriteFast(SYNC_pin, 1 - ((tick / 6) & 1));
    
    
  }
}
