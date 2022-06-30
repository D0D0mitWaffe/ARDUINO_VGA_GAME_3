#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // Touchscreen library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_MINX 117
#define TS_MINY 101
#define TS_MAXX 942
#define TS_MAXY 906

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


#define X_MAX 319
#define Y_MAX 239

//Drawable Letters
static const char str0[] PROGMEM = "0";
static const char str1[] PROGMEM = "1";
static const char str2[] PROGMEM = "2";
static const char str3[] PROGMEM = "3";
static const char str4[] PROGMEM = "4";
static const char str5[] PROGMEM = "5";
static const char str6[] PROGMEM = "6";
static const char str7[] PROGMEM = "7";
static const char str8[] PROGMEM = "8";
static const char str9[] PROGMEM = "9";
String s0 = "0";
String s1 = "1";
String s2 = "2";
String s3 = "3";
String s4 = "4";
String s5 = "5";
String s6 = "6";
String s7 = "7";
String s8 = "8";
String s9 = "9";

/************************************************************************/
class GameObject {
public:
    int posX;
    int posY;
    int posXold;
    int posYold;
    int scaleX;
    int scaleY;
    int color;

    GameObject(int _posX = 0, int _posY = 0, int _posXold = 0, int _posYold = 0, int _scaleX = 1, int _scaleY = 1, int _color = RED) {
        posX = _posX;
        posY = _posY;
        posXold = _posXold;
        posYold = _posYold;
        scaleX = _scaleX;
        scaleY = _scaleY;
        color = _color;
    }
};

//************************************************************************
//Variables
int lolol = 0;
int CurrentFrame = 0;

int moveX = 1;
int moveY = 1;

int moveXold = 0;
int moveYold = 0;

//************************************************************************
//Custom Functions
void DrawGameObject(GameObject& g, bool* isMoving) {
    if (g.posX != g.posXold || g.posY != g.posYold) {
        tft.fillRect(g.posXold, g.posYold, g.scaleX, g.scaleY, BLACK);
        g.posXold = g.posX;
        g.posYold = g.posY;
        *isMoving = true;
    }
    tft.fillRect(g.posX, g.posY, g.scaleX, g.scaleY, g.color);
}

bool CollisionCheck(GameObject g1 = GameObject(1, 1, 0, 0, 4, 4, RED), GameObject g2 = GameObject(1, 1, 0, 0, 4, 4, RED)) {
    //Maximale Coordinate der Objecte auf der X Achse 
    int g1_posXmax = g1.posX + g1.scaleX;
    int g2_posXmax = g2.posX + g2.scaleX;

    if ((g1.posX <= g2.posX && g1_posXmax >= g2.posX) || (g1.posX <= g2_posXmax && g1_posXmax >= g2_posXmax)) {
        //Wenn die x coordinaten nicht mit den coordinaten des anderen GameObjects überschneiden, dann maximale Coordinate der Objecte auf der Y Achse besorgen
        int g1_posYmax = g1.posY + g1.scaleY;
        int g2_posYmax = g2.posY + g2.scaleY;
        if ((g1.posY <= g2.posY && g1_posYmax >= g2.posY) || (g1.posY <= g2_posYmax && g1_posYmax >= g2_posYmax)) return true;
    }

    return false;
}
//End custom Functions
//************************************************************************
//Test Game

bool firstObject = true;
GameObject g;
GameObject movingCube = GameObject(10, 10, 9, 9, 8, 8, RED);
int scoreTestGame = 0;

void hasMovedUpdate() {
    if (firstObject) {
        int RPosX = random(0, X_MAX);
        int RPosY = random(0, Y_MAX);
        g = GameObject(RPosX, RPosY, RPosX, RPosY, 4, 4, YELLOW);
        firstObject = false;
    }
    else if (CollisionCheck(movingCube, g)) {
        //neues Object erstellen
        tft.fillRect(g.posX, g.posY, g.scaleX, g.scaleY, BLACK);
        int RPosX = random(0, X_MAX);
        int RPosY = random(0, Y_MAX);
        g = GameObject(RPosX, RPosY, RPosX, RPosY, 4, 4, YELLOW);
        //Score erhöhen, da collidiert
        scoreTestGame++;
    }
    //Display Score
    tft.setCursor(280, 20);
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(2);
    tft.println(String(scoreTestGame));
    //Display Umrandung zeichnen, nur wenn der Stein sich bewegt hat...
    tft.drawPixel(0, 0, RED);
    tft.drawPixel(X_MAX, Y_MAX, RED);
    tft.drawPixel(X_MAX, 0, RED);
    tft.drawPixel(0, Y_MAX, RED);
    //GameObject zeichnen
    tft.fillRect(g.posX, g.posY, g.scaleX, g.scaleY, g.color);
}

