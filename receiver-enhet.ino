//RINGEKLOKKE BRUKER_1

#include <pitches.h>
#include <CircusESP32Lib.h>

//diverse keys til Circus of Things
char ssid[] = "iPhone";
char password[] = "12345666";
char token[] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiI1MzIxIn0.2qfAjdqlQM95BrhbLF_VKQ8iITa7oTqWy6t26Kvotz8";
char server[] = "www.circusofthings.com";
char gjestestatus_bruker1_key[] = "21595";
char gjestestatus_total_key[] = "7535";
char ringeklokke_bruker1_key[] = "18206";
char ringeklokke_stue_key[] = "4268";
char lasYtterdor_bruker1_key[] = "11585";
char ringetone_key[] = "10344";

CircusESP32Lib circusESP32(server, ssid, password);

//konstanter
const int potPin = 33;
const int buzzer = 14;
const int button = 32;

//variabler potensiometer
int potValue;
int volume = 0;

//variabler switch case
bool breakRinging = 0;
bool accessDenied = 0;

//PWM-egenskaper buzzer
const int freq = 5000;
const int buzzerChannel = 0;
const int resolution = 8;

int potFunction() {                             //potensiometer funksjon, leser potPin og konverterer fra 12 bit til 8 bit vha map()  
  potValue = analogRead(potPin);
  volume = map(potValue, 0, 4095, 0, 255);
  return volume;
}

void delayTone(int delayPeriod){                //funksjon som avbryter ringetone ved knappetrykk  
  int timeNow;
  timeNow = millis();
  while (millis() < timeNow + delayPeriod){     //venter x millisekund
    if(digitalRead(button) == HIGH){            //sjekker om button er høy
      break;
    }
  }
  if(digitalRead(button) == HIGH){
    breakRinging = 1;
  }
}

void delayMillis(int delayPeriod){              //samme egenskaper som den øvrige, men uten break
  int timeNow;
  timeNow = millis();
  while (millis() < timeNow + delayPeriod){
    //venter x millisekund
  }
}

void tune(int note, int duration) {             //lager en funksjon av frekvens og lengde
  ledcWriteTone(buzzerChannel, note);
  ledcWrite(buzzerChannel, potFunction());      //volumet styres av potensiometeret  
  delayTone(duration);
  ledcWrite(buzzerChannel, 0);
}

void buzz(int note, int duration) {             //lik tone() bortsett fra delayMillis (avbrytes ikke ved knappetrykk)
  ledcWriteTone(buzzerChannel, note);
  ledcWrite(buzzerChannel, potFunction());      //volumet styres av potensiometeret  
  delayMillis(duration);
  ledcWrite(buzzerChannel, 0);
}

void superMarioTheme() { //Super Mario Bros melodi i E-dur
  while(breakRinging == 0 and accessDenied == 0){
    for(int i=0; i<3; i++){
      tune(NOTE_E5, 150);
      delayTone(37.5);
      tune(NOTE_E5, 187.5);
      delayTone(187.5);
      tune(NOTE_E5, 187.5);
      delayTone(187.5);
      tune(NOTE_C5, 187.5);
      tune(NOTE_E5, 187.5);
      delayTone(187.5);
      tune(NOTE_G5, 562.5);
      delayTone(187.5);
      tune(NOTE_G4, 562.5);
      delayTone(187.5);
    }
  accessDenied = 1;
  }
}

void starWarsTheme() { //"The Imperial March" (Darth Vader melodi) spilles av i G-moll
  while(breakRinging == 0 and accessDenied == 0){
    for(int i=0; i<3; i++){
      //1. og 2. takt
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_DS5, 400);
      delayTone(50);
      tune(NOTE_AS5, 100);
      delayTone(50);
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_DS5, 400);
      delayTone(50);
      tune(NOTE_AS5, 100);
      delayTone(50);
      tune(NOTE_G5, 800);
      delayTone(400);
      //3. og 4. takt
      tune(NOTE_D6, 500);
      delayTone(100);
      tune(NOTE_D6, 500);
      delayTone(100);
      tune(NOTE_D6, 500);
      delayTone(100);
      tune(NOTE_DS6, 400);
      delayTone(50);
      tune(NOTE_AS5, 100);
      delayTone(50);
      tune(NOTE_FS5, 500);
      delayTone(100);
      tune(NOTE_DS5, 400);
      delayTone(50);
      tune(NOTE_AS5, 100);
      delayTone(50);
      tune(NOTE_G5, 800);
      delayTone(400);
    }
  accessDenied = 1;
  }
}

void slimShady(){ //"The Real Slim Shady" av Eminem i C-moll
  while(breakRinging == 0 and accessDenied == 0){
    for(int i=0; i<3; i++){
      tune(NOTE_C5, 150);
      delayTone(150);
      tune(NOTE_DS5, 150);
      delayTone(150);
      tune(NOTE_G5, 150);
      delayTone(150);
      tune(NOTE_GS5, 150);
      delayTone(150);
      tune(NOTE_C6, 300);
      delayTone(600);
      tune(NOTE_GS5, 150);
      delayTone(150);
      tune(NOTE_G5, 300);
      delayTone(600);
      tune(NOTE_GS5, 150);
      delayTone(150);
    
      //triol
      tune(NOTE_G5, 100);
      tune(NOTE_GS5, 100);
      tune(NOTE_G5, 100);
      
      tune(NOTE_FS5, 150);
      delayTone(150);
      tune(NOTE_G5, 150);
      delayTone(150);
      tune(NOTE_B4, 150);
      delayTone(150);
    }
  accessDenied = 1;
  }
}

