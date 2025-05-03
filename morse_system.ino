#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int buttonPin = 2;
const int irSensorPin = 3;
const int ledPin = 4;
const int buzzerPin = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int mode = 0;
String morseBuffer = "";
String decodedText = "";
unsigned long pressStartTime = 0;
bool buttonWasPressed = false;
unsigned long lastInputTime = 0;
bool waitingToDecode = false;

unsigned long irBlockStart = 0;
bool irWasBlocked = false;
unsigned long lastActivity = 0;

const int encodeDotThreshold = 400;
const int dotTime = 600;
const int letterGap = 1500;
const int wordGap = 3000;
const int debounceDelay = 50;
const int modeSwitchThreshold = 2000;

String textToMorse(char c) {
  switch (toupper(c)) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case '0': return "-----";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case ' ': return " ";
  }
  return "";
}

char decodeMorse(String code) {
  String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
                      ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
                      "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                      "-----", ".----", "..---", "...--", "....-", ".....", "-....",
                      "--...", "---..", "----."};
  char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  for (int i = 0; i < 36; i++) {
    if (code == letters[i])
      return chars[i];
  }
  return '?';
}

void buzzDot() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(150);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(150);
}

void buzzDash() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(450);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(150);
}

void modeSwitchBeep(int count) {
  for (int i = 0; i < count; i++) {
    tone(buzzerPin, 1200);
    digitalWrite(ledPin, HIGH);
    delay(200);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
    delay(150);
  }
}

void resetSystem() {
  morseBuffer = "";
  decodedText = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Reset");
  delay(1000);
  updateLCD();
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(trimToFit(morseBuffer));
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(trimToFit(decodedText));
}

String trimToFit(String s) {
  if (s.length() > 14)
    return s.substring(s.length() - 14);
  else
    return s;
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(irSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Morse System");
  lcd.setCursor(0, 1);
  lcd.print("Mode: PUSH");
  delay(1500);
  lcd.clear();
  updateLCD();
}

void loop() {
  unsigned long now = millis();
  bool buttonState = (digitalRead(buttonPin) == LOW);
  static bool lastButtonState = HIGH;

  if (buttonState != lastButtonState) {
    delay(debounceDelay);
    buttonState = (digitalRead(buttonPin) == LOW);
  }
  lastButtonState = buttonState;

  if (buttonState && !buttonWasPressed) {
    pressStartTime = now;
    buttonWasPressed = true;
  }

  if (buttonState && buttonWasPressed && (now - pressStartTime >= modeSwitchThreshold)) {
    mode = (mode + 1) % 4;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode Switched");
    lcd.setCursor(0, 1);
    if (mode == 0)
      lcd.print("Push Decode");
    else if (mode == 1)
      lcd.print("IR Decode");
    else if (mode == 2)
      lcd.print("Serial Encode");
    else
      lcd.print("System Reset");

    modeSwitchBeep(mode + 1);
    morseBuffer = "";
    decodedText = "";
    delay(1000);
    updateLCD();
    while (digitalRead(buttonPin) == LOW) {}
    buttonWasPressed = false;
    return;
  }

  if (!buttonState && buttonWasPressed) {
    unsigned long pressDuration = now - pressStartTime;
    buttonWasPressed = false;

    if (mode == 0 && pressDuration < modeSwitchThreshold) {
      lastInputTime = now;
      waitingToDecode = true;
      if (pressDuration < encodeDotThreshold) {
        morseBuffer += ".";
        buzzDot();
      } else {
        morseBuffer += "-";
        buzzDash();
      }
      updateLCD();
    }
  }

  if (mode == 0 && waitingToDecode && (now - lastInputTime >= 2000)) {
    char decoded = decodeMorse(morseBuffer);
    decodedText += decoded;
    Serial.print("Decoded Morse: ");
    Serial.print(morseBuffer);
    Serial.print(" => ");
    Serial.println(decoded);
    morseBuffer = "";
    updateLCD();
    waitingToDecode = false;
    lastActivity = now;
  }

  if (mode == 0 && !waitingToDecode && (now - lastActivity >= wordGap) && decodedText.length() > 0) {
    decodedText += ' ';
    updateLCD();
    lastActivity = now;
  }

  if (mode == 1) {
    bool irState = (digitalRead(irSensorPin) == LOW);

    if (irState && !irWasBlocked) {
      irBlockStart = now;
      irWasBlocked = true;
    }

    if (!irState && irWasBlocked) {
      irWasBlocked = false;
      unsigned long duration = now - irBlockStart;
      lastActivity = now;

      if (duration < dotTime) {
        morseBuffer += ".";
        buzzDot();
      } else {
        morseBuffer += "-";
        buzzDash();
      }
      updateLCD();
    }

    if (morseBuffer.length() > 0 && (now - lastActivity >= letterGap)) {
      char decoded = decodeMorse(morseBuffer);
      decodedText += decoded;
      Serial.print("IR Morse: ");
      Serial.print(morseBuffer);
      Serial.print(" => ");
      Serial.println(decoded);
      morseBuffer = "";
      updateLCD();
    }

    if (now - lastActivity >= wordGap) {
      decodedText += ' ';
      updateLCD();
      lastActivity = now;
    }
  }

  if (mode == 2 && Serial.available()) {
    String inputText = Serial.readStringUntil('\n');
    inputText.trim();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sending Morse...");
    Serial.println("Encoding to Morse:");
    morseBuffer = "";
    decodedText = inputText;
    for (char c : inputText) {
      String code = textToMorse(c);
      morseBuffer += code + " ";
      for (int i = 0; i < code.length(); i++) {
        if (code[i] == '.')
          buzzDot();
        else if (code[i] == '-')
          buzzDash();
      }
      delay(500);  
    }
    updateLCD();
  }

  if (mode == 3) {
    resetSystem();
    mode = 0;
  }
}
