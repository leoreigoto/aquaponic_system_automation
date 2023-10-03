#define TEMPO 5000
#define pin1 3 // chave de nivel do reservatorio
#define pin2 4 // chave de nivel alto do aquario
#define pin3 5 // chave nivel baixo do aquario
#define out1 6 // solenoide do reservatório
#define out2 7 // solenoide do desvio do aquario
#define out_lamp 2 //lampada do aquario
#define TEMPO2 43200000// 12 horas em millis segundos
#include <Stepper.h>
long tempo_anterior =0;
long tempo_ant2=0;
const int stepsPerRevolution = 500; 
int trocar=0;
long tempoatual=0;
int horario_atual;
int inicial_hora = 13*1000*60*60;
int inicial_minuto= 30*1000*60;
int inicial_horario;
int alimentacao_hora=19*1000*60*60;
int alimentacao_minuto=30*1000*60;
int alimentacao_horario;
int ligar_lampada_hora = 18*1000*60*60;
int ligar_lampada_minuto= 0*1000*60;
int ligar_lampada_horario;
int desligar_lampada_hora = 5*1000*60*60;
int desligar_lampada_minuto = 15*1000*60;
int desligar_lampada_horario;
int am_pm;  
int am_pm2;
int contagemdias=0;
//Inicializa a biblioteca utilizando as portas de 8 a 11 para 
//ligacao ao motor 
Stepper myStepper(stepsPerRevolution, 8,10,9,11); 
    
void setup() 
{ 
Serial.begin (9600);
pinMode (pin1, INPUT);
pinMode (pin2, INPUT);
pinMode (pin3, INPUT);
pinMode (out1, OUTPUT);
pinMode (out2, OUTPUT);
pinMode (out_lamp, OUTPUT);
 //Determina a velocidade inicial do motor 
 myStepper.setSpeed(60);
 inicial_horario= inicial_hora+inicial_minuto;
 if (inicial_horario >= TEMPO2)
  {am_pm=1;}
  else
  {am_pm=0;}

 alimentacao_horario=alimentacao_hora+alimentacao_minuto;
 if (alimentacao_horario >= TEMPO2)
  {am_pm2=1;}
  else
  {am_pm2=0;}

if (inicial_horario>alimentacao_horario)
{
  //se am_pm e am_pm2 tem o mesmo valor, pois caso am_pm e am_pm forem diferentes sua soma não dará 2x am_pm
  if (am_pm+am_pm2==am_pm+am_pm)
  {
    if (am_pm2==1)
    { alimentacao_horario=alimentacao_horario - TEMPO2;
    am_pm2=0;}
  else {alimentacao_horario=alimentacao_horario+TEMPO2;
  am_pm2=1;}
  }
  //unica combinacao de inicial>alimentacao e que AM e PM sejam diferentes é inicial ser PM e alimentacao ser AM
  //nesse caso só trocaremos se a diferenca for menor ou igual a 12h se nao, já passou do horario de alimentacao noturna)
  else
  {  if (inicial_horario-alimentacao_horario<=TEMPO2)
   { alimentacao_horario=alimentacao_horario+TEMPO2;
   am_pm2=1;}
  }
}
//inicial <= alimentacao
//nesse caso só queremos trocar caso alimentacao seja PM e sua diferenca seja maior ou igual a 12h, do contrario já passou do horario de alimentaco matinal
else
{ if (alimentacao_horario-inicial_horario>=TEMPO2)
 {alimentacao_horario=alimentacao_horario-TEMPO2;
 am_pm2=0;}
  }
   
ligar_lampada_horario=ligar_lampada_hora+ligar_lampada_minuto;
desligar_lampada_horario=desligar_lampada_horario+desligar_lampada_minuto;

} 

//a hora do motor já foi ajustada no setup em termos AM e PM para ser a da proxima alimentacao  
void loop() 
{ 
long tempo_decorrido=millis();
tempoatual=inicial_horario+tempo_decorrido;

if (tempoatual>24*1000*60*60)
{
  tempo_anterior=tempoatual;
 contagemdias+=1;
}
tempoatual=tempoatual-tempo_anterior;
if( tempoatual > alimentacao_horario && am_pm==am_pm2)
{ //Gira o motor no sentido horario a 90 graus
 for (int i = 0; i<=2; i++)
 {
 myStepper.step(-256); 
 delay(2000);
 }
  
 //Gira o motor no sentido anti-horario a 120 graus
 for (int i = 0; i<=2; i++)
 {
 myStepper.step(384); 
 delay(2000);
 }
 if (am_pm2==1)
 {
  alimentacao_horario=alimentacao_horario-TEMPO2;
  am_pm2=0;}
  else 
  { alimentacao_horario=alimentacao_horario+TEMPO2;
  am_pm2=1;}
 }
if(tempoatual-tempo_ant2>TEMPO)// ciclo de verificação a cada 5 seg para substituir o delay
{//if(tempoatual-tempo_ant2>TEMPO)
  tempo_ant2=tempoatual;
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
{
digitalWrite(out2, LOW);// quando o aquario estiver em nivel baixo a solenoide de desvio é fechada e a de entrada de agua no aquario é aberta
digitalWrite(out1,HIGH);// abre a solenoide do reservatorio para o aquario, entra agua para aumentar o nivel do aquario
} //if c=1
}//if(tempoatual-tempo_ant2>TEMPO)
if (tempoatual>=ligar_lampada_horario)
{digitalWrite(out_lamp, HIGH);}
else 
{if (tempoatual>=desligar_lampada_horario)
{digitalWrite(out_lamp, LOW);}
}
}
