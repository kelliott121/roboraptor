//////////////////////////////////////////////////////////////////
// Begin RoboRaptor specific variable deinitions
//////////////////////////////////////////////////////////////////


// Some but not all RR commands are defined
#define  FORWARD     0x186 
#define  BACKWARD    0x187
#define  LEFT        0x180
#define  RIGHT       0x188
#define  STOP        0x18E
#define  DEMO        0x1D0
#define  HEAD_RIGHT  0x194
#define  HEAD_LEFT   0x191
#define  TAIL_LEFT   0x192
#define  TAIL_RIGHT  0x193
#define  BITE        0x1D1

#define COMMAND_SIZE 12

int command, buff[]={
  FORWARD, BACKWARD, LEFT, RIGHT, STOP, DEMO, HEAD_RIGHT,
  HEAD_LEFT, TAIL_LEFT, TAIL_RIGHT, BITE};

int LedControl = 13;     // Show when control on 
int IROut= 3;            // Where the echoed command will be sent from
//int bitTime=516;          // Bit time (Theoretically 833 but 516)
// works for transmission and is faster
int bitTime = 833;
int freqOut = 11;


//int last;                 // Previous command from IR





//////////////////////////////////////////////////////////////////
// Begin RoboRaptor specific code
//////////////////////////////////////////////////////////////////
void startTransmission(){
  digitalWrite(IROut, HIGH);
  delayMicroseconds(bitTime*8);
}

void endTransmission(){
  digitalWrite(IROut, LOW);
  delay(250); 
}

void encodeOne(){
  digitalWrite(IROut, LOW);
  delayMicroseconds(bitTime*4);
  digitalWrite(IROut, HIGH);
  delayMicroseconds(bitTime*1);
}

void encodeZero(){
  digitalWrite(IROut, LOW);
  delayMicroseconds(bitTime*1);
  digitalWrite(IROut, HIGH);
  delayMicroseconds(bitTime*1);
}

// send the command 12 bits
void RRSendCommand(int command) {
  /*
  digitalWrite(IROut,LOW);
   delayMicroseconds(8*bitTime);
   for (int i=0;i<8;i++) {
   digitalWrite(IROut,HIGH);  
   delayMicroseconds(bitTime);
   if ((command & 128) !=0) delayMicroseconds(3*bitTime);
   digitalWrite(IROut,LOW);
   delayMicroseconds(bitTime);
   command <<= 1;
   }
   digitalWrite(IROut,HIGH);
   delay(250); // Give a 1/4 sec before next
   */

  digitalWrite(LedControl, HIGH);
  startTransmission();
  for (int i=0; i<COMMAND_SIZE; i++){
    if (command & 2048)
      encodeOne();
    else
      encodeZero();
    command <<= 1;
  }
  endTransmission();
  digitalWrite(LedControl, LOW);
}


// Set up RoboSpapien functionality
void RRSetup()                    
{
  pinMode(IROut, OUTPUT);
  pinMode(LedControl,OUTPUT);
  digitalWrite(IROut,LOW);
  pinMode(freqOut, OUTPUT);
  //digitalWrite(freqOut, HIGH);

  //CODE FOR 32.9KHZ MODULATION
  /*DDRD |= (1 << 5);
   PORTB |= (1 << 5);
   
   TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode 
   
   TCCR1A |= (1 << COM1A0); // Enable timer 1 Compare Output channel A in toggle mode 
   
   OCR1A = 0xCB; // Set CTC compare value to 39200Hz at 16MHz AVR clock 
   
   TCCR1B |= (1 << CS11); // Start timer at Fcpu/8 
   */

  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = B11000111; // 199, so timer2 counts from 0 to 199 (200 cycles at 16 MHz)

  // Make robot burp to indicate setup is complete
  //RSSendCommand(RSBurp);
}


