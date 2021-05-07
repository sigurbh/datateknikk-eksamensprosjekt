#include <SPI.h>
#include <MFRC522.h>
#include <CircusESP32Lib.h>
#include <esp_task_wdt.h>

//kobler opp mot Circus of Things
char ssid[] = "iPhone"; 
char password[] = "12345666";
char token[] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiI1MzIxIn0.2qfAjdqlQM95BrhbLF_VKQ8iITa7oTqWy6t26Kvotz8";
char server[] = "www.circusofthings.com";
char hjemmestatus_bruker1_key[] = "25788";
char gjestestatus_bruker1_key[] = "21595";
char ringeklokke_bruker1_key[] = "18206";
char ringeklokke_stue_key[] = "4268";
char lasYtterdor_bruker1_key[] = "11585";
char gjestestatus_total_key[] = "7535";

CircusESP32Lib circusESP32(server, ssid, password);

//watchdog
#define WDT_TIMEOUT 120
int watchdogTimer = millis();

//keypad konstanter
const int S0 = 13;
const int S1 = 34;
const int S2 = 35;
const int S3 = 32;
const int S4 = 33;
const int S5 = 39;
const int S6 = 26;
const int S7 = 27;
const int S8 = 14;
const int S9 = 12;
const int SGuest = 21;
const int outButton = 25;
const int mainDoorbell = 4;

//keypad state variabler
bool S0_state = 0;
bool S1_state = 0;
bool S2_state = 0;
bool S3_state = 0;
bool S4_state = 0;
bool S5_state = 0;
bool S6_state = 0;
bool S7_state = 0;
bool S8_state = 0;
bool S9_state = 0;
bool SGuest_state = 0;
bool outButton_state = 0;

//diverse variabler
bool outButtonMemory = 0;
int guestPush = 0;                  //hører til switch case
int numOfGuests_bruker1;
int numOfGuests_total;
bool mainDoorbell_state = 0;        //felles ringeklokke   
unsigned long previousTime = 0;
unsigned long currentTime = 0;

//RFID konstanter
#define RST_PIN 1
#define SS_PIN 5
byte readCard[4];
String masterTag = "1317230137";    //TagID på kort til inngangsparti
String tagID = "";
int accessRFID = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);

//ledlys konstanter
const int redLed = 0;
const int greenLed = 2;
const int blueLed = 15;

//konfigurerer PWM for RGB
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

//buzzer-konstanter
const int buzzer = 17;
int buzzerChannel = 3;

//variabler og konstanter som tilhører awaitingRelease-funksjonen
unsigned long timeNow = 0;
int delayPeriod = 30;

//variabler som tilhører keypad modul
String keyCode = "";

//brukernavn til forskjellige brukere
String Bruker_1 = "Ola Nordmann";
String Bruker_2 = "Bruker_2";
String Bruker_3 = "Bruker_3";
String Bruker_4 = "Bruker_4";
String Bruker_5 = "Bruker_5";
String Bruker_6 = "Bruker_6";

//passord til forskjellige brukere
String password_1 = "1234";
String password_2 = "2345";
String password_3 = "3456";
String password_4 = "4567";
String password_5 = "5678";
String password_6 = "6789";

int timeStamp;                          //tidsstempel til autolock
bool sleepMemory = 0;                   //huskevariabel til deepsleep-funksjon

//delay-funksjon vha millis()
void delayMillis(int delayPeriod){      
  int timeNow;
  timeNow = millis();
  while (millis() < timeNow + delayPeriod){
    //venter oppgitt antall millisekund
  }
}

//funksjon for å styre RGB'en
void RGBColour(int i, int j, int k){
  ledcWrite(redChannel, i);
  ledcWrite(greenChannel, j);
  ledcWrite(blueChannel, k);
}

