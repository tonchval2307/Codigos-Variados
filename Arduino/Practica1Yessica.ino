#include<Keypad.h>
#include <LiquidCrystal.h>
/*
 *  Practica 1. Control de Iluminacion de Una Calle de 110V.
 *  1. Utilizar una Fotocelda para la lectura de iluminacion exterior y otra
 *  para detectar la iluminacion del foco.
 *  2. con el Teclado figamos los valores Maximos y Minimos para encender la lampara.
 *  3. encender una lampara en el limite inferior y apagarla en el superior.
 *  4. Otra Fotocelda indicara el Estado de Apagao y encendido de la lampara.
 * 
 */
const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'#','0','*','D'}
};
byte rowPins[rows] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[cols] = {22, 24, 26, 28}; //connect to the column pinouts of the keypad

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad Teclado = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

int ValorAmbiente = 0;
int StatusLampara = 0;
int Maximo = 0;
int Minimo = 0;

int pinLampara = 7;
char StatusLamp = '0';
enum Comportamiento{Configuracion, Runing};
enum Comportamiento Estado;

int DataStorage[4];
int MinNumber[2];
int MaxNumber[2];
int counter = 0;

char key;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print(millis());
  pinMode(9,OUTPUT);
  pinMode(pinLampara,OUTPUT);
  Estado = Configuracion;
  lcd.setCursor(0,0);
  lcd.print("P-1 : Cntrl Lamp");
  lcd.setCursor(0,1);
  for(int i=0;i<16;i++)
  {
    lcd.print(".");
    delay(250);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingresr Valores:");
  delay(2000);
  lcd.clear();
}

void loop() {

    switch(Estado)
    {
      case Configuracion:
        lcd.setCursor(0,0);
        lcd.print("Ingrsr Valor Min");
        while(counter < 2)
        {
          key = Teclado.getKey();
          if(key != NO_KEY)
          {
            if(key != 'A')
            {
              MinNumber[counter] = key - 48;
              lcd.setCursor(counter + 2,1);
              lcd.print(MinNumber[counter]);
            }
            else
            {
              counter++;
            }
          }
        }
        counter = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ingrsr Valor Max");
        while(counter < 2)
        {
          key = Teclado.getKey();
          if(key != NO_KEY)
          {
            if(key != 'A')
            {
              MaxNumber[counter] = key - 48;
              lcd.setCursor(counter + 2,1);
              lcd.print(MaxNumber[counter]);
            }
            else
            {
              counter++;
            }
          }
        }
        lcd.clear();
        delay(1000);
        lcd.setCursor(0,0);
        lcd.print("- Max ---- Min -");
        lcd.setCursor(2,1);
        Maximo = (MaxNumber[0] * 10) + MaxNumber[1];
        Minimo = (MinNumber[0] * 10) + MinNumber[1];
        lcd.print(Maximo);
        lcd.setCursor(11,1);
        lcd.print(Minimo);
        delay(3000);
        lcd.clear();
        Estado = Runing;
      break;
      case Runing:
        ValorAmbiente = analogRead(0);
        ValorAmbiente = map(ValorAmbiente,0,1023,0,100);
        StatusLampara = analogRead(1);
        StatusLampara = map(StatusLampara,0,1023,100,0);
        delay(250);
        if(ValorAmbiente > Maximo)
        {
          digitalWrite(pinLampara,HIGH);
        }
        else if(ValorAmbiente < Minimo)
        {
          digitalWrite(pinLampara,LOW);
        }
        else
        {
          digitalWrite(pinLampara,HIGH);
        }
        if(StatusLampara > Maximo)
        {
          StatusLamp = 'I';
        }
        else if(StatusLampara < Minimo)
        {
          StatusLamp = 'O';
        }
        else
        {
          StatusLamp = 'I';
        }
        if((StatusLamp == 'O') && (digitalRead(pinLampara) == HIGH))
        {
          digitalWrite(9,HIGH);
        }
        lcd.setCursor(0,0);
        lcd.print("M %:");
        lcd.setCursor(3,0);
        lcd.print(Maximo);
        lcd.setCursor(9,0);
        lcd.print ("m %:");
        lcd.print(Minimo);
        lcd.setCursor(0,1);
        lcd.print("Lamp=");
        lcd.print(StatusLamp);
        lcd.setCursor(7,1);
        lcd.print("Amb %: ");
        lcd.print(StatusLampara);
        key = Teclado.getKey();
        if((key != NO_KEY) && (key == 'A'))
        {
          Estado = Configuracion;
          digitalWrite(9,LOW);
          counter = 0;
          lcd.clear();
        }
      break;
    }

}

/*
 *   // put your main code here, to run repeatedly:
    lcd.setCursor(0, 1);
  key = Teclado.getKey();
  if(key != NO_KEY)
  {
    Serial.println(key);
    lcd.print(key);
  }
 */
