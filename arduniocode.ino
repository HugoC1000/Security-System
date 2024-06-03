#include <M5Core2.h>
#include <SPIFFS.h>
#include <vector>
ButtonColors onCol = {BLACK, WHITE, WHITE};
ButtonColors offCol = {WHITE, WHITE, WHITE};
Button key1(0, 30, 106, 52, false, "1", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key2(106, 30, 106, 52, false, "2", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key3(212, 30, 106, 52, false, "3", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key4(0, 82, 106, 52, false, "4", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key5(106, 82, 106, 52, false, "5", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key6(212, 82, 106, 52, false, "6", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key7(0, 134, 106, 52, false, "7", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key8(106, 134, 106, 52, false, "8", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key9(212, 134, 106, 52, false, "9", onCol, offCol, MC_DATUM, -10, 0, 0);
Button keyClear(0, 186, 106, 52, false, "Clear", onCol, offCol, MC_DATUM, -10, 0, 0);
Button key0(106, 186, 106, 52, false, "0", onCol, offCol, MC_DATUM, -10, 0, 0);
Button keySubmit(212, 186, 106, 52, false, "Confirm", onCol, offCol, MC_DATUM, -10, 0, 0);
Button homeButton(22, true, 10);
Button logKey(  80, 60, 152, 52, false ,"Access Logs", onCol, offCol,MC_DATUM,0,0,5);
Button changePW( 80,  124, 152, 52, false ,"Edit Password", onCol, offCol,MC_DATUM,0,0,5);
// 下段
Button alarmOnOff( 80,  188, 152, 52, false ,"Alarm: On", onCol, offCol,MC_DATUM,0,0,5);
const char *logFileName = "/motion_log.txt";
int PIR_PIN = 33;

String inputString = "";
String password = "314159";
String pageStatus = "login";
String alarmStatus = "On";

int maxLogEntries = 8;

// void colorButtons(Event& e) {
//   if (e.button != nullptr) {
//     Button& b = *e.button;
//     if(b.label()!="background"){
//       // M5.Lcd.fillRect(b.x, b.y, b.w, b.h, b.isPressed() ? WHITE : BLACK);
//       // M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
//       // M5.Lcd.setCursor(b.x + 53, b.y + 27); 
//       // M5.Lcd.println(b.label());
//     }
//   }
// }

String getCurrentDateTime() {
  time_t now = time(nullptr);
  struct tm *timeinfo;
  timeinfo = localtime(&now);
  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buf);
}

void logMotionEvent(const char *event) {
    Serial.println("Logged Motion Event");
  String logEntry =  getCurrentDateTime() + ": Motion detected";
  File logFile = SPIFFS.open(logFileName, FILE_APPEND);
  if (logFile) {
    logFile.println(logEntry);
    logFile.close();
    Serial.println(logEntry); // Print to Serial monitor
  } else {
    Serial.println("Failed to open log file for writing");
  }
}

//Initialize SPIFFS
void initSPIFFS() {
  Serial.println("Initialized SPIFFS.");
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  } else {
    Serial.println("SPIFFS mounted successfully");
  }
}

void printLogFile() {
  Serial.println("Print Log File Function Accessed");
  File logFile = SPIFFS.open(logFileName, FILE_READ);
  if (!logFile) {
    Serial.println("Failed to open log file for reading");
    return;
  }
  M5.Lcd.setCursor(5, 64);
  std::vector<String> lines;
  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');
    lines.push_back(line);
  }
  logFile.close();
  
  // Print lines from the end to the beginning
  int count = 0;
  for (int i = lines.size() - 1; i >= 0 && count < maxLogEntries; i--) {
    Serial.println(lines[i]);
    M5.Lcd.println(lines[i]);
    count++;
  }
}

void addHandlers(){
  key1.addHandler(inputIntoKeypad1,E_RELEASE);
  key2.addHandler(inputIntoKeypad2,E_RELEASE);
  key3.addHandler(inputIntoKeypad3,E_RELEASE);
  key4.addHandler(inputIntoKeypad4,E_RELEASE);
  key5.addHandler(inputIntoKeypad5,E_RELEASE);
  key6.addHandler(inputIntoKeypad6,E_RELEASE);
  key7.addHandler(inputIntoKeypad7,E_RELEASE);
  key8.addHandler(inputIntoKeypad8,E_RELEASE);
  key9.addHandler(inputIntoKeypad9,E_RELEASE);
  key0.addHandler(inputIntoKeypad0,E_RELEASE);
  keyClear.addHandler(clearKeypadInputKey,E_RELEASE);
  keySubmit.addHandler(submitKeypadInput,E_RELEASE);
  //M5.Buttons.addHandler(colorButtons, E_TOUCH + E_RELEASE);

  alarmOnOff.addHandler(changeAlarmStatus,E_RELEASE);
  logKey.addHandler(setUpLog,E_RELEASE);
  changePW.addHandler(setUpChangePW,E_RELEASE);
}

void hideButtons(){
  key1.hide();
  key2.hide();
  key3.hide();
  key4.hide();
  key5.hide();
  key6.hide();
  key7.hide();
  key8.hide();
  key9.hide();
  key0.hide();
  keyClear.hide();
  keySubmit.hide();


  logKey.hide();
  changePW.hide();
  alarmOnOff.hide();

}

void setUpKeyPad(){
    Serial.println("Set up keypad accessed.");
  M5.Lcd.fillRect(0,0, 320, 240, BLACK);

  hideButtons();
  M5.Lcd.setTextSize(1);
  key1.draw();
  key2.draw();
  key3.draw();
  key4.draw();
  key5.draw();
  key6.draw();
  key7.draw();
  key8.draw();
  key9.draw();
  key0.draw();
  keyClear.draw();
  keySubmit.draw();

  Serial.println("Set up keypad");
}

void inputIntoKeypad1(Event& e){
  Serial.println("Key 1 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "1";
  }
}

void inputIntoKeypad2(Event& e){
    Serial.println("Key 2 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "2";
  }
}
void inputIntoKeypad3(Event& e){
    Serial.println("Key 3 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "3";
  }
}
void inputIntoKeypad4(Event& e){
    Serial.println("Key 4 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "4";
  }
}
void inputIntoKeypad5(Event& e){
    Serial.println("Key 5 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "5";
  }
}
void inputIntoKeypad6(Event& e){
    Serial.println("Key 6 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "6";
  }
}
void inputIntoKeypad7(Event& e){
    Serial.println("Key 7 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "7";
  }
}
void inputIntoKeypad8(Event& e){
    Serial.println("Key 8 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "8";
  }
}
void inputIntoKeypad9(Event& e){
    Serial.println("Key 9 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "9";
  }
}
void inputIntoKeypad0(Event& e){
    Serial.println("Key 0 Pressed.");
  if(inputString.length() < 20){
    M5.Lcd.fillRect(0,0, 320, 30, BLACK);
    inputString += "0";
  }
}

void changeAlarmStatus(Event& e){
  if(alarmStatus == "On"){
    alarmStatus = "Off";
    alarmOnOff.setLabel("Alarm: Off");
    Serial.println("Alarm Turned off");
    String logEntry = getCurrentDateTime() + ": Alarm turned Off";
    File logFile = SPIFFS.open(logFileName, FILE_APPEND);
    if (logFile) {
      logFile.println(logEntry);
      logFile.close();
      Serial.println(logEntry); // Print to Serial monitor
    } else {
      Serial.println("Failed to open log file for writing");
    }
  }else{
    alarmStatus = "On";
    alarmOnOff.setLabel("Alarm: On");
    Serial.println("Alarm Turned On");

    String logEntry = getCurrentDateTime() + ": Alarm turned On";
    File logFile = SPIFFS.open(logFileName, FILE_APPEND);
    if (logFile) {
      logFile.println(logEntry);
      logFile.close();
      Serial.println(logEntry); // Print to Serial monitor
    } else {
      Serial.println("Failed to open log file for writing");
    }
  }

  
}

void clearKeypadInputKey(Event& e){
  clearKeypadInput();
    Serial.println("Clear Input Key Pressed.");
}

void clearKeypadInput(){
  M5.Lcd.fillRect(0,0, 320, 30, BLACK);
  inputString = "";
}


void setUpHomeScreen(){
    Serial.println("Set up homescreen accesed.");

  M5.Lcd.fillRect(0,0, 320, 240, BLACK);
  hideButtons();
  

  logKey.draw();
  changePW.draw();
  alarmOnOff.draw();
  M5.Lcd.drawFastHLine(0, 48, 320, WHITE);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(5, 36);
  M5.Lcd.println("Welcome back!");

  pageStatus = "homeScreen";
  
}

void submitKeypadInput(Event &e){
  Serial.println("Submit keypad input accessed.");
  if(pageStatus == "Change Password"){
    password = inputString;
    setUpHomeScreen();
  }else{
  if(checkPassword()){
    setUpHomeScreen();
  }else{

  }
  }
  inputString = "";
  pageStatus = "homeScreen";
}

bool checkPassword(){
  if(inputString == password){
    Serial.println("Password is correct.");
    return true;
  }else{
      Serial.println("Password is incorrect.");
    return false;
  }
  clearKeypadInput();
}

void setUpLog(Event& e){
    Serial.println("Set up log pressed.");
  M5.Lcd.fillRect(0,0, 320, 240, BLACK);
  M5.Lcd.setTextSize(1);
  hideButtons();

  logKey.hide();
  changePW.hide();
  alarmOnOff.hide();

  M5.Lcd.drawFastHLine(0, 48, 320, WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(5, 36);
  M5.Lcd.println("Displaying most recent logs:");

  printLogFile();
}

void setUpChangePW(Event&e ){
  pageStatus = "Change Password";
  setUpKeyPad();

}

void setup() {
  M5.begin();
  Serial.begin(115200);
  pinMode(33, INPUT);

  addHandlers();

  if(pageStatus == "login"){
    setUpKeyPad();
  }
  if(pageStatus == "homeScreen"){
    setUpHomeScreen();
  }

  // Initialize SPIFFS
  initSPIFFS();


  File logFile = SPIFFS.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.close();
  } else {
    Serial.println("Failed to create log file");
  }


}

int lastMotionDetect = 0;

void loop() {
  M5.update();
  if (M5.BtnB.wasPressed()){
    Serial.println("Home Button pressed.");
    if(pageStatus != "login"){
      setUpHomeScreen();
    }
  }
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 15);
  M5.Lcd.println(inputString);

  static int lastMotionState = LOW;

  int currMotionDetect = millis();
  int motionState = digitalRead(PIR_PIN);

  
  if(currMotionDetect - lastMotionDetect > 5000){
    //   Serial.println("WHY IS THIS NOT WORKING: ");
    //  Serial.println(currMotionDetect);
    //   Serial.println(lastMotionDetect);
    //   Serial.println(currMotionDetect - lastMotionDetect);
      if (motionState == HIGH && lastMotionState == LOW) {
      // Motion detected
      logMotionEvent("Motion detected!");
      lastMotionDetect = currMotionDetect;
    } else if (motionState == LOW && lastMotionState == HIGH) {
      // Motion stopped
      logMotionEvent("Motion stopped.");
      lastMotionDetect = currMotionDetect;
    }

    
  }
  
  lastMotionState = motionState;

}