void TestGame() {
    bool isMoving;
    DrawGameObject(movingCube, &isMoving);
    if (isMoving) hasMovedUpdate();
    movingCube.posYold = movingCube.posY;

    //Movement by Serial.Read() Controll with WASD keys
    int input = Serial.read();
    if (input == 119) {                           //W = 119
        movingCube.posY -= 5;                      //A = 97
    }                                             //S = 115
    else if (input == 115) {                      //D = 100
        movingCube.posY += 5;
    }
    else if (input == 97) {
        movingCube.posX -= 5;
    }
    else if (input == 100) {
        movingCube.posX += 5;
    }
    //reset player, if out of bounds
    if (movingCube.posX < 0) movingCube.posX = X_MAX;
    if (movingCube.posX > X_MAX) movingCube.posX = 0;
    if (movingCube.posY < 0) movingCube.posY = Y_MAX;
    if (movingCube.posY > Y_MAX) movingCube.posY = 0;
}

//************************************************************************
//Flappy Bird

GameObject bird = GameObject(30, 30, 30, 30, 4, 4, YELLOW);

bool spaceLastPressed = false;

float currentJumpingCurve[3] = { 0, 0, 0 };
//-4,-5,-3
float defaultJumpingCurve[3] = { -0.5, -1, -0.25 };
float birdSpeed = 0;
float birdPosYFloat = 0;

//Physics
void BirdUpdate() {
    //Springen auf Leertastendruck
    if (Serial.read() == 100/*SPACBAR*/) {
        if (spaceLastPressed == false) {
            spaceLastPressed = true;
            //hier Aktionen bei Leertastendruck einfügen
            for (int i = 0; i < 3; i++) {
                currentJumpingCurve[i] = defaultJumpingCurve[i];
                birdSpeed /= 3;
            }
        }
    }
    else {
        spaceLastPressed = false;
    }

    birdSpeed += currentJumpingCurve[0];

    birdSpeed += 0.1;

    for (int i = 0; i < 2; i++) {
        currentJumpingCurve[i] = currentJumpingCurve[i + 1];
    }

    currentJumpingCurve[2] = 0;

    if (birdSpeed > 2) birdSpeed = 2;        //Maximal Geschwindigkeit
    else if (birdSpeed < -2) birdSpeed = -2; //Minimal Geschwindigkeit

    //Generelle Controllen, damit der Bird in sichtbaren Bereich bleibt
    if (birdPosYFloat >= 239) {
        birdPosYFloat = 0;
    }
    else if (birdPosYFloat < 0) {
        birdPosYFloat = 239;
    }

    bird.posY = round(birdPosYFloat);

    DrawGameObject(bird, NULL);
}

//Eviroment
GameObject block1_O = GameObject(160, 0, 160, 0, 1, random(40, 200), GREEN);
GameObject block1_U = GameObject(160, 120, 160, 120, 1, 200, GREEN);

GameObject block2_O = GameObject(320, 0, 320, 0, 1, random(40, 200), GREEN);
GameObject block2_U = GameObject(320, 120, 320, 120, 1, 200, GREEN);

void EnvironmentUpdate() {
    block1_O.posX--;
    block1_U.posX--;

    if (block1_O.posX == -1) {
        tft.fillRect(0, 0, 1, 240, BLACK);  //clear old block
        int rnd = random(40, 190);
        block1_O.scaleY = rnd;
        block1_U.scaleY = 240 - (rnd + 45);
        block1_U.posY = rnd + 45;
        block1_O.posX = 320;
        block1_U.posX = 320;
    }

    DrawGameObject(block1_O, NULL);
    DrawGameObject(block1_U, NULL);

    block2_O.posX--;
    block2_U.posX--;

    if (block2_O.posX == -1) {
        tft.fillRect(0, 0, 1, 240, BLACK);
        int rnd = random(40, 190);
        block2_O.scaleY = rnd;
        block2_U.scaleY = 240 - (rnd + 45);
        block2_U.posY = rnd + 45;
        block2_O.posX = 320;
        block2_U.posX = 320;
    }

    DrawGameObject(block2_O, NULL);
    DrawGameObject(block2_U, NULL);
}

//Ab hier wirds kritisch
//Score
int score = 123;