void jaViElsker() { //Spiller nasjonalsangen i G-dur
  while(breakRinging == 0 and accessDenied == 0){
    for(int i=0; i<3; i++){
      tune(NOTE_G5, 800);
      delayTone(100);
      tune(NOTE_F5, 250);
      delayTone(50);
      tune(NOTE_E5, 500);
      delayTone(100);
      tune(NOTE_D5, 500);
      delayTone(100);
      tune(NOTE_C5, 500);
      delayTone(100);
      tune(NOTE_D5, 500);
      delayTone(100);
      tune(NOTE_E5, 500);
      delayTone(100);
      tune(NOTE_F5, 500);
      delayTone(100);
      tune(NOTE_G5, 800);
      delayTone(100);
      tune(NOTE_A5, 250);
      delayTone(50);
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_F5, 500);
      delayTone(100);
      tune(NOTE_E5, 1200);
      delayTone(1200);
      //
      tune(NOTE_A5, 800);
      delayTone(100);
      tune(NOTE_G5, 250);
      delayTone(50);
      tune(NOTE_F5, 500);
      delayTone(100);
      tune(NOTE_E5, 500);
      delayTone(100);
      tune(NOTE_D5, 500);
      delayTone(100);
      tune(NOTE_E5, 500);
      delayTone(100);
      tune(NOTE_F5, 500);
      delayTone(100);
      tune(NOTE_G5, 500);
      delayTone(100);
      tune(NOTE_G5, 800);
      delayTone(100);
      tune(NOTE_A5, 250);
      delayTone(50);
      tune(NOTE_A5, 500);
      delayTone(100);
      tune(NOTE_B5, 500);
      delayTone(100);
      tune(NOTE_C6, 1200);
      delayTone(1200);
    }
  accessDenied = 1;
  }
}

void bjelleKlang(){ //Bjelleklang i E-dur
  while(breakRinging == 0 and accessDenied == 0){
    for(int i=0; i<3; i++){
      tune(NOTE_E5, 300);
      delayTone(100);
      tune(NOTE_E5, 300);
      delayTone(100);
      tune(NOTE_E5, 600);
      delayTone(200);
      tune(NOTE_E5, 300);
      delayTone(100);
      tune(NOTE_E5, 300);
      delayTone(100);
      tune(NOTE_E5, 600);
      delayTone(200);
      tune(NOTE_E5, 300);
      delayTone(100);
      tune(NOTE_G5, 300);
      delayTone(100);
      tune(NOTE_C5, 500);
      delayTone(100);
      tune(NOTE_D5, 150);
      delayTone(50);
      tune(NOTE_E5, 800);
      delayTone(800);
    }
  accessDenied = 1;
  }
}

void hentRingeTone(){ //bruker Switch Case for å bestemme hvilken ringetone som skal hentes (ringer 3 ganger, med mindre avbrudd)
  int ringeTone = circusESP32.read(ringetone_key, token);
  switch(ringeTone) {
    case 1:
      superMarioTheme();
      break;
    case 2:
      starWarsTheme();
      break;
    case 3:
      slimShady();
      break;
      
    //17. mai
    case 4:            
     jaViElsker();
     break;

    //25. desember
    case 5:            
     bjelleKlang();
     break;
  }
}

void awaitingRinging(){
  if(circusESP32.read(ringeklokke_bruker1_key, token)==1){     //leser ringeklokkesignalet fra CoT, spiller ringetone når signalet blir 1
    Serial.println("Ringer...");
    hentRingeTone();                                           //ringer tre ganger, med mindre avbrudd
    Serial.println("Ringing ferdig");
    if(breakRinging == 1){                                     //hvis avbrudd
      Serial.println("Ytterdøren Åpnes");
      buzz(NOTE_B3, 750);                                      //buzz
    }
    else if(accessDenied == 1){
      Serial.println("Ingen svar, ytterdøren forblir låst");
      accessDenied = 0;                                        //reseter accessDenied
    }
  circusESP32.write(ringeklokke_bruker1_key, 0, token);        //setter signalet tilbake til 0
  }
}

void awaitingAccessGranted(){                                  //ved å trykke på "button" låses ytterdøren opp
                                                               //simuleres ved at RGB-dioden ved inngangspartiet blir GRØNN med buzzer
  if(digitalRead(button)==HIGH or breakRinging == 1){          //Bruker_1 trykker på knappen for å åpne ytterdøra
    circusESP32.write(lasYtterdor_bruker1_key, 1, token);      //døren ÅPNES
    Serial.println("Ytterdøren er åpen");
    delayMillis(10000);                                        //døren holdes åpen i 10 sekunder før den låses igjen
    circusESP32.write(lasYtterdor_bruker1_key, 0, token);      //døren låses (signalet settes til 0) også fra inngangspartiet som en ekstra sikkerhet
    Serial.println("Ytterdøren er låst");
    }
    breakRinging = 0;                                          //resetter breakRinging
    delayMillis(50);
}

void awaitingMainDoorbell(){
  if(circusESP32.read(ringeklokke_stue_key, token)==1){
    Serial.println("Ringer...");
    buzz(1200, 700);
    buzz(1000, 1000);                                          //vanlig "ding dong"
    Serial.println("Ringing ferdig");
    circusESP32.write(ringeklokke_stue_key, 0, token);         //setter signalet tilbake til 0 
  }
}

//----------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT);
  
  //PWM-setup buzzer
  ledcSetup(buzzerChannel, freq, resolution);
  ledcAttachPin(buzzer, buzzerChannel);

  //etablerer kontakt med CoT
  circusESP32.begin();
}

//----------------------------------------------------------------------------------------------

void loop() {
  awaitingRinging();

  awaitingAccessGranted();

  awaitingMainDoorbell();
}
