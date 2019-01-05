#include <Keypad.h>
#include <Password.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

Password senha_config = Password( "123456" );
Password senha_desativa = Password( "654321" );

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
 
//Define the keymap
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
 
//// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {6,7,8,9}; //connect to row pinouts
 
// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[COLS] = {2,3,4,5}; //connect to column pinouts
 
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);


//VARIAVEIS DO SISTEMA:

char digito_capturado = "";
char tempo[6];
char senha_capturada[6];
int modo_opera = 0;
int fio_desarme = 0;
int bomba_armada = 0;
int estado_fio_1 = 0;
int estado_fio_2 = 0;
int senha_ok = 0;

void setup(){
   Serial.begin(9600);
   lcd.begin(16, 2);
   pinMode(A1, INPUT);
   pinMode(A2, INPUT);

   //SPLASH
   /*
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("    ORION*3D");
   lcd.setCursor(0,1);
   lcd.print("   TECNOLOGIA");
   delay(2000);
   lcd.clear();
   delay(500);
   lcd.setCursor(0,0);
   lcd.print("CARREGANDO...");
   delay(1000);
   lcd.setCursor(0,1);
   for (int i = 0; i < 8; i++){
    lcd.print((char)255); //CARACTER BARRA
    delay(200);
   }
   for (int i = 0; i < 8; i++){
    lcd.print((char)255); //CARACTER BARRA
    delay(75);
   }
   lcd.setCursor(0,0);
   lcd.print("CARREGADO.       ");
   delay(1500);
   */
   //FIM DO SPLASH
   
}
 
void loop(){

menu_principal();
delay(150);

}

void captura_digito(){
  int contador = 0;
  while (contador < 1) {
    char tecla = keypad.getKey();
    if (tecla != NO_KEY){
      delay(60);
      digito_capturado = tecla;
      contador++;       
    }
  }
}

void menu_principal(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PRONTA P/ ATIVAR");
  lcd.setCursor(0,1);
  lcd.print((char) 255);
  captura_digito();
    //lcd.print (digito_capturado);
    if (digito_capturado == 'A'){
      ativa();    
    }
    if (digito_capturado == 'C'){
      configura();
    }
  
}

void ativa(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEMPO P/ DETONAR");
  lcd.setCursor(0,1);
  lcd.print("  :  :");
  lcd.setCursor(0,1);
  int numero_digitos = 6;
  int contador = 0;
  while (contador < numero_digitos){
    captura_digito();
    if (digito_capturado == '*' || digito_capturado == '#' || digito_capturado == 'A' || digito_capturado == 'B' || digito_capturado == 'C' || digito_capturado == 'D'){
      //NÃO FAÇA NADA.
    }
    else{
    tempo[contador] = digito_capturado;
    contador++;
    lcd.print(digito_capturado);
      if (contador == 2 || contador ==4){
        lcd.print(":");
      }
    }
    
  }

  
  bomba_armada = 1;

  senha_ok = 0;
  
  while (bomba_armada == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("BOMBA ATIVADA!");
      lcd.setCursor(0,1);
      lcd.print("D - DESATIVAR");
      delay(100);
    estado_fio_1 = digitalRead(A1);
    estado_fio_2 = digitalRead(A2);
    if(estado_fio_1 == 1 && estado_fio_2 == 1){
      //NAO FACA NADA 
    }else{
      if(fio_desarme == 1 && estado_fio_2 == 0){
        //DETONADO
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DETONADO");
        delay(1000);
        bomba_armada = 0;  
      }else if(fio_desarme == 2 && estado_fio_1 == 0) {
        //DETONADO
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DETONADO");
        delay(1000);
        bomba_armada = 0;        
      }else{
        //DESATIVADO!
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DESARMADO!!!");
        delay(1000);
        bomba_armada = 0;
      }
    }
    
    char key = keypad.getKey();
    if (key != NO_KEY){
      delay(60);
      if (key == 'D'){
        if (modo_opera == 1){
          
          pede_senha_desat();          
        }
        if (modo_opera == 2){
          pede_fio_desat();
        }
        if (modo_opera == 3){
          pede_senha_desat();
          pede_fio_desat(); //PAREI AQUI
        }
        
      }else{
        //NAO FACA NADA.
      }
    }
    
  }

//FIM DA void ativa();
}