void setUpDisplaySettings(int x, int y, int foregroundColor, int backgroundColor, int textSize = 2) {
    tft.setCursor(x, y);
    tft.setTextColor(foregroundColor, backgroundColor);
    tft.setTextSize(textSize);
}
//*******************************
void DrawScore() {                  //Performance verlust liegt nicht an der Umwandlung zu string
    Serial.println(score);            //Hängt mit "LenghOfString" zusammen
    String s = String(score);         //Liegt wohl irgendwie an dem switch case konstrukt
    int scoreTemp = score;
    int LengthOfScore = s.length();
    int startPositionX = 300;
    const int startPositionY = 20;
    //Start by converting the score in a array by dividing the int in all its digits in an array
    int digits[LengthOfScore];
    for (int i = 0; i < LengthOfScore; i++) {
        digits[i] = scoreTemp % 10;
        scoreTemp /= 10;
    }
    //Drawing the Score

    //Performance probleme liegen an dem ständigem ausgeben des scores... Also jedes mal, wenn der screen geupdated wird
    //Wird schneller, wenn man die schriftgröße verkleinert und keine Hintergrundfarbe verwendet 
    //Dann hat man aber ein problem, wenn der score geupdatet wird, dass dannn text überreste vorhanden sind.
    //Ich kann nur hoffen, dass die libary in der schule das schneller macht, da sie die pixel besser/anders displayed...

    //Neue option: tft.drawChar(x, y, c, color, background, size); ... vielleicht ist das schneller
    //Ist genauso langsam...
    //Da tft.print auf drawChar() basiert...

    for (int i = 0; i < LengthOfScore; i++) {
        setUpDisplaySettings(startPositionX, startPositionY, WHITE, BLACK);
        int digit = digits[i];
        if (digit == 0) {
            tft.drawChar(startPositionX, startPositionY, '0', WHITE, BLACK, 2);
        }
        else if (digit == 1) {
            tft.drawChar(startPositionX, startPositionY, '1', WHITE, BLACK, 2);
        }
        else if (digit == 2) {
            tft.drawChar(startPositionX, startPositionY, '2', WHITE, BLACK, 2);
        }
        else if (digit == 3) {
            tft.drawChar(startPositionX, startPositionY, '3', WHITE, BLACK, 2);
        }
        else if (digit == 4) {
            tft.drawChar(startPositionX, startPositionY, '4', WHITE, BLACK, 2);
        }
        else if (digit == 5) {
            tft.drawChar(startPositionX, startPositionY, '5', WHITE, BLACK, 2);
        }
        else if (digit == 6) {
            tft.drawChar(startPositionX, startPositionY, '6', WHITE, BLACK, 2);
        }
        else if (digit == 7) {
            tft.drawChar(startPositionX, startPositionY, '7', WHITE, BLACK, 2);
        }
        else if (digit == 8) {
            tft.drawChar(startPositionX, startPositionY, '8', WHITE, BLACK, 2);
        }
        else if (digit == 9) {
            tft.drawChar(startPositionX, startPositionY, '9', WHITE, BLACK, 2);
        }

        startPositionX -= 15;
    }
}

void CheckScore() {

    if (bird.posX == block1_O.posX) {
        //Collision check for block1 with bird
        if ((CollisionCheck(bird, block1_O) == false) && (CollisionCheck(bird, block1_U) == false)) {
            score++;
        }
    }
    else if (bird.posX == block2_O.posX) {
        //Collision check for block2 with bird
        if ((CollisionCheck(bird, block2_O) == false) && (CollisionCheck(bird, block2_U) == false)) {
            score++;
        }
    }

    DrawScore();
}

void FlappyBird() {
    //4 Eck pixel
    tft.drawPixel(0, 0, RED);
    tft.drawPixel(0, Y_MAX, RED);
    tft.drawPixel(X_MAX, 0, RED);
    tft.drawPixel(X_MAX, Y_MAX, RED);

    BirdUpdate();

    EnvironmentUpdate();
    CheckScore();
}

//************************************************************************
//Main
void setup() {
    Serial.begin(115200);
    Serial.println(F("TFT LCD test"));
    tft.reset();

    uint16_t identifier = tft.readID();
    identifier = 0x9341;
    tft.begin(identifier);
    tft.setRotation(3);
    tft.fillScreen(BLACK);
    tft.drawPixel(0, 0, RED);
    tft.drawPixel(X_MAX, Y_MAX, RED);
    tft.drawPixel(X_MAX, 0, RED);
    tft.drawPixel(0, Y_MAX, RED);
}

void loop() {
    //TestGame();
    FlappyBird();
}

//Liegt irgendwie an der TouchSteuerung

/*TSPoint p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  if(p.z > ts.pressureThreshhold){
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    if(p.x > 0 && p.x < 170 && p.y > 50 && p.y < 190){
      movingCube.posX--;
      Serial.print("cube wird nach links bewegt");
    } else if(p.x > 170 && p.x < 320 && p.y > 50 && p.y < 190){
        movingCube.posX++;
        Serial.print("cube wird nach rechts bewegt");
    } else if(p.x > 50 && p.x < 270 && p.y > 0 && p.y < 50){
        movingCube.posY--;
        Serial.print("cube wird nach oben bewegt");
    } else if(p.x > 50 && p.x < 270 && p.y > 190 && p.y < 240){
        movingCube.posY++;
        Serial.print("cube wird nach unten bewegt");
    }
  }*/

  /*    komischer müll code der als alternative zu den if gemetzel vorhanden ist... Ist beides extrem langsam
  switch (digits[i]) {
        case 0:
          tft.println(s0);
          break;
        case 1:
          tft.println(s1);
          break;
        case 2:
          tft.println(s2);
          break;
        case 3:
          tft.println(s3);
          break;
        case 4:
          tft.println(s4);
          break;
        case 5:
          tft.println(s5);
          break;
        case 6:
          tft.println(s6);
          break;
        case 7:
          tft.println(s7);
          break;
        case 8:
          tft.println(s8);
          break;
        case 9:
          tft.println(s9);
          break;
      }
  */