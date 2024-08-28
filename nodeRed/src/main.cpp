#include <Arduino.h>

#define pot A0
#define fotoR A1
#define soundS A3

int led1 = 13;
int led2 = 12;
String lineaR;

int timbre = 2; // Cambiado al pin 2 para usar la interrupción INT0

volatile bool timbrePresionado = false; // Variable para indicar si se presionó el timbre

// Declaración de la rutina de interrupción (ISR) antes de su uso
void timbreISR();

void setup()
{
  // Inicia la comunicación serial a 9600 bps
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(timbre, INPUT_PULLUP); // Configura el pin como entrada con pull-up interno activado

  // Configura la interrupción en el pin 2 (INT0) para que se active en un flanco de bajada (LOW)
  attachInterrupt(digitalPinToInterrupt(timbre), timbreISR, FALLING);

  Serial.begin(9600);
}

void loop()
{

  // Lógica de lectura de sensores sin delay
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 1000;

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // Lee el valor del pin A0 (valor entre 0 y 1023)
    int potValue = analogRead(pot);
    int fotoRValue = analogRead(fotoR);
    int soundRValue = analogRead(soundS);

    // Imprime el valor leído en el monitor serial
    int remappedpot = map(potValue, 0, 1023, 0, 500); // lm35
    Serial.print(remappedpot);
    Serial.print(",");
    Serial.print(fotoRValue);
    Serial.print(",");
    Serial.print(soundRValue);
    Serial.print(",");
    Serial.print(timbrePresionado);
    Serial.println("");
    

    // Verifica si se presionó el timbre
    if (timbrePresionado)
    {
      // Serial.println("1");  // Imprime 'true' por serial
      timbrePresionado = 0; // Restablece la variable
    }

    // Lee el comando serial para encender/apagar el LED
    String comando = Serial.readStringUntil('\n');

    if (comando == "$1")
    {
      digitalWrite(led1, HIGH); // Enciende el LED
    }
    else if (comando == "$0")
    {
      digitalWrite(led1, LOW); // Apaga el LED
    }
    else if (comando == "#1")
    {
      digitalWrite(led2, HIGH); // Apaga el LED
    }
    else if (comando == "#0")
    {
      digitalWrite(led2, LOW); // Apaga el LED
    }
  }
}

// Definición de la rutina de interrupción (ISR)
void timbreISR()
{
  timbrePresionado = 1; // Establece la variable para indicar que se presionó el timbre
}
