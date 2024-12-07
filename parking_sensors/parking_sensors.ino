#define NUMBER_OF_SENSORS 3

int trig[] = {2, 4, 6};
int echo[] = {3, 5, 7};

void setup() {
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    pinMode(trig[i], OUTPUT);
    pinMode(echo[i], INPUT);
  }
  
  Serial.begin(9600);  // zapocinjem serijsku komunikaciju
  // Serial.println("Mjerenje udaljenost s HC-SR04 senzorom");
  // Serial.println("");
  
}

int input = 0;
void loop()
{
  
  long duration[NUMBER_OF_SENSORS];
  long distance[NUMBER_OF_SENSORS];

  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    digitalWrite(trig[i], HIGH);   // Slanje triga
    delayMicroseconds(10);      // Odaslani trig traje 10 μs = 0.1 ms
    digitalWrite(trig[i], LOW);

    duration[i] = pulseIn(echo[i], HIGH);
    distance[i] = (duration[i]/2) / 28;

    // Serial.print("Distance ");
    // Serial.print(i);
    // Serial.print(": ");
    // Serial.println(distance[i]);  
    // Serial.println(" cm");
  
  }
  
  if (Serial.available() > 0) {
    
    input = Serial.read();
    
    if ( input == '0')
    {
      Serial.println(distance[0]);
    } else if (input == '1')
    {
      Serial.println(distance[1]);
    } else if (input == '2')
    {
      Serial.println(distance[2]);
    }
  }

  
  delay(10);  // preporucena pauza izmedu ocitanja ne bi trebala biti manja od 50ms
}
