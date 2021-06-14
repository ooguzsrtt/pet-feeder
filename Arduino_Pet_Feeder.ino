// Arduino UNO Pet Feeder Project
// This porject has 11 components
// These components are given below
// 1. 10K Potentiometer
// 2. SG90 Servo
// 3. Green LED
// 4. Yellow LED
// 5. White LED
// 6. Red LED
// 7. Red LED
// 8. 2x16 LCD Screen
// 9. 10K Potentiometer for LCD
// 10. Hook Switch 
// 11. 330R and 220R resistors

// Code and project by Oğuz Sert (https://github.com/ooguzsrtt)

#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //LCD connection pins. Look the datasheet of 2x16 LCD display for further information.

// PIN DEFINITIONS
#define POT A5
#define SERVO_PIN 10
#define G_LED 6
#define Y_LED 7
#define W_LED 8
#define R_LED 9
#define BTN_CONFIG 13

#define FREQ 3

//Timing and servo parameters
#define FEED_TIME 1 //1*1000 ms
#define FILL_TIME 5 //5*1000 ms
#define SERVO_ANGLE 100 //in degrees

// Project has a debug mode that limits the interval to debugTime parameter as given below.
bool debug = true;
int debugTime = 10; // In seconds

Servo servo;

int freq = 0;
int sensorFreq = 0;
float sensorValue = 0;

float hourMax= 5; //Maximum feed time
float iteration;


int confButtonState = 0;
int resetButtonState = 0;

bool setupContinue = true;

void setup() {
  Serial.begin(9600);
  boot();
}

void loop() {
  lcd.clear();
  if(setupContinue == true){
    set_frequency();
  }else{
    update_blink_in_sc();
    if(sensorFreq==0){
      feed(FEED_TIME);
      fill(FILL_TIME*1000);
      sensorFreq = freq+1;
    }
    sensorFreq = sensorFreq-1;
  }
}


void boot(){
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  //pinMode(13, INPUT);
  lcd.begin(16, 2);
  pinMode(Y_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(W_LED, OUTPUT);
  digitalWrite(G_LED, HIGH);
  servo_turn(0);
  lcd.home();
  boot_dot_matrix(75);
  lcd.clear();
  lcd.home();
  lcd.print("CatFeeder");
  lcd.setCursor(0,1);
  lcd.print("WELCOME");
  delay(3000);
}

void feed(int feed_time){
  digitalWrite(Y_LED, LOW);
  servo.write(SERVO_ANGLE);
  for (int i = 1; i <= feed_time; i++) {
    lcd.clear();
    lcd.print("FEEDING");
    digitalWrite(W_LED, LOW);
    digitalWrite(R_LED, HIGH);
    delay(500);
    digitalWrite(R_LED, LOW);
    digitalWrite(W_LED, HIGH);
    delay(500);
  }
}

void fill(int fill_time){
  servo_turn(0);
  led_off(Y_LED);
  led_on(W_LED);
  led_on(R_LED);
  lcd.clear();
  lcd.print("FILLING");
  delay(fill_time);
  led_off(W_LED);
  led_off(R_LED);
}

void update_blink_in_sc(){
  lcd.clear();
  lcd.home();
  lcd.print("REMAINING:");
  lcd.setCursor(0,1);
  lcd_print_time(sensorFreq);
  led_off(Y_LED);
  delay(1000);
  led_on(Y_LED);
  led_off(W_LED);
}

void set_frequency(){
  lcd.print("FEED INTERVAL:");
  lcd.setCursor(0,1);
  iteration = (hourMax*60)/30;
  sensorValue = analogRead(POT);
  if(debug == false){
    sensorValue = map(sensorValue, 0, 1023, 1, iteration*10);
    sensorValue = (ceil(sensorValue/10)*30)*60;
  }else{
    sensorValue = debugTime;
  }
  
  freq = sensorValue;
  lcd_print_time(freq);
  confButtonState = digitalRead(BTN_CONFIG);
  if(confButtonState == 1){
    sensorFreq = freq;
    setupContinue = false;
    lcd.clear();
    lcd.home();
    lcd.print("TIME SET");
    delay(2000);
  }
  delay(500);
}

void lcd_print_time(int timetable){
  int seconds = timetable%60;
  int hours = timetable/3600;
  int minutes = (timetable/60)-(hours*60);
  String secondsS = (String(seconds).length() < 2) ? "0"+String(seconds) : String(seconds);
  String hoursS = (String(hours).length() < 2) ? "0"+String(hours) : String(hours);
  String minutesS = (String(minutes).length() < 2) ? "0"+String(minutes) : String(minutes);
  lcd.print(hoursS+" S. "+minutesS+" D. "+secondsS+" S. ");
}

void boot_dot_matrix(int in_time){
  for(int row = 0; row <= 1; row++){
    for(int column = 0; column <= 15; column++){
      lcd.setCursor(column, row);
      lcd.print(".");
      delay(in_time);
    }
  }
}

void servo_turn(int angle){
  servo.write(angle);
}

void led_on(char LED_NAME){
  digitalWrite(LED_NAME, HIGH);
}

void led_off(char LED_NAME){
  digitalWrite(LED_NAME, LOW);
}

// Code and project by Oğuz Sert (https://github.com/ooguzsrtt)
