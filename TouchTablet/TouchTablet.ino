  

int sum = 0;
unsigned char sample_count = 0;
float voltage = 0.0;
int batery = 0;
float range = 0;
int state = 0;
int state_s_r = 0;
int currentValue;
int threshold;
float temp;
int i = 0;
int o = 0;
float test = 0.0;
int shpejt;
String str = "=";
String finish = "!";
String komunikimi;
volatile unsigned long t=0;
volatile unsigned long ot=0;
unsigned int rpmjat=0;
volatile unsigned long delay_time=0;

ISR(PCINT0_vect) {
  if (PINB & 1) {
    ot=t;
    t=millis();
    delay_time = millis();
  }
} 

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600); 
  MCUCR = (1<<ISC01) | (1<<ISC00);
  PCICR |= (1<<PCIE0); 
  PCMSK0 |= (1<<PCINT0); 
  pinMode(8, INPUT); //Make pin an input 
  digitalWrite(8,HIGH); //Enable pullup resistor on Analog Pin 
  interrupts(); 
}
void loop() {
  while (!Serial);
  relays();
  if (state == 2){
    full();
    komunikimi = "0" + str + batery + str + temp + str + rpmjat*60/1000/2.2 + str + finish;
    Serial.println(komunikimi);
    Serial.flush();
  }else if (state == 1){
    full();
    komunikimi = "0" + str + batery + str + temp + str + shpejt + str + finish;
    Serial.println(komunikimi);
    Serial.flush();
  }else if (state == 0){
    not_full;
    komunikimi = "0=0=0=0.00=0!";
    Serial.println(komunikimi);
    Serial.flush();
  }
  o=i;
  delay(200);
}
  
void full(){
  tension();
  battery();
  temperature();
  mspeed();
  rpm();
}
void not_full(){
  threshold = 0;
  voltage = 0.00;
  batery = 0;
  temp = 0.00;
  range = 0;
  shpejt = 0;
  rpmjat = 0;
}

void relays() {
  if (Serial.available()) {
    state = Serial.parseInt();
    if (state == 1) {
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
    }
    if (state == 0) {
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
    }
    if (state == 2) {
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
    }
  }
}

void tension() {
  sum = analogRead(A0);
  test = (float)sum / 1024;
  voltage = test * 52.25;
  
  if(voltage ==0){
    voltage=0; 
  }else if (voltage>7 && voltage <11){
    voltage=voltage-0.20;
  }else if (voltage>11 && voltage <17){
    voltage=voltage - 0.40;
  }else if (voltage>17){
    voltage=voltage - 0.50;
  }
  
}

void battery() {
  batery = map(voltage, 0, 19, 0, 100);
}

void temperature() {
  temp = ((analogRead(A5) * 5.0 / 1024) - 0.5) * 30;
}

void rpm() {
  
  if((millis() - delay_time) < 500) {
    rpmjat=(60000.0/(float)(t-ot));
    rpmjat = rpmjat - (rpmjat%100);
  }else {
    rpmjat=0;
  }
}

void mspeed() {
  shpejt = map(rpmjat, 0, 8000, 0, 120);
}
