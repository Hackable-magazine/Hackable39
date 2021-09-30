#include <WiFi.h>
#include <ArduinoOTA.h>
#include <fabgl.h>
#include <Pangodream_18650_CL.h>

/*
 * Conf:
 * 4M - minial SPIFFS - QIO - PSRAM enable
 */

// mot de passe pour l'OTA
const char* otapass = "424242";
const char* myhostname = "vgaterm3";

// hôte distant (client telnet)
WiFiClient client;
// serveur telnet
WiFiServer server(23);

fabgl::VGATextController DisplayController;
//fabgl::VGA16Controller DisplayController;
//fabgl::VGA8Controller DisplayController;
fabgl::Terminal          Terminal;

Pangodream_18650_CL BL(35, 1.75, 20);

void confOTA() {
  // Port 8266 (défaut)
  ArduinoOTA.setPort(8266);

  // Hostname défaut : esp8266-[ChipID]
  ArduinoOTA.setHostname(myhostname);

  // mot de passe pour OTA
  ArduinoOTA.setPassword(otapass);

  // lancé au début de la MaJ
  ArduinoOTA.onStart([]() {
    Terminal.printf("\e[93m\e[41m /!\\ MaJ OTA /!\\ \r\n");
  });

  // lancé en fin MaJ
  ArduinoOTA.onEnd([]() {
    Terminal.printf("\n\n/!\\ MaJ terminee ");
  });

  // lancé lors de la progression de la MaJ
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Terminal.printf("\e[40mProgression: %u%%\r", (progress / (total / 100)));
  });

  // En cas d'erreur
  ArduinoOTA.onError([](ota_error_t error) {
    Terminal.printf("Error[%u]: \r\n", error);
    if (error == OTA_AUTH_ERROR) Terminal.printf("OTA_AUTH_ERROR\r\n");
    else if (error == OTA_BEGIN_ERROR) Terminal.printf("OTA_BEGIN_ERROR\r\n");
    else if (error == OTA_CONNECT_ERROR) Terminal.printf("OTA_CONNECT_ERROR\r\n");
    else if (error == OTA_RECEIVE_ERROR) Terminal.printf("OTA_RECEIVE_ERROR\r\n");
    else if (error == OTA_END_ERROR) Terminal.printf("OTA_END_ERROR\r\n");
    else Terminal.printf("Erreur inconnue\r\n");
  });

  // Activation fonctionnalité OTA
  ArduinoOTA.begin();
}