// Loop for RoboSapien functionality
/*void RSLoop() 
 { 
 digitalWrite(LedControl,HIGH);
 // Has a new command come?
 if(viRobsapienCmd != -1)
 {
 // New command - send it to robot
 Serial.print("Sending command to RoboSapien: ");
 Serial.println(viRobsapienCmd, HEX);
 RSSendCommand(viRobsapienCmd);
 
 // Now clear command
 viRobsapienCmd = -1;
 }
 digitalWrite(LedControl,LOW);
 }*/


void setup()
{
  Serial.begin(9600);
  Serial.println("RoboRaptor Start");


  RRSetup();
}



void loop()
{ 
  /*
  //Uncomment this to make RoboSapien show random movements  
   command=random(11);
   RSSendCommand(buff[command]);
   delay(1000);
   */
  if (Serial.available()){
    delay(100);
    char inData[20]; // Allocate some space for the string
    char inChar=-1; // Where to store the character read
    byte index = 0; // Index into array; where to store the character
    //String input = String((char)Serial.read()).toLowerCase;
    //String input = "test";
    while (Serial.available() > 0) // Don't read unless
      // there you know there is data
    {
      if(index < 19) // One less than the size of the array
      {
        inChar = Serial.read(); // Read a character
        inData[index] = inChar; // Store it
        index++; // Increment where to write next
        inData[index] = '\0'; // Null terminate the string
      }
    }
    String input = String(inData);
    //exSerial.print(input);

    if (input.equals("forward") || input.equals("fwd")){
      RRSendCommand(FORWARD);
      Serial.println("Telling RoboRaptor to Walk Forward");
    }
    else if (input.equals("backward") || input.equals("bwd")){
      RRSendCommand(BACKWARD);
      Serial.println("Telling RoboRaptor to Walk Backwards");
    }
    else if (input.equals("left") || input.equals("l")){
      RRSendCommand(LEFT);
      Serial.println("Telling RoboRaptor to Turn Left");
    }
    else if (input.equals("right") || input.equals("r")){
      RRSendCommand(RIGHT);
      Serial.println("Telling RoboRaptor to Turn Right");
    }
    else if (input.equals("demo") || input.equals("d")){
      RRSendCommand(DEMO);
      Serial.println("Telling RoboRaptor to Enter Demo Mode");
    }
    else if (input.equals("head right") || input.equals("hr")){
      RRSendCommand(HEAD_RIGHT);
      Serial.println("Telling RoboRaptor to Move Head Right");
    }
    else if (input.equals("head left") || input.equals("hl")){
      RRSendCommand(HEAD_LEFT);
      Serial.println("Telling RoboRaptor to Move Head Left");
    }
    else if (input.equals("tail right") || input.equals("tr")){
      RRSendCommand(TAIL_RIGHT);
      Serial.println("Telling RoboRaptor to Move Tail Right");
    }
    else if (input.equals("tail left") || input.equals("tl")){
      RRSendCommand(TAIL_LEFT);
      Serial.println("Telling RoboRaptor to Move Tail Left");
    }
    else if (input.equals("bite") || input.equals("b")){
      RRSendCommand(BITE);
      Serial.println("Telling RoboRaptor to Bite");
    }
    else if (input.equals("?")){
      Serial.println("Command options");
      Serial.println("Forward: forward, fwd");
      Serial.println("Backward: backward, bwd");
      Serial.println("Left: left, l");
      Serial.println("Right: right, r");
      Serial.println("Demo: demo, d");
      Serial.println("Head Right: head right, hr");
      Serial.println("Head Left: head left, hl");
      Serial.println("Tail Right: tail right, tr");
      Serial.println("Tail Left: tail left, tl");
      Serial.println("Bite: bite, b");
      Serial.println("Stop: any characters");
    }
    else{
      RRSendCommand(STOP);
      Serial.println("Telling RoboRaptor to Stop");
    }

  }
  //RRSendCommand(FORWARD);
  //delay(5000);
  //RRSendCommand(STOP);
  //delay(5000);
}












