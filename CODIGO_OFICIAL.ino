#include <LiquidCrystal_I2C.h>

#include <Servo.h>

//--------DEFINIR CONSTANTES Y PINES--------

const short int TRIG_ENTRADA = 4;    // trigger en pin 4
const short int ECO_ENTRADA = 3;      // echo en pin 3

const short int TRIG_SALIDA = 7;    // trigger en pin 4
const short int ECO_SALIDA = 6;      // echo en pin 3

const short int SERVO_ENTRADA = 9;
const short int SERVO_SALIDA = 11;

//-------------------------------------------


Servo myServo_entrada;
Servo myServo_salida;

short int espacios_totales = 9;
short int espacios_ocupados = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

int distancia_medida_cm(int trig,int eco,String tipo)
{
    int d,duracion;
    digitalWrite(trig, HIGH);
    delay(1);
    digitalWrite(trig, LOW);

    duracion = pulseIn(eco, HIGH);
    d = duracion / 58.2;

    return d;
}

bool bandera_entrada = false;
bool bandera_salida = false;
int contador_entrada = 0;
int contador_salida = 0;
void calcular_distancia_prender_led_servo(int d,int servo,class Servo &servomotor,bool &bandera, int &contador)
{
    
    if (d <= 110 && d >= 0){ //VARIA      
        servomotor.write(0);
        bandera = true;
  
    }else{//NORMAL
        servomotor.write(90);
      if (bandera == true){
        bandera = false;
        contador++;
      }
      delay(2000);
    }

    
    
}

void iniciar_LCD(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.display();
}

void mostrar_texto_LCD(int espacio_disponible){
  if (espacio_disponible > 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  ");
    lcd.print(espacio_disponible);
    lcd.print(" ESPACIOS");
    lcd.setCursor (0,1);
    lcd.print("  DISPONIBLES  ");
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(">NO HAY ESPACIO<");
  }
  
}


//------------------------------------------------------------------------------------------------

void setup()
{
    pinMode(TRIG_ENTRADA, OUTPUT);  // trigger como salida
    pinMode(ECO_ENTRADA, INPUT);    // echo como entrada

    pinMode(TRIG_SALIDA, OUTPUT);  // trigger como salida
    pinMode(ECO_SALIDA, INPUT);    // echo como entrada
  
    myServo_entrada.attach(SERVO_ENTRADA);
    myServo_salida.attach(SERVO_SALIDA);
  
    iniciar_LCD();
  
    Serial.begin(9600);     // inicializacion de comunicacion serial a 9600 bps

}

void loop()
{

    delay(500);
    short int distancia_medida_entrada = distancia_medida_cm(TRIG_ENTRADA,ECO_ENTRADA,"entrada");//delay(200);
    short int distancia_medida_salida = distancia_medida_cm(TRIG_SALIDA,ECO_SALIDA,"salida");//delay(200);
    if (espacios_ocupados != 9){
      calcular_distancia_prender_led_servo(distancia_medida_entrada,SERVO_ENTRADA,myServo_entrada,bandera_entrada,contador_entrada);
    }else{
      myServo_entrada.write(0);
    }
    calcular_distancia_prender_led_servo(distancia_medida_salida,SERVO_SALIDA,myServo_salida, bandera_salida, contador_salida);

    espacios_ocupados = contador_entrada - contador_salida;
  
  
    mostrar_texto_LCD(espacios_totales - espacios_ocupados);
}