void print16colors(int space)
{
  int i;
  Terminal.write("\e[40m");  // BLACK
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[41m");  // RED
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[42m");  // GREEN
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[43m");  // YELLOW
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[44m");  // BLUE
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[45m");  // MAGENTA
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[46m");  // CYAN
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[47m");  // WHITE
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[100m"); // HBLACK
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[101m"); // HRED
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[102m"); // HGREEN
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[103m"); // HYELLOW
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[104m"); // HBLUE
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[105m"); // HMAGENTA
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[106m"); // HCYAN
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[107m"); // HWHITE
  for(i=0; i<space; i++) Terminal.write(" ");
  Terminal.write("\e[40m\r\n"); // reset to black
}

void printinfo()
{
  FlashMode_t fm = ESP.getFlashChipMode();

  Terminal.write("\e[36mFabGL - Network VT/ANSI Terminal Server\r\n");
  print16colors(4);
  Terminal.write("\e[100m\e[93m(c)2021 by Denis Bodor \e[92m\e[40m\r\n\n");
  Terminal.printf("\e[92mScreen Size            :\e[93m %d x %d\r\n", DisplayController.getScreenWidth(), DisplayController.getScreenHeight());
//  Terminal.printf("\e[92mViewPort Size          :\e[93m %d x %d\r\n", DisplayController.getViewPortWidth(), DisplayController.getViewPortHeight());
  Terminal.printf("\e[92mTerminal Size          :\e[93m %d x %d\r\n", Terminal.getColumns(), Terminal.getRows());
  Terminal.printf("\e[92m----\r\n");
  Terminal.printf("\e[92mCPU model              :\e[93m %s @%lu MHz\r\n", ESP.getChipModel(), ESP.getCpuFreqMHz());
  Terminal.printf("\e[92mFlash size             :\e[93m %u\r\n", ESP.getFlashChipSize());
  Terminal.printf("\e[92mFlash mode             :\e[93m %s\r\n", (fm==FM_QIO ? "QIO" : fm==FM_QOUT ? "QOUT" : fm==FM_DIO ? "DIO" : fm==FM_DOUT ? "DOUT" : "UNKNOWN"));
  Terminal.printf("\e[92mFlash speed            :\e[93m %u\r\n", ESP.getFlashChipSpeed());
  
  Terminal.printf("\e[92mCode size              :\e[93m %u\r\n", ESP.getSketchSize());
  Terminal.printf("\e[92mFree code space        :\e[93m %u\r\n", ESP.getFreeSketchSpace());
  Terminal.printf("\e[92m----\r\n");
  Terminal.printf("\e[92mFree DMA Memory        :\e[93m %d\r\n", heap_caps_get_free_size(MALLOC_CAP_DMA));
  Terminal.printf("\e[92mFree internal Memory   :\e[93m %d\r\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Terminal.printf("\e[92mFree PSRAM Memory      :\e[93m %d\r\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  Terminal.printf("\e[92mFree 32 bit Memory     :\e[93m %d\r\n", heap_caps_get_free_size(MALLOC_CAP_32BIT));
  Terminal.printf("\e[92m----\r\n");
  Terminal.printf("\e[92mValue from pin 35      :\e[93m %d\r\n", analogRead(35));
  Terminal.printf("\e[92mAverage value from pin :\e[93m %d\r\n", BL.pinRead());
  Terminal.printf("\e[92mVolts                  :\e[93m %f\r\n", BL.getBatteryVolts());
  Terminal.printf("\e[92mCharge level           :\e[93m %d%%\r\n", BL.getBatteryChargeLevel());
  Terminal.printf("\e[92m----\r\n");
  Terminal.printf("\e[92mMAC address            :\e[93m %s%\r\n", WiFi.macAddress().c_str());
  if (WiFi.status() == WL_CONNECTED) {
    Terminal.printf("\e[92mWiFi SSID              :\e[93m %s\r\n", WiFi.SSID().c_str());
    Terminal.printf("\e[92mCurrent IP             :\e[93m %s\r\n", WiFi.localIP().toString().c_str());
    Terminal.printf("\e[92mHostname               :\e[93m %s.local\r\n", myhostname);
  }
}

/*
void slowPrintf(const char * format, ...)
{
  va_list ap;
  va_start(ap, format);
  int size = vsnprintf(nullptr, 0, format, ap) + 1;
  if (size > 0) {
    va_end(ap);
    va_start(ap, format);
    char buf[size + 1];
    vsnprintf(buf, size, format, ap);
    for (int i = 0; i < size; ++i) {
      Terminal.write(buf[i]);
      delay(25);
    }
  }
  va_end(ap);
}

void demo2()
{
  Terminal.write("\e[40;92m"); // background: black, foreground: green
  slowPrintf("16 colors supported\r\n");
  slowPrintf("ANSI colors:\r\n");
  // foregrounds
  Terminal.write("\e[31mRED\t"); delay(500);
  Terminal.write("\e[32mGREEN\t"); delay(500);
  Terminal.write("\e[33mYELLOW\t"); delay(500);
  Terminal.write("\e[34mBLUE\t"); delay(500);
  Terminal.write("\e[35mMAGENTA\t"); delay(500);
  Terminal.write("\e[36mCYAN\t"); delay(500);
  Terminal.write("\e[37mWHITE\r\n"); delay(500);
  Terminal.write("\e[90mHBLACK\t"); delay(500);
  Terminal.write("\e[91mHRED\t"); delay(500);
  Terminal.write("\e[92mHGREEN\t"); delay(500);
  Terminal.write("\e[93mHYELLOW\t"); delay(500);
  Terminal.write("\e[94mHBLUE\t"); delay(500);
  Terminal.write("\e[95mHMAGENTA\t"); delay(500);
  Terminal.write("\e[96mHCYAN\t"); delay(500);
  Terminal.write("\e[97mHWHITE\r\n"); delay(500);
  // backgrounds
  Terminal.write("\e[40mBLACK\t"); delay(500);
  Terminal.write("\e[41mRED\e[40m\t"); delay(500);
  Terminal.write("\e[42mGREEN\e[40m\t"); delay(500);
  Terminal.write("\e[43mYELLOW\e[40m\t"); delay(500);
  Terminal.write("\e[44mBLUE\e[40m\t"); delay(500);
  Terminal.write("\e[45mMAGENTA\e[40m\t"); delay(500);
  Terminal.write("\e[46mCYAN\e[40m\t"); delay(500);
  Terminal.write("\e[47mWHITE\e[40m\r\n"); delay(500);
  Terminal.write("\e[100mHBLACK\e[40m\t"); delay(500);
  Terminal.write("\e[101mHRED\e[40m\t"); delay(500);
  Terminal.write("\e[102mHGREEN\e[40m\t"); delay(500);
  Terminal.write("\e[103mHYELLOW\e[40m\t"); delay(500);
  Terminal.write("\e[104mHBLUE\e[40m\t"); delay(500);
  Terminal.write("\e[105mHMAGENTA\e[40m\t"); delay(500);
  Terminal.write("\e[106mHCYAN\e[40m\r\n"); delay(500);
}

void demo3()
{
  Terminal.write("\e[40;92m"); // background: black, foreground: white
  slowPrintf("\nSupported styles:\r\n");
  slowPrintf("\e[97m\e[0mNormal\r\n");
  slowPrintf("\e[97m\e[1mBold\e[0m\r\n");
  slowPrintf("\e[97m\e[4mUnderlined\e[0m\r\n");
  slowPrintf("\e[97m\e[5mBlink\e[0m\r\n");
  slowPrintf("\e[97m\e[7mInverse\e[0m\r\n");
  slowPrintf("\e[97m\e[1;4mBoldUnderlined\e[0m\r\n");
  slowPrintf("\e[97m\e[1;4;5mBoldUnderlinedBlinking\e[0m\r\n");
  slowPrintf("\e[97m\e[1;4;5;7mBoldUnderlinedBlinkingInverse\e[0m\r\n");
}
*/

void setup()
{
  int count = 0;
  //Serial.begin(115200); // DEBUG ONLY


  DisplayController.begin();
  //DisplayController.setResolution(VGA_640x480_73Hz);
  //DisplayController.setResolution(VGA_720x400_70Hz);
  DisplayController.setResolution();
  
  Terminal.begin(&DisplayController);
  //Terminal.loadFont(&fabgl::FONT_10x20);
  //Terminal.loadFont(&fabgl::FONT_8x13);
  //Terminal.setLogStream(Serial);  // DEBUG ONLY

  Terminal.setBackgroundColor(Color::Black);
  Terminal.setForegroundColor(Color::BrightWhite);
  Terminal.clear();

  Terminal.write("Connecting to Wifi\r\n");
  WiFi.mode(WIFI_STA);
  WiFi.begin("monSSIDap", "monmotdepasse");
  while(WiFi.status() != WL_CONNECTED && count<=16) {
    Terminal.write(".");
    delay(500);
    count++;
  }
  if(count>16) {
    Terminal.write("\r\nUnable to connect Wifi. Rebooting... in 3s\r\n");
    delay(3000);
    ESP.restart();
  }
  Terminal.write("\r\nWiFi connected\r\n");

  // configuration OTA
  confOTA();

  server.begin();
  server.setNoDelay(true);

  printinfo();
  
  //demo3();
}

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void loop()
{
  unsigned long currentMillis = millis();

  if(server.hasClient()){
    if (!client || !client.connected()){
      if(client) client.stop();
      client = server.available();
      //Terminal.write("Connexion client\r\n");
      //Terminal.clear();
      //printinfo();
    }
  }

  if(client && client.connected()) {
    while(client.available()) {
      int c = client.read();
      if(c=='\n')
        Terminal.write('\r');
      Terminal.write(c);
      //Terminal.write(client.read());
    }
  }

  if(currentMillis - previousMillis >= interval) {
    /*
    Terminal.printf("\e[92mValue:\e[93m %04d   \e[92mVolts:\e[93m %f   \e[92mLevel: \e[93m[", BL.pinRead(), BL.getBatteryVolts(), BL.getBatteryChargeLevel());
    int lev = BL.getBatteryChargeLevel();
    for(int i=0; i<20; i++) {
      if(lev >= i*5)
        Terminal.printf("\e[36m\e[7m \e[0m");
      else
        Terminal.printf("\e[36m ");
    }
    Terminal.printf("\e[93m] %d%%\r\n", lev);
    */

    if(WiFi.status() != WL_CONNECTED) {
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
    }
  previousMillis = currentMillis;
  }
  
  // gestion OTA
  ArduinoOTA.handle();
}
