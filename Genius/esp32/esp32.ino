#include <ArduinoWebsockets.h>

const char *ssid = "";
const char *password = "";
const char* websockets_server = "http://192.168.0.151:3000/";

using namespace websockets;

WebsocketsClient client;

const int led1 = 27; //azul
const int led2 = 14; // verde
const int led3 = 12; // amarelo
const int leds[] = {27, 14, 12};

int speedLed = 500;
int randNumber = 0;

bool playPlayer = false;
bool playMachine = false;
bool startGame = false;

int shotMachine[100];

int contMachine = 0;

int contRoundMachine = 0;
int contRoundPlayer = 0;

bool gameOver = false;

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1, LOW);
    delay(500);
  }

  digitalWrite(led2, HIGH);

  bool connected = client.connect(websockets_server);

  digitalWrite(led2, LOW);

  client.onMessage([&](WebsocketsMessage message) {

    if (message.data() == "start") {
      startGame = true;
      gameOver = false;
      contRoundPlayer = 0;
      contRoundMachine = 0;
      contMachine = 0;
    }

    if ((message.data() == "0" || message.data() == "1" || message.data() == "2") && !playPlayer) {

      if (message.data().toInt() != shotMachine[contRoundPlayer]) {
        gameOver = true;
      }

      contRoundPlayer++;

      if (contRoundPlayer == contRoundMachine) {
        contRoundPlayer = 0;
        contRoundMachine = 0;
        playPlayer = true;
      }
    }
    
  });
}
void Machine() {
  playPlayer = false;
  randNumber = random(0, 3);
  shotMachine[contMachine] = randNumber;

  for (int i = 0; i <= contMachine; i++) {
    digitalWrite(leds[shotMachine[i]], HIGH);
    delay(speedLed);
    digitalWrite(leds[shotMachine[i]], LOW);
    delay(speedLed);
    contRoundMachine++;
  }
  
  contMachine++;
}
void loop() {

  if (client.available()) {
    client.poll();
  }
  if ((startGame || playPlayer) && gameOver == false) {
    startGame = false;
    Machine();
  }

  if (gameOver) {
    client.send("Game Over");
  }

  delay(300);
}
