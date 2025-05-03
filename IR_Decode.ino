
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ledPin = 4;
const int buzzerPin = 5;
LiquidCrystal_I2C lcd(0x27, 16, 2);

String morseBuffer = "";
String decodedText = "";

void buzzDot()
{
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(150);
}

void buzzDash()
{
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(450);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(150);
}

String trimToFit(String s)
{
    if (s.length() > 14)
        return s.substring(s.length() - 14);
    return s;
}

char decodeMorse(String code)
{
    String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
                        ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
                        "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                        "-----", ".----", "..---", "...--", "....-", ".....", "-....",
                        "--...", "---..", "----."};
    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (int i = 0; i < 36; i++)
    {
        if (code == letters[i])
            return chars[i];
    }
    return '?';
}

void updateLCD()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("M:");
    lcd.print(trimToFit(morseBuffer));
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(trimToFit(decodedText));
}

const int irSensorPin = 3;
unsigned long irBlockStart = 0;
bool irBlocked = false;
unsigned long lastActivity = 0;
const int dotTime = 600;
const int letterGap = 1500;
const int wordGap = 3000;

void setup()
{
    pinMode(irSensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.print("IR Decode Mode");
    delay(1500);
    lcd.clear();
}

void loop()
{
    unsigned long now = millis();
    bool irState = (digitalRead(irSensorPin) == LOW);

    if (irState && !irBlocked)
    {
        irBlockStart = now;
        irBlocked = true;
    }

    if (!irState && irBlocked)
    {
        irBlocked = false;
        unsigned long duration = now - irBlockStart;
        if (duration < dotTime)
        {
            morseBuffer += ".";
            buzzDot();
        }
        else
        {
            morseBuffer += "-";
            buzzDash();
        }
        updateLCD();
        lastActivity = now;
    }

    if (morseBuffer.length() > 0 && (now - lastActivity >= letterGap))
    {
        char decoded = decodeMorse(morseBuffer);
        decodedText += decoded;
        Serial.println(morseBuffer + " => " + decoded);
        morseBuffer = "";
        updateLCD();
    }

    if (now - lastActivity >= wordGap)
    {
        decodedText += ' ';
        updateLCD();
        lastActivity = now;
    }
}