void configura(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SENHA CONFIG:");
  lcd.setCursor(0,1);
  int numero_digitos = 6;
  int contador = 0;
  while (contador < numero_digitos){
    captura_digito();
    if (digito_capturado == '*' || digito_capturado == '#' || digito_capturado == 'A' || digito_capturado == 'B' || digito_capturado == 'C' || digito_capturado == 'D'){
      //NÃO FAÇA NADA.
    }
    else{
      senha_capturada[contador] = digito_capturado;
      lcd.print(digito_capturado);
      contador++;
    }
  }
  delay(1000); //DELAY DEPOIS DA SENHA

  for (int i=0; i<6; i++){   //traz para senha_config a senha_capturada em 6 passos.
      senha_config.append(senha_capturada[i]);
  }

  if (senha_config.evaluate()){
    //FAZER AQUI A VALIDAÇÃO DA PASSWORD.
    //IF SENHA OK CONTINUA
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MODO DESARME:");
    lcd.setCursor(0,1);
    lcd.print("1 - SENHA");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("2 - CORTA FIO");
    lcd.setCursor(0,1);
    lcd.print("3 - SENHA+CT FIO");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE O MODO");
    lcd.setCursor(0,1);
    lcd.print("DESEJADO:");
    numero_digitos = 1;
    contador = 0;
    while (contador < numero_digitos){
      captura_digito();
      if (digito_capturado == '1'){
        lcd.print(digito_capturado);
        delay(1000);
        modo_opera = 1;
        pede_senha();
        contador++;       
      } else if (digito_capturado == '2'){
        lcd.print(digito_capturado);
        delay(1000);
        modo_opera = 2;
        pede_fio();
        contador++;
          } else if (digito_capturado == '3'){
            lcd.print(digito_capturado);
            delay(1000);
            modo_opera = 3;
            pede_senha();
            pede_fio();
            contador++;      
          }
      
      else{
        //NAO FAÇA NADA
      }
    senha_config.reset(); //RESETA A SENHA PARA PROXIMO LOOP
    
    lcd.clear();
    lcd.print("CONFIG. OK");
    delay(1500);
    
    }
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SENHA INCORRETA!");
    delay(1000);
    senha_config.reset(); //RESETA A SENHA PARA PROXIMO LOOP
    
  }
 
 //FIM DA VOID CONFIGURA 
}

void pede_senha(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE A SENHA");
    lcd.setCursor(0,1);
    lcd.print("P/ DESAT:");
    int numero_digitos = 6;
    int contador = 0;
    while (contador < numero_digitos){
      captura_digito();
      if (digito_capturado == '*' || digito_capturado == '#' || digito_capturado == 'A' || digito_capturado == 'B' || digito_capturado == 'C' || digito_capturado == 'D'){
        //NÃO FAÇA NADA.
      }
      else{
        senha_capturada[contador] = digito_capturado;
        lcd.print(digito_capturado);
        contador++;
      }  
    }
    delay(1000); //DELAY PARA DEPOIS DA SENHA
}
void pede_fio(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE O FIO");
    lcd.setCursor(0,1);
    lcd.print("P/ DESAT:");
    int numero_digitos = 1;
    int contador = 0;
    while (contador < numero_digitos){
        captura_digito();
        if (digito_capturado == '1'){
          fio_desarme = 1;
          lcd.print(digito_capturado);
          contador++;
          delay(1000);
        }
        else if (digito_capturado == '2'){
          fio_desarme = 2;
          lcd.print(digito_capturado);
          contador++;
          delay(1000);  
        }
        else{
          //NAO FAÇA NADA
        }
    }
}

void pede_senha_desat(){
 if(senha_ok == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE A SENHA");
    lcd.setCursor(0,1);
    lcd.print("P/ DESAT:");
    int numero_digitos = 6;
    int contador = 0;
    while (contador < numero_digitos){
      captura_digito();
      if (digito_capturado == '*' || digito_capturado == '#' || digito_capturado == 'A' || digito_capturado == 'B' || digito_capturado == 'C' || digito_capturado == 'D'){
        //NÃO FAÇA NADA.
      }
      else{
        senha_capturada[contador] = digito_capturado;
        lcd.print(digito_capturado);
        contador++;
      }  
    }
    delay(1000); //DELAY PARA DEPOIS DA SENHA
    
    for (int i=0; i<6; i++){   //traz para senha_config a senha_capturada em 6 passos.
      senha_desativa.append(senha_capturada[i]);
    }

      if (senha_desativa.evaluate()){
        
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("SENHA CORRETA!");
          senha_ok = 1;
          senha_desativa.reset();
          delay(1000);
          if (modo_opera == 1){
            bomba_armada = 0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("DESARMADO!!!");
            delay(1000);
          }
      
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SENHA");
        lcd.setCursor(0,1);
        lcd.print("INCORRETA!");
        senha_desativa.reset();
        delay(1000);      
      }
    

 }  
    
}

void pede_fio_desat(){
  if(senha_ok == 1 || modo_opera == 2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CORTE O FIO:");
    delay(1000);
  }
}
