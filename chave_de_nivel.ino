
#define TEMPO 5000
#define pin1 3 // chave de nivel do reservatorio
#define pin2 4 // chave de nivel alto do aquario
#define pin3 5 // chave nivel baixo do aquario
#define out1 6 // solenoide do reservatório
#define out2 7 // solenoide do desvio do aquario

int trocar=0;

long tempoanterior=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
pinMode (pin1, INPUT);
pinMode (pin2, INPUT);
pinMode (pin3, INPUT);
pinMode (out1, OUTPUT);
pinMode (out2, OUTPUT);

}

void loop() 
{//void loop() 
  // put your main code here, to run repeatedly:
long tempoatual=millis();
if(tempoatual-tempoanterior>TEMPO)// ciclo de verificação a cada 5 seg para substituir o delay
{//if(tempoatual-tempoanterior>TEMPO)
  tempoanterior=tempoatual;
int a = digitalRead(pin1);// chave de nivel do reservatorio, se não me engano 1 é para ela aberta e 0 para ela fechada
int b = digitalRead(pin2);// nivel alto do aquario
int c = digitalRead(pin3);// nivel baixo do aquario
if(a=1)
{//if(a=1)
if(trocar<150)
{//if(trocar=<150)
  trocar=trocar+1;
  Serial.println("encher galão reservatório");
  Serial.println("Contagem de galões ");
  Serial.print(trocar);
}//if(trocar>150)
else{ trocar=0;}
  
}//if(a=1)

if(b=1)
{//if(b=1)
digitalWrite(out1,HIGH);//abre a solenoide do reservatorio para manter nivel alto do aquario
}//if(b=1)
else{digitalWrite(out1,LOW);}// quando o aquario estiver em nivel alto fecha a entrada de agua para o aquario vinda do reservatorio
if(c=1)
{//if(c=1)
digitalWrite(out2, LOW);// quando o aquario estiver em nivel baixo a solenoide de desvio é fechada e a de entrada de agua no aquario é aberta
digitalWrite(out1,HIGH);// abre a solenoide do reservatorio para o aquario, entra agua para aumentar o nivel do aquario
}//if(c=1);

}//if(tempoatual-tempoanterior>TEMPO)
}//void loop() 
