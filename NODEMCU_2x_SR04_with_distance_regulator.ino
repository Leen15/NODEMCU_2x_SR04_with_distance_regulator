const int ECHO_PIN_1 = D1; // ECHO PIN WORKING ONLY WITH THIS PINS: D1,D2,D8 
const int ECHO_PIN_2 = D2; // ECHO PIN WORKING ONLY WITH THIS PINS: D1,D2,D8 
const int TRIG_PIN_1 = D3;
const int TRIG_PIN_2 = D4;


const int ReleMaxDistance = 60; // in cm
const int RelePin = D5;
int LastDistance1 = 0;
int LastDistance2 = 0;

int EnableReleS1 = 0;  
int EnableReleS2 = 0;  

int CurrentSetDistance = 0;
 
void setup() {
  // initialize serial communication:
  Serial.begin(115200);
 
  pinMode(TRIG_PIN_1,OUTPUT);
  pinMode(ECHO_PIN_1,INPUT);
  pinMode(TRIG_PIN_2,OUTPUT);
  pinMode(ECHO_PIN_2,INPUT);

  pinMode(BUILTIN_LED, OUTPUT); 
  digitalWrite(BUILTIN_LED, LOW);
  
  pinMode(RelePin, OUTPUT); 
  digitalWrite(RelePin, HIGH);
}
 
long duration1, distanceCm1;
long duration2, distanceCm2;
long potentiometerValue = 0;

const char STRING_cm[] PROGMEM = "cm ";
const char STRING_oof[] PROGMEM = "Out of range ";
const char STRING_s1[] PROGMEM = "S1: ";
const char STRING_s2[] PROGMEM = "S1: ";
const char STRING_releState[] PROGMEM  = "releState: ";
const char STRING_currentSet[] PROGMEM  = " current_set:";
   
void loop()
{
  // read current position of potentiometer and calculate allowed distance
  potentiometerValue = analogRead(A0);
  CurrentSetDistance = ReleMaxDistance * potentiometerValue / 530;
  
 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN_1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_1, LOW);
  duration1 = pulseIn(ECHO_PIN_1,HIGH); 
  // convert the time into a distance
  distanceCm1 = duration1 / 29.1 / 2 ;

  delay(50); 
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN_2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_2, LOW);
  duration2 = pulseIn(ECHO_PIN_2,HIGH); 
  // convert the time into a distance
  distanceCm2 = duration2 / 29.1 / 2 ; 
  
  EnableReleS1 = 0;
  if (distanceCm1 <= 0){ // Out of range
    distanceCm1 = 1000;
  }
  else if (distanceCm1 <= CurrentSetDistance & LastDistance1 < 100) // Aggiungo un check per evitare che becchi misurazioni inesistenti
  {
    EnableReleS1 = 1;
  }

  EnableReleS2 = 0;
  if (distanceCm2 <= 0){ // Out of range
    distanceCm2 = 1000;
  }
  else if (distanceCm2 <= CurrentSetDistance & LastDistance2 < 100) // Aggiungo un check per evitare che becchi misurazioni inesistenti
  {
    EnableReleS2 = 1;
  }

  digitalWrite(RelePin, (EnableReleS1 & EnableReleS2) ? LOW : HIGH);

  int releState = digitalRead(RelePin);
  Serial.print(FPSTR(STRING_s1));
  if (distanceCm1 == 1000)
  {
    Serial.print(FPSTR(STRING_oof));
  }
  else
  {
    Serial.print(distanceCm1);
    Serial.print(FPSTR(STRING_cm));
  }
  
  Serial.print(FPSTR(STRING_s2));
  if (distanceCm2 == 1000)
  {
    Serial.print(FPSTR(STRING_oof));
  }
  else
  {
    Serial.print(distanceCm2);
    Serial.print(FPSTR(STRING_cm));
  }
  
  Serial.print(FPSTR(STRING_releState));
  Serial.print(!releState);

  Serial.print(FPSTR(STRING_currentSet) );
  Serial.print(potentiometerValue);
  Serial.print(" ");
  Serial.print(CurrentSetDistance);
  Serial.print(FPSTR(STRING_cm));
    
  Serial.println(); 

    
  LastDistance1 = distanceCm1;
  LastDistance2 = distanceCm2;
 // delay(50);
}
