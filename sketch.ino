

#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define DHTPIN 4     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
float Set=22.00;
uint8_t buton1 = 7;
uint8_t buton2 = 2;
uint8_t pwm= 6;

void setup() {
  // Initialize device.  
 pinMode(buton1,INPUT_PULLUP);//Green button increases target temperature by 0.5C
 pinMode(buton2,INPUT_PULLUP);//Red button decreases target temperature by 0.5C
 pinMode(pwm,OUTPUT);

  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 10000;

  lcd.init();                      // initialize the lcd


}

void loop() {
 // Delay between measurements.
 delay(delayMS);

// Limit the set value between 16 and 32
if(Set<=32 && Set>=16){ 
   if(digitalRead(buton1)==LOW)
  {
  if(Set!=32)
  Set+=0.5;
  }
  else if(digitalRead(buton2)==LOW)
  {
  if(Set!=16) 
  Set-=0.5;
  }
}

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(event.temperature);
  lcd.print("C");
   
   //Get pwm depending on difference between current and target temperature.
   //As the difference between current and target temperature increases, duty cycle of the pwm increases.
   //Assume the difference between current and target temperature is max 16C and min 0C.
  analogWrite(pwm, abs(event.temperature-Set)*255/16);
  delay(5);

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  lcd.setCursor(0,1);
  lcd.print(event.relative_humidity);
  lcd.print("%");

  //Print target value.
  lcd.setCursor(9,0);
  lcd.print("TARGET");
  lcd.setCursor(9,1);
  lcd.print(Set);
  lcd.print("C");

}
