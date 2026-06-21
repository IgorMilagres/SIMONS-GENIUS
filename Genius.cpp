// GENIUS COM SENSOR ULTRASSÔNICO
#include <TimerOne.h>
#include <MultiFuncShield.h>
//pinus
#define TRIG_PIN 5
#define ECHO_PIN 6
//config
#define TAM_SEQUENCIA 5
// faixas de distância
#define DIST_1 10
#define DIST_2 20
#define DIST_3 30
#define DIST_4 40
//variaveis
int sequencia[TAM_SEQUENCIA];
int fase = 1;
bool jogoIniciado = false;
//ler sensor
long readUltrasonicDistance(int triggerPin, int echoPin) {
 pinMode(triggerPin, OUTPUT);
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);
 pinMode(echoPin, INPUT);
 long tempo = pulseIn(echoPin, HIGH, 30000);
 return tempo;
}
float lerDistancia() {
 return 0.01723 * readUltrasonicDistance(TRIG_PIN, ECHO_PIN);
}
int detectarFaixa(float distancia) {
 if (distancia > 0 && distancia <= DIST_1) return 1;
 else if (distancia <= DIST_2) return 2;
 else if (distancia <= DIST_3) return 3;
 else if (distancia <= DIST_4) return 4;
 return 0;
}
//mostrar item
void mostrarItem(int valor) {
 MFS.writeLeds(LED_ALL, OFF);
 MFS.write(" ");
 switch (valor) {
 case 1:
 MFS.writeLeds(LED_1, ON);
 MFS.write(" 1 ");
 MFS.beep(1, 2, 1, 0, 0);
 break;
 case 2:
 MFS.writeLeds(LED_2, ON);
 MFS.write(" 2 ");
 MFS.beep(2, 2, 1, 0, 0);
 break;
 case 3:
 MFS.writeLeds(LED_3, ON);
 MFS.write(" 3 ");
 MFS.beep(3, 2, 1, 0, 0);
 break;
 case 4:
 MFS.writeLeds(LED_ALL, ON);
 MFS.write(" 4 ");
 MFS.beep(4, 2, 1, 0, 0);
 break;
 }
 delay(1500);
 MFS.writeLeds(LED_ALL, OFF);
 MFS.write(" ");
 delay(700);
}
//mostra sequencia
void mostrarSequencia() {
 Serial.println("Sequencia:");
 for (int i = 0; i < fase; i++) {
 mostrarItem(sequencia[i]);
 }
}
//erro
void animacaoErro() {
 Serial.println("ERRO!");
 MFS.write("Err ");
 for (int i = 0; i < 3; i++) {
 MFS.writeLeds(LED_ALL, ON);
 MFS.beep(1, 5, 1, 0, 0);
 delay(500);
 MFS.writeLeds(LED_ALL, OFF);
 delay(500);
 }
 MFS.write(" ");
}
//vitoria
void animacaoVitoria() {
 Serial.println("VOCE VENCEU!");
 MFS.write("Win!");
 for (int i = 0; i < 5; i++) {
 MFS.writeLeds(LED_ALL, ON);
 MFS.beep(1, 2, 1, 0, 0);
 delay(300);
 MFS.writeLeds(LED_ALL, OFF);
 delay(300);
 }
 MFS.write(" ");
}
//fase completa
void animacaoFaseCompleta() {
 Serial.println("FASE COMPLETA!");
 MFS.write("ok ");
 MFS.writeLeds(LED_ALL, ON);
 MFS.beep(2, 2, 1, 1, 0);
 delay(800);
 MFS.writeLeds(LED_ALL, OFF);
 MFS.write(" ");
}
void setup() {
 Timer1.initialize();
 MFS.initialize(&Timer1);
 Serial.begin(9600);
 randomSeed(analogRead(A0));
 MFS.write("G ");
 delay(500);
 MFS.write("Gn ");
 delay(500);
 MFS.write("Gni ");
 delay(500);
 MFS.write("Gniu");
 delay(500);
 MFS.write("Gnius");
 delay(800);
 MFS.write(" ");
 Serial.println("GENIUS PRONTO");
 Serial.println("Pressione o botao 1 para iniciar");
 MFS.write("btn1");
}
void loop() {
 //aguaradr apertar botao 1 para iniciar
 if (!jogoIniciado) {
 byte botao = MFS.getButton();
 if (botao) {
 byte numero = botao & B00111111;
 byte acao = botao & B11000000;
 if (numero == 1 && acao == BUTTON_PRESSED_IND) {
 delay(50);
 jogoIniciado = true;
 fase = 1;
 for (int i = 0; i < TAM_SEQUENCIA; i++) {
 sequencia[i] = random(1, 5);
 }
 Serial.println("JOGO INICIADO!");
 MFS.write("GO! ");
 delay(1000);
 }
 }
 return;
 }
 //fase no display
 Serial.print("FASE: ");
 Serial.println(fase);
 MFS.write(fase);
 delay(1200);
 //mostrar sequencia
 mostrarSequencia();
 //resposta
 for (int i = 0; i < fase; i++) {
 Serial.print("Item ");
 Serial.print(i + 1);
 Serial.print(" - Esperado: ");
 Serial.println(sequencia[i]);
 MFS.write(i + 1);
 MFS.writeLeds(LED_ALL, OFF);
 Serial.println("Posicione a mao...");
 delay(3000);
 float soma = 0;
 for (int j = 0; j < 5; j++) {
 soma += lerDistancia();
 delay(50);
 }
 float distancia = soma / 5;
 int resposta = detectarFaixa(distancia);
 Serial.print("Resposta: ");
 Serial.println(resposta);
 MFS.write(resposta > 0 ? resposta : 0);
 delay(500);
 if (resposta != sequencia[i]) {
 animacaoErro();
 jogoIniciado = false;
 Serial.println("Clique no botao 1 para reiniciar");
 MFS.write("btn1");
 return;
 }
 }
 // fase completa
 animacaoFaseCompleta();
 fase++;
 // vitoria
 if (fase > TAM_SEQUENCIA) {
 animacaoVitoria();
 jogoIniciado = false;
 Serial.println("Pressione o botao 1 para jogar novamente");
 MFS.write("btn1");
 return;
 }
 delay(2000);
}