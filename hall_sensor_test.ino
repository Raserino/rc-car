#define PIN 6

long starttime = 0;
double rpm = 0;
bool S1 = 0;

void setup() 
{
  pinMode(PIN, INPUT);
  Serial.begin(115200);
  starttime = millis();
}

void loop() 
{
  bool sensor = digitalRead(PIN);

  if(sensor && !S1)
  {
    S1 = 1;
  }

  if(!sensor && S1)
  {
    long T = millis() - starttime;
    starttime = millis();
    rpm = (60.0*1000)/T;
    Serial.print(rpm);
    Serial.print(" ");
    Serial.print("0");
    Serial.print(" ");
    Serial.println("5000");
    S1 = 0;
  }
}