//funksjon for å avvente release
void awaitingRelease(){
  delayMillis(30);
  while(S0_state==1 or S1_state==1 or S2_state==1 or S3_state==1 or S4_state==1 or S5_state==1 or 
        S6_state==1 or S7_state==1 or S8_state==1 or S9_state==1 or SGuest_state==1 or outButton_state==1){
    S0_state = digitalRead(S0);   
    S1_state = digitalRead(S1);
    S2_state = digitalRead(S2);
    S3_state = digitalRead(S3);
    S4_state = digitalRead(S4);
    S5_state = digitalRead(S5);
    S6_state = digitalRead(S6);
    S7_state = digitalRead(S7);
    S8_state = digitalRead(S8);
    S9_state = digitalRead(S9);
    SGuest_state = digitalRead(SGuest);
    outButton_state = digitalRead(outButton);
  }
}

//buzzer-funksjon
void buzzerFunction(int x, int y){  //x bestemmer pitchen, y antall millisekund
  ledcWriteTone(buzzerChannel, x);
  ledcWrite(buzzerChannel, 255);    //maks volum
  delayMillis(y);
  ledcWrite(buzzerChannel, 0);
 }

//funksjon passord
void passwordFunction(){
  if (digitalRead(S0)==HIGH){
    S0_state = 1;
    keyCode.concat("0");
    Serial.print("0");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S1)==HIGH){
    S1_state = 1;
    keyCode.concat("1");
    Serial.print("1");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S2)==HIGH){
    S2_state = 1;
    keyCode.concat("2");
    Serial.print("2");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S3)==HIGH){
    S3_state = 1;
    keyCode.concat("3");
    Serial.print("3");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S4)==HIGH){
    S4_state = 1;
    keyCode.concat("4");
    Serial.print("4");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S5)==HIGH){
    S5_state = 1;
    keyCode.concat("5");
    Serial.print("5");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S6)==HIGH){
    S6_state = 1;
    keyCode.concat("6");
    Serial.print("6");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S7)==HIGH){
    S7_state = 1;
    keyCode.concat("7");
    Serial.print("7");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S8)==HIGH){
    S8_state = 1;
    keyCode.concat("8");
    Serial.print("8");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(S9)==HIGH){
    S9_state = 1;
    keyCode.concat("9");
    Serial.print("9");
    buzzerFunction(2000, 70);
    awaitingRelease();
  }
  if (digitalRead(SGuest)==HIGH){
    SGuest_state = 1;
    awaitingRelease();
  }
  if (digitalRead(outButton)==HIGH){
    outButton_state = 1;
    awaitingRelease();
  }
}

//funksjon guestButton
//samme som passordFunction(), men denne returnerer tastetrykket som en integer
//denne skriver ikke til seriemonitoren
//kun konfigurert for tastene 1-6
int guestButton(){
  if (digitalRead(S1)==HIGH){
    S1_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 1;
  }
  if (digitalRead(S2)==HIGH){
    S2_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 2;
  }
  if (digitalRead(S3)==HIGH){
    S3_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 3;
  }
  if (digitalRead(S4)==HIGH){
    S4_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 4;
  }
  if (digitalRead(S5)==HIGH){
    S5_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 5;
  }
  if (digitalRead(S6)==HIGH){
    S6_state = 1;
    buzzerFunction(2000, 70);
    awaitingRelease();
    return 6;
  }
}

