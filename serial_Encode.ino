
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

String textToMorse(char c)
{
    switch (toupper(c))
    {
    case 'A':
        return ".-";
    case 'B':
        return "-...";
    case 'C':
        return "-.-.";
    case 'D':
        return "-..";
    case 'E':
        return ".";
    case 'F':
        return "..-.";
    case 'G':
        return "--.";
    case 'H':
        return "....";
    case 'I':
        return "..";
    case 'J':
        return ".---";
    case 'K':
        return "-.-";
    case 'L':
        return ".-..";
    case 'M':
        return "--";
    case 'N':
        return "-.";
    case 'O':
        return "---";
    case 'P':
        return ".--.";
    case 'Q':
        return "--.-";
    case 'R':
        return ".-.";
    case 'S':
        return "...";
    case 'T':
        return "-";
    case 'U':
        return "..-";
    case 'V':
        return "...-";
    case 'W':
        return ".--";
    case 'X':
        return "-..-";
    case 'Y':
        return "-.--";
    case 'Z':
        return "--..";
    case '0':
        return "-----";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
    case ' ':
        return " ";
    }
    return "";
}

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.print("Serial Encode Mode");
    delay(1500);
    lcd.clear();
}

void loop()
{
    if (Serial.available())
    {
        String inputText = Serial.readStringUntil('\n');
        inputText.trim();
        morseBuffer = "";
        decodedText = inputText;
        updateLCD();

        for (char c : inputText)
        {
            String code = textToMorse(c);
            morseBuffer += code + " ";
            Serial.print(c);
            Serial.print(" => ");
            Serial.println(code);

            if (code == " ")
            {
                delay(3000); // word gap
                continue;
            }

            for (char symbol : code)
            {
                if (symbol == '.')
                    buzzDot();
                else if (symbol == '-')
                    buzzDash();
            }
            delay(1000); // letter gap
        }

        updateLCD();
    }
}
