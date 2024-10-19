//inclui as bibliotecas do teclado e da tela lcd
#include <Keypad.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 4, 3 , 2, A4, A5);  //conexoes da tela lcd
const byte numRows= 4; // numero de linhas	
const byte numCols= 4;// numero de colunas

String senha_ativa = "925C";  //string senha para ativar
String senha_desativa = "C529"; // string senha para desativar
String digita_senha = ""; // string senha digitada
int estado = 0;  // estado (desativado, carregando, ativado)
int minuto = 1;  //temporizador
int tempo = 59;
int tempo2 = 59;
int alarme = 0;  // estado do alarme

char keymap[4][4]=   // mapear o teclado
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[4] = {A0, A1, 11, 10}; // pinos conectados para linhas
byte colPins[4]= {9, 8, 7, 6};   //pinos conectados para colunas

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
// juntando todas variaveis para criamos a funçao mykeypad, para ser usada depois para receber uma tecla

void setup()
{
  pinMode(13, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop()
{
  char keypressed = myKeypad.getKey();  // variavel com valor da tecla apertada
  if (estado == 0){ 
    lcd.setCursor(0,0);
    lcd.print("Desativado");      
    lcd.setCursor(0,1);
    lcd.print("Senha:");
    if (keypressed != NO_KEY){  // le se alguma tecla for pressionada
      digita_senha = digita_senha + keypressed; // adiciona a tecla a string        
      lcd.setCursor(6, 1);
      lcd.print(digita_senha); // printa senha
      Serial.println(digita_senha);
      delay(100);
      }
    if (digita_senha == senha_ativa){  // compara senha de ativar com a digitada
      lcd.clear();
      estado = 1;  // vai pro estado carregando
      delay(100);
    }
  }
  while (estado == 1)  // carregando
  {
    for (tempo = 59; tempo >= 0; tempo -= 1)
    {
      if (tempo >= 10){
        lcd.setCursor(0,0);
        lcd.print("Ativando...");
        lcd.setCursor(12,0);
        lcd.print(minuto);
        lcd.setCursor(13,0);
        lcd.print(":");
        lcd.setCursor(14,0);
        lcd.print(tempo);
        delay(100);
      }
      if (tempo <= 9){{  // muda o lugar onde a variavel é printada
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ativando...");
        lcd.setCursor(12,0);
        lcd.print(minuto);
        lcd.setCursor(13,0);
        lcd.print(":");
        lcd.setCursor(14,0);
        lcd.print("0");
        lcd.setCursor(15,0);
        lcd.print(tempo);
        delay(100);
      }
      if (tempo == 0){ // diminui o minuto
        minuto = 0;
        lcd.clear();
      }
    }
    if (minuto == 0){
      for (tempo2 = 59; tempo2 >0; tempo2 -=1) {
        if (tempo2 >= 10){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Ativando...");
          lcd.setCursor(12,0);
          lcd.print(minuto);
          lcd.setCursor(13,0);
          lcd.print(":");
          lcd.setCursor(14,0);
          lcd.print(tempo2);
          delay(100);
        }
        if (tempo2 <= 9){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Ativando...");
          lcd.setCursor(12,0);
          lcd.print(minuto);
          lcd.setCursor(13,0);
          lcd.print(":");
          lcd.setCursor(14,0);
          lcd.print("0");
          lcd.setCursor(15,0);
          lcd.print(tempo2);
          delay(100);
        }
      }
        if (tempo2 ==0) {  
          estado = 2;  // vai pro estado ativo
          lcd.clear();
          digita_senha = "";
        }
      }
    }
  }
  while (estado == 2){
    minuto = 1;  // devolve o valor para caso ative o sistema de novo
    Keypad myKeypad2 = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
    char keypressed2 = myKeypad2.getKey();
    lcd.setCursor(0,0);
    lcd.print("Ativado");
    lcd.setCursor(0,1);
    lcd.print("Senha:");
    delay(100);
    if (digitalRead(13) == HIGH) {  // ler sensor de movimento
      alarme = 1;  // aciona alarme
    }
    while (alarme == 1){
      Keypad myKeypad3 = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
      char keypressed3 = myKeypad3.getKey();
      tone(12, 523, 300);
      delay(100);
      if (keypressed3 != NO_KEY){  
        digita_senha = digita_senha + keypressed3;
        lcd.setCursor(6, 1);
        lcd.print(digita_senha);
        Serial.println(digita_senha);
        delay(200);
      }
      if (digita_senha == senha_desativa){
        lcd.clear();
        estado = 0;
        alarme = 0;  //desativa alarme
        digita_senha = "";
        delay(100);
      }
    }
    if (keypressed2 != NO_KEY){
      digita_senha = digita_senha + keypressed2;
      lcd.setCursor(6, 1);
      lcd.print(digita_senha);
      Serial.println(digita_senha);
      delay(200);
    }
    if (digita_senha == senha_desativa){
      lcd.clear();
      estado = 0;  // volta para estado desativado
      digita_senha = "";
      delay(100);
    }
  }


              
              
              
  delay(10); // Delay a little bit to improve simulation performance
}