int rfidBruker_1()  {                                               //RFID-funksjon for Bruker 1, returnerer integer fra 0-2
  if ( ! mfrc522.PICC_IsNewCardPresent()) { return false;}          //gjør klar for å lese PICCs
  if ( ! mfrc522.PICC_ReadCardSerial()) { return false;}
  tagID = "";
  for ( byte i = 0; i < 4; i++) {                                   //MIFARE PICCs har 4 byte UID
  tagID += mfrc522.uid.uidByte[i];
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();                                             //stans avlesning
  if (tagID == masterTag) {accessRFID = 1;}                  
  else {accessRFID = 2;}
 
  return accessRFID;
}

//deepsleep-funksjon
void deepSleepFunction() {
  S0_state = digitalRead(S0);
  S1_state = digitalRead(S1);
  S2_state = digitalRead(S2);
  S3_state = digitalRead(S3);
  S4_state = digitalRead(S4);
  S5_state = digitalRead(S5);
  S6_state = digitalRead(S6);
  S7_state = digitalRead(S7);
  S8_state = digitalRead(S8); 
  S9_state= digitalRead(S9);
  SGuest_state = digitalRead(SGuest); 
  outButton_state = digitalRead(outButton);
  mainDoorbell_state = digitalRead(mainDoorbell);
  
  currentTime = millis();
  if (S0_state == 0 and S1_state == 0 and S2_state == 0 and S3_state == 0 and S4_state == 0 and S5_state == 0 and S5_state == 0 and S6_state == 0 and S7_state == 0 and S8_state == 0
  and S9_state == 0 and SGuest_state ==0 and outButton_state == 0 and mainDoorbell_state == 0 and sleepMemory == 0) {
    previousTime = millis();
    sleepMemory = 1;
  }
  else if (S0_state == 1 or S1_state == 1 or S2_state == 1 or S3_state == 1 or S4_state == 1 or S5_state == 1 or S5_state == 1 or S6_state == 1 or S7_state == 1 or S8_state == 1
  or S9_state == 1 or SGuest_state == 1 or outButton_state == 1 or mainDoorbell_state == 1) {
    sleepMemory = 0;
  }
  if (currentTime - previousTime >= (120000)) {                 //2 minutter
    Serial.println("Inngangsparti går i deep-sleep");
    esp_deep_sleep_start();
  }  
}

void awaitingCheckIn(){
  //ingen UT-knapp aktiv ---> brukeren er på vei INN  
  if(rfidBruker_1() == 1){
    Serial.println("Hei " + Bruker_1 + ", " "Tast din personlige 4-sifrede kode");
    RGBColour(0,255,0);                                         //lyser grønt
    buzzerFunction(4000, 70);                                   //lyst pip i 70ms
    RGBColour(0,0,0);

    timeStamp = millis();
    while (keyCode.length() < 4){                               //avventer kode ...
      passwordFunction();
      if(millis() - timeStamp > 20000){                         //hopper ut av løkka dersom inntasting tar over 20 sekunder
        Serial.println("Tid utløpt ");
        break;
      }                 
    }
    if (keyCode==password_1){
      Serial.println("");
      Serial.println("Passord godkjent");

                                                                //DØREN LÅSES OPP - dette simuleres ved at RGB-dioden blir grønn, med lyst pip fra buzzer
      RGBColour(0,255,0);                                       //lyser grønt 500ms
      buzzerFunction(4000, 300);                                //lyst pip i 300ms
      delayMillis(500);
      RGBColour(0,0,0);

      //oppdaterer CoT om at Bruker_1 er hjemme
      circusESP32.write(hjemmestatus_bruker1_key, 1, token);    
      Serial.println("Velkommen " + Bruker_1 + ", innsjekk er registrert");
    }
    else {
      Serial.println("");
      Serial.println("Feil passord, ingen adgang");
                                                                //DØREN FORBLIR LÅST - dette simuleres ved at RGB-dioden blir rød, med mørkt pip fra buzzer
      RGBColour(255,0,0);                                       //lyser rødt 500ms
      buzzerFunction(1000, 300);                                //mørkt pip i 300ms
      delayMillis(500);
      RGBColour(0,0,0);
    }
    keyCode = "";                                               //reseter keyCode-strengen

  }
  else if(rfidBruker_1() == 2){
    Serial.println("Ukjent kort");
    RGBColour(255,0,0);                                         //lyser rødt
    buzzerFunction(300, 70);                                    //mørkt pip i 70ms
    RGBColour(0,0,0);
  }
  accessRFID = 0;                                               //setter rfid-funksjon for bruker_1 tilbake til state 0 (venter på kort)
}

void awaitingCheckOut(){
  if(digitalRead(outButton)==HIGH){                             //UT-knappen blir aktiv, dette tolkes som at brukeren er på vei UT
    outButtonMemory = 1;
    Serial.println("Hei, scan kort for å registrere utsjekk");
    RGBColour(0,255,0);                                         //lyser grønt
    buzzerFunction(4000, 70);                                   //lyst pip i 70ms
    RGBColour(0,0,0);
    outButton_state = 1;
    awaitingRelease();
  }
  timeStamp = millis();
    while(outButtonMemory == 1){                                //UT-knapp har blitt påtrykt
      if(rfidBruker_1() == 1){ 
        RGBColour(0,255,0);                                     //lyser grønt
        buzzerFunction(4000, 70);                               //lyst pip i 70ms
        RGBColour(0,0,0);
        
                                                                //DØREN LÅSES OPP
                                                                //når brukeren er på vei UT, er det bare en "bonus" at døren låses opp. 
                                                                //døren vil altså alltid kunne åpnes fra innsiden uansett om kort registreres
                                                                //her har RGB-dioden og buzzeren kun til formål å fortelle brukeren at utsjekkingen ble registrert
        
        circusESP32.write(hjemmestatus_bruker1_key, 0, token);  //oppdaterer CoT om at Bruker_1 IKKE er hjemme
        Serial.println("Ha det fint, " + Bruker_1 + ". Utsjekk er registrert");
        
        outButtonMemory = 0;                                    //reseter utKnappMemory kun dersom et kjent kort registreres
      }
      else if(rfidBruker_1() == 2){
        Serial.println("Ukjent kort");
        RGBColour(255,0,0);                                     //lyser rødt
        buzzerFunction(300, 70);                                //mørkt pip i 70ms
        RGBColour(0,0,0);
      }
      else if((millis() - timeStamp > 10000)){                  //hopper ut av løkka dersom det tar over 10 sekunder å scanne riktig kort
        outButtonMemory = 0;
        Serial.println("Tid utløpt ");    
      }
    }
  accessRFID = 0;
}

void awaitingCheckInGuest(){                                    //registrerer gjestetrykk, ringer den aktuelle brukeren og oppdaterer CoT
  if(digitalRead(SGuest)==HIGH){
    Serial.println("Hei, hvem ønsker du å besøke? ");
    Serial.println("Tast 1 for " + Bruker_1);
    Serial.println("Tast 2 for " + Bruker_2);
    Serial.println("Tast 3 for " + Bruker_3);
    Serial.println("Tast 4 for " + Bruker_4);
    Serial.println("Tast 5 for " + Bruker_5);
    Serial.println("Tast 6 for " + Bruker_6);

    RGBColour(0,255,0);                                         //lyser grønt
    buzzerFunction(4000, 70);                                   //lyst pip i 70ms
    RGBColour(0,0,0);
    SGuest_state = 1;
    awaitingRelease(); 

    timeStamp = millis();
    while(guestPush == 0){                                      //avventer tastetrykk
      guestPush = guestButton();
      if(millis() - timeStamp > 20000){                         //hopper ut av løkka dersom inntasting tar over 20 sekunder
        Serial.println("Tid utløpt ");
        break;
      }
    }
    
    switch(guestPush){                                          //kun case 1 (tilhører Bruker_1) er ferdig simulert 
      case 1:
        numOfGuests_bruker1= circusESP32.read(gjestestatus_bruker1_key, token);
        numOfGuests_total = circusESP32.read(gjestestatus_total_key, token);
        
          if(numOfGuests_bruker1> 2) {                                                //dersom antall gjester overstiger 3
            Serial.println(Bruker_1 + " kan ikke overstige 3 gjester, vennligst prøv igjen senere ");
            break;
          }
          else if(numOfGuests_total > 4) {
            Serial.println("Kollektivet kan ikke overstige 5 gjester totalt, vennsligst prøv igjen senere ");
            break;
          }
        Serial.println("Ringer " + Bruker_1 + "... ");
        circusESP32.write(ringeklokke_bruker1_key, 1, token);
        timeStamp = millis();
          while(circusESP32.read(lasYtterdor_bruker1_key, token) != 1 and (millis() - timeStamp < 20000)){
            //venter i løkka helt til ytterdøra blir låst opp av Bruker_1, eller det har gått over 20 sekunder        
          }
          if(millis() - timeStamp > 20000){
            Serial.println(Bruker_1 + " svarer ikke, vennligst prøv igjen senere");
            circusESP32.write(ringeklokke_bruker1_key, 0, token);
            break;                                                                     //hopper ut av case 1 (åpner ikke døra, skrur av ringeklokkesignal)
          }
        numOfGuests_bruker1+= 1;                                                       //legger til en gjest til Bruker_1
        numOfGuests_total += 1;                                                        //legger til en gjest til TOTAL
        circusESP32.write(gjestestatus_bruker1_key, numOfGuests_bruker1, token);       //forteller CoT at Bruker_1 har fått en gjest
        circusESP32.write(gjestestatus_total_key, numOfGuests_total, token);           //endrer antall TOTALE gjester
        
        Serial.println("VELKOMMEN INN ");
        Serial.println("Innsjekk vellykket, " + Bruker_1 + " har nå " + numOfGuests_bruker1 + " gjest(er) registrert");
        
        RGBColour(0,255,0);                                                            //DØREN LÅSES OPP
        buzzerFunction(4000, 300);                                                     //lyst pip i 300ms
                                                                                       //døren står åpen i 10 sekunder før den låses igjen
        timeStamp = millis();
          while(circusESP32.read(lasYtterdor_bruker1_key, token)==1){            
            if(millis() - timeStamp > 10000){                                          //LED lyser grønt i 10 sekunder, når den slutter å lyse er døren låst
            circusESP32.write(lasYtterdor_bruker1_key, 0, token);                      
            break;                                                                     //låser etter 10 sekunder som ekstra sikkerhet
            }
          }
        RGBColour(0,0,0);
        guestPush = 0;                                                                 //reseter guestPush
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
    }
  }
}

void awaitingDoorbell(){
  //felles ringeklokke som ringer i stua, registrerer IKKE inn-/utsjekk. 
  //simuleringen benytter seg av samme ESP som ringeklokka til Bruker_1
  
  if(digitalRead(mainDoorbell)==HIGH){
    mainDoorbell_state = 1;
    buzzerFunction(1200, 700);
    buzzerFunction(1000, 1000);                                                                     //vanlig "ding dong"
    timeStamp = millis();
    circusESP32.write(ringeklokke_stue_key, 1, token);                                              //setter ringeklokkesignalet til 1
    while(mainDoorbell_state==1){
      if((millis() - timeStamp > 10000) or (circusESP32.read(ringeklokke_stue_key, token)==0)) {    //setter til 0 etter 10 sekunder
        mainDoorbell_state = 0;
      }
    }
  }
}

void watchdogReset() {
  if (millis() - watchdogTimer > 2000) {                                                           //reseter watchdogen hvert 2 sekund 
      esp_task_wdt_reset();
      watchdogTimer = millis();
  }
}

//----------------------------------------------------------------------------------------------------------------------

void setup() {
  //ESP wakeup config
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);   //vi har valgt å simulere systemet ved å bruke kun én wakeup source - knapp 0
                                                  //alternativt kunne vi vekket ESP'en med alle knappene, dette krever flere RTC-pinner enn tilgjengelig
  Serial.begin(115200);
  Serial.println("VELKOMMEN");
  Serial.println("Scan inngangskort eller trykk på GJEST");

  esp_task_wdt_init(WDT_TIMEOUT, true);           //120 sekund uten reset vil boote ESP
  esp_task_wdt_add(NULL);
  
  pinMode(S0, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
  pinMode(S7, INPUT);
  pinMode(S8, INPUT);
  pinMode(S9, INPUT);
  pinMode(SGuest, INPUT);
  pinMode(outButton, INPUT);
  pinMode(mainDoorbell, INPUT);

//konfigurerer ledPWM funksjonaliteter
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
//kobler kanal opp mot pinne
  ledcAttachPin(redLed, redChannel);
  ledcAttachPin(greenLed, greenChannel);
  ledcAttachPin(blueLed, blueChannel); 

//PWM for buzzer
  ledcSetup(buzzerChannel, freq, resolution);
  ledcAttachPin(buzzer, buzzerChannel);

//initialiserer RFID
  SPI.begin();              //SPI bus
  mfrc522.PCD_Init();       //MFRC52

//kobler opp mot Cot
  circusESP32.begin();  
}

//----------------------------------------------------------------------------------------------------------------------

void loop() {
  watchdogReset();
  
  awaitingCheckIn();
  
  awaitingCheckOut();
    
  awaitingCheckInGuest();
  
  awaitingDoorbell();
  
  deepSleepFunction(); 
}
