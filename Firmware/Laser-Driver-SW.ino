#define SWITCH_PIN A3
#define CURRENT_PIN A0
#define LED_PIN 13
//use a toggle switch (SPDT) between PIN 7 (A), 6 (COMMON), 5 (B) to choose between PULSE and CW mode
#define A_PIN 7
#define COMMON_PIN 6
#define B_PIN 5

boolean switchState = 1 ;
long lastTime = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  //define in- and outputs
  pinMode(CURRENT_PIN, INPUT);
  pinMode(SWITCH_PIN, OUTPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  pinMode(LED_PIN, OUTPUT);
  //toggle switch for mode selection  
  pinMode(A_PIN, INPUT_PULLUP);
  pinMode(COMMON_PIN, OUTPUT);
  digitalWrite(COMMON_PIN, LOW);
  pinMode(B_PIN, INPUT_PULLUP);
  lastTime=millis();
}

void loop() {
  // read the input on the current measurement pin:
  int sensorValue = analogReadN(CURRENT_PIN,30);
  float outputValue = convert2Ampere(sensorValue);
  //output the measured current via serial
  Serial.print(sensorValue); Serial.print(": "); Serial.println(outputValue);
  delay(200);
  //send out pulse every 3sec if switch is on pulse mode
  if(millis()-lastTime > 3000) {
    digitalWrite(SWITCH_PIN, HIGH);
    digitalWrite(13, HIGH);
    delayMicroseconds(50);
    if(digitalRead(7)==LOW) {
      digitalWrite(SWITCH_PIN, LOW);
      digitalWrite(13, LOW);
    }
    //serial output for debugging the pulses
    Serial.print("* "); Serial.println(switchState);
    switchState=!switchState;
    lastTime=millis();
  }
}

float convert2Ampere(int rawInput) {
  int temp=rawInput+43; //offset evaluated experimentally
  if(temp<0) temp=0;
  float conversionFactor = 0.0424; //evaluated experimentally
  //float conversionFactor = 0.040283203125;  // theoretical: (100A / 2 Volt)*(3.3 Volt /4096)
  return conversionFactor*rawInput;  
}

int analogReadN(int pinNumber, int iterations) {
  int result=0;
  for(int i=0; i<iterations; i++) {
    result = result + analogRead(pinNumber);
  }
  return result/iterations;
}

