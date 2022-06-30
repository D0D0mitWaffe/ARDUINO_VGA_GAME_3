#include <Arduino.h>
#include <VGAX.h>
#include <math.h>
#include <VGAXUtils.h> 
#include <string.h>
#include <stdio.h>

VGAX vga;
VGAXUtils vgaU;

class GameObject {
public:
  int posX;
  int posY;
  int posXold;
  int posYold;
  int scaleX;
  int scaleY;
  int color;      // 0 = Black || 1 = Grün || 2 = Rot || 3 = Geld

  GameObject(int _posX = 0, int _posY = 0, int _posXold = 0, int _posYold = 0, int _scaleX = 1, int _scaleY = 1, int _color = 2) {
    posX = _posX;
    posY = _posY;
    posXold = _posXold;
    posYold = _posYold;
    scaleX = _scaleX;
    scaleY = _scaleY;
    color = _color;
  }


};

#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_SYMBOLS_COUNT 95

#define yellow 3
#define black 0
#define red 2
#define green 1 

int lolol = 0;
int CurrentFrame = 0;

//data size=570 bytes
const unsigned char fnt_nanofont_data[FNT_NANOFONT_SYMBOLS_COUNT][1 + FNT_NANOFONT_HEIGHT] PROGMEM = {
{ 1, 128, 128, 128, 0, 128, 0, }, //glyph '!' code=0
{ 3, 160, 160, 0, 0, 0, 0, }, //glyph '"' code=1
//{ 5, 80, 248, 80, 248, 80, 0, },  //glyph '#' code=2
//{ 5, 248, 248, 248, 248, 248, 0, },  //glyph '#' code=2 - full rectangle
{ 5, 240, 240, 240, 240, 240, 240, },  //glyph '#' = 'full rectangle' code=2
//{ 5, 32, 160, 240, 120, 32, 32, },  //glyph '#' = 'planeR' code=2
{ 3, 80, 32, 112, 112, 32, 0, },  //glyph '$' = 'bomb' code=3
{ 5, 32, 40, 120, 240, 32, 32, },  //glyph '%' = 'planeL' code=4
{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
{ 5, 248, 248, 248, 248, 248, 248, },  //glyph ''' = 'rectangle 5 x 6' code=6
//{ 5, 120, 160, 112, 40, 240, 0, },  //glyph '$' code=3
//{ 5, 136, 16, 32, 64, 136, 0, },  //glyph '%' code=
//{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
//{ 2, 128, 64, 0, 0, 0, 0, },  //glyph ''' code=6
{ 2, 64, 128, 128, 128, 64, 0, }, //glyph '(' code=7
{ 2, 128, 64, 64, 64, 128, 0, },  //glyph ')' code=8
{ 3, 0, 160, 64, 160, 0, 0, },  //glyph '*' code=9
{ 3, 0, 64, 224, 64, 0, 0, }, //glyph '+' code=10
{ 2, 0, 0, 0, 0, 128, 64, },  //glyph ',' code=11
{ 3, 0, 0, 224, 0, 0, 0, }, //glyph '-' code=12
{ 1, 0, 0, 0, 0, 128, 0, }, //glyph '.' code=13
{ 5, 8, 16, 32, 64, 128, 0, },  //glyph '/' code=14
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph '0' code=15
{ 3, 64, 192, 64, 64, 224, 0, },  //glyph '1' code=16
{ 4, 224, 16, 96, 128, 240, 0, }, //glyph '2' code=17
{ 4, 224, 16, 96, 16, 224, 0, },  //glyph '3' code=18
{ 4, 144, 144, 240, 16, 16, 0, }, //glyph '4' code=19
{ 4, 240, 128, 224, 16, 224, 0, },  //glyph '5' code=20
{ 4, 96, 128, 224, 144, 96, 0, }, //glyph '6' code=21
{ 4, 240, 16, 32, 64, 64, 0, }, //glyph '7' code=22
{ 4, 96, 144, 96, 144, 96, 0, },  //glyph '8' code=23
{ 4, 96, 144, 112, 16, 96, 0, },  //glyph '9' code=24
{ 1, 0, 128, 0, 128, 0, 0, }, //glyph ':' code=25
{ 2, 0, 128, 0, 0, 128, 64, },  //glyph ';' code=26
{ 3, 32, 64, 128, 64, 32, 0, }, //glyph '<' code=27
{ 3, 0, 224, 0, 224, 0, 0, }, //glyph '=' code=28
{ 3, 128, 64, 32, 64, 128, 0, },  //glyph '>' code=29
{ 4, 224, 16, 96, 0, 64, 0, },  //glyph '?' code=30
{ 4, 96, 144, 176, 128, 112, 0, },  //glyph '@' code=31
{ 4, 96, 144, 240, 144, 144, 0, },  //glyph 'A' code=32
{ 4, 224, 144, 224, 144, 224, 0, }, //glyph 'B' code=33
{ 4, 112, 128, 128, 128, 112, 0, }, //glyph 'C' code=34
{ 4, 224, 144, 144, 144, 224, 0, }, //glyph 'D' code=35
{ 4, 240, 128, 224, 128, 240, 0, }, //glyph 'E' code=36
{ 4, 240, 128, 224, 128, 128, 0, }, //glyph 'F' code=37
{ 4, 112, 128, 176, 144, 112, 0, }, //glyph 'G' code=38
{ 4, 144, 144, 240, 144, 144, 0, }, //glyph 'H' code=39
{ 3, 224, 64, 64, 64, 224, 0, },  //glyph 'I' code=40
{ 4, 240, 16, 16, 144, 96, 0, },  //glyph 'J' code=41
{ 4, 144, 160, 192, 160, 144, 0, }, //glyph 'K' code=42
{ 4, 128, 128, 128, 128, 240, 0, }, //glyph 'L' code=43
{ 5, 136, 216, 168, 136, 136, 0, }, //glyph 'M' code=44
{ 4, 144, 208, 176, 144, 144, 0, }, //glyph 'N' code=45
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph 'O' code=46
{ 4, 224, 144, 224, 128, 128, 0, }, //glyph 'P' code=47
{ 4, 96, 144, 144, 144, 96, 16, },  //glyph 'Q' code=48
{ 4, 224, 144, 224, 160, 144, 0, }, //glyph 'R' code=49
{ 4, 112, 128, 96, 16, 224, 0, }, //glyph 'S' code=50
{ 3, 224, 64, 64, 64, 64, 0, }, //glyph 'T' code=51
{ 4, 144, 144, 144, 144, 96, 0, },  //glyph 'U' code=52
{ 3, 160, 160, 160, 160, 64, 0, },  //glyph 'V' code=53
{ 5, 136, 168, 168, 168, 80, 0, },  //glyph 'W' code=54
{ 4, 144, 144, 96, 144, 144, 0, },  //glyph 'X' code=55
{ 3, 160, 160, 64, 64, 64, 0, },  //glyph 'Y' code=56
{ 4, 240, 16, 96, 128, 240, 0, }, //glyph 'Z' code=57
{ 2, 192, 128, 128, 128, 192, 0, }, //glyph '[' code=58
{ 5, 128, 64, 32, 16, 8, 0, },  //glyph '\' code=59
{ 2, 192, 64, 64, 64, 192, 0, },  //glyph ']' code=60
{ 5, 32, 80, 136, 0, 0, 0, }, //glyph '^' code=61
{ 4, 0, 0, 0, 0, 240, 0, }, //glyph '_' code=62
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph '`' code=63
{ 3, 0, 224, 32, 224, 224, 0, },  //glyph 'a' code=64
{ 3, 128, 224, 160, 160, 224, 0, }, //glyph 'b' code=65
{ 3, 0, 224, 128, 128, 224, 0, }, //glyph 'c' code=66
{ 3, 32, 224, 160, 160, 224, 0, },  //glyph 'd' code=67
{ 3, 0, 224, 224, 128, 224, 0, }, //glyph 'e' code=68
{ 2, 64, 128, 192, 128, 128, 0, },  //glyph 'f' code=69
{ 3, 0, 224, 160, 224, 32, 224, },  //glyph 'g' code=70
{ 3, 128, 224, 160, 160, 160, 0, }, //glyph 'h' code=71
{ 1, 128, 0, 128, 128, 128, 0, }, //glyph 'i' code=72
{ 2, 0, 192, 64, 64, 64, 128, },  //glyph 'j' code=73
{ 3, 128, 160, 192, 160, 160, 0, }, //glyph 'k' code=74
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph 'l' code=75
{ 5, 0, 248, 168, 168, 168, 0, }, //glyph 'm' code=76
{ 3, 0, 224, 160, 160, 160, 0, }, //glyph 'n' code=77
{ 3, 0, 224, 160, 160, 224, 0, }, //glyph 'o' code=78
{ 3, 0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
{ 3, 0, 224, 160, 160, 224, 32, },  //glyph 'q' code=80
{ 3, 0, 224, 128, 128, 128, 0, }, //glyph 'r' code=81
{ 2, 0, 192, 128, 64, 192, 0, },  //glyph 's' code=82
{ 3, 64, 224, 64, 64, 64, 0, }, //glyph 't' code=83
{ 3, 0, 160, 160, 160, 224, 0, }, //glyph 'u' code=84
{ 3, 0, 160, 160, 160, 64, 0, },  //glyph 'v' code=85
{ 5, 0, 168, 168, 168, 80, 0, },  //glyph 'w' code=86
{ 3, 0, 160, 64, 160, 160, 0, },  //glyph 'x' code=87
{ 3, 0, 160, 160, 224, 32, 224, },  //glyph 'y' code=88
{ 2, 0, 192, 64, 128, 192, 0, },  //glyph 'z' code=89
{ 3, 96, 64, 192, 64, 96, 0, }, //glyph '{' code=90
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph '|' code=91
{ 3, 192, 64, 96, 64, 192, 0, },  //glyph '}' code=92
//{ 3, 96, 192, 0, 0, 0, 0, },  //glyph '~' code=93
{ 5, 32, 160, 240, 120, 32, 32, }, //= 'planeR' code=93
{ 4, 48, 64, 224, 64, 240, 0, },  //glyph ' ' code=94
};

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
static const char str10[] PROGMEM = "#";
static const char str15[] PROGMEM = "Red wins!";
static const char str16[] PROGMEM = "Green wins!";
static const char str18[] PROGMEM = "Game Over!!!";
static const char str21[] PROGMEM = "########";
static const char str30[] PROGMEM = "Hoerst du SCHLECHT";
static const char str31[] PROGMEM = "PongLOLLOL";
static const char str32[] PROGMEM = "Breakout";
static const char str33[] PROGMEM = "Bomber";
static const char str34[] PROGMEM = "Etch-a-Sketch";
static const char str35[] PROGMEM = "Tetris";
static const char str40[] PROGMEM = "X";
static const char str51[] PROGMEM = "~"; //planeR
static const char str52[] PROGMEM = "%"; //planeL
static const char str53[] PROGMEM = "$"; //bomb
static const char str54[] PROGMEM = "'"; //rectangle 5 x 6
static const char str59[] PROGMEM = "'''''''"; //clear 'Hit' banner 
static const char str510[] PROGMEM = "Hit!!!"; //'Hit' banner
static const char str511[] PROGMEM = "Arduino VGA Bomber";
static const char str512[] PROGMEM = "by Roberto Melzi";
static const char str513[] PROGMEM = "Blog HALLO";
static const char str514[] PROGMEM = "Schulz tomaten geheimnisse";
static const char str515[] PROGMEM = "Olis Laser Augen";
static const char str516[] PROGMEM = "Bueschgens du Folgst mir jetzt";
static const char str517[] PROGMEM = "ELKES schlechtes gehoer";
static const char str518[] PROGMEM = "Tolles Untermenue";
static const char str519[] PROGMEM = "Untermenue 1";
static const char str520[] PROGMEM = "Untermenue 2";
static const char str521[] PROGMEM = "Untermenue 3";
char str1000[] = "Untermenue 3";

void vgaPrint(const char* str, byte x, byte y, byte color) {            // x = 0 - 119 -> Resolution on X || Y = 0 - 59 || Resolution on Y
  vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str, x, y, color);
}

int moveX = 1;
int moveY = 1;

int moveXold = 0;
int MoveYold = 0;

int test = 1;
int testold = 0;

void DrawGameObject(GameObject& g/* = GameObject(1, 1, 0, 0, 4, 4, red)*/, bool* isMoving = false) {

  if (g.posX != g.posXold || g.posY != g.posYold) {
    vga.fillrect(g.posXold, g.posYold, g.scaleX, g.scaleY, black);
    g.posXold = g.posX;
    g.posYold = g.posY;
    *isMoving = true;
  }

  vga.fillrect(g.posX, g.posY, g.scaleX, g.scaleY, g.color);
}

bool CollisionCheck(GameObject g1 = GameObject(1, 1, 0, 0, 4, 4, red), GameObject g2 = GameObject(1, 1, 0, 0, 4, 4, red)) {
  int g1_posXmax = g1.posX + g1.scaleX - 1;
  int g2_posXmax = g2.posX + g2.scaleX - 1;

  if (g1.posX <= g2.posX && g1_posXmax >= g2.posX || g1.posX <= g2_posXmax && g1_posXmax >= g2_posXmax) {
    int g1_posYmax = g1.posY + g1.scaleY - 1;
    int g2_posYmax = g2.posY + g2.scaleY - 1;
    if (g1.posY <= g2.posY && g1_posYmax >= g2.posY || g1.posY <= g2_posYmax && g1_posYmax >= g2_posYmax)return true;
  }

  return false;

  if (g1.posX == g2.posX && g1.posY == g2.posY)return true;
  return false;
}

GameObject movingCube = GameObject(1, 1, 0, 0, 4, 4, red);

bool firstObject = true;

GameObject g;

void RandomPosition() {

  if (firstObject) {
    int RPosX = random(0, 119);
    int RPosY = random(0, 59);
    g = GameObject(RPosX, RPosY, RPosX, RPosY, 2, 2, yellow);
    firstObject = false;
  }
  else if (CollisionCheck(movingCube, g)) {
    vga.fillrect(g.posX, g.posY, g.scaleX, g.scaleY, black);
    int RPosX = random(0, 119);
    int RPosY = random(0, 59);
    g = GameObject(RPosX, RPosY, RPosX, RPosY, 2, 2, yellow);
  }
  vga.fillrect(g.posX, g.posY, g.scaleX, g.scaleY, g.color);
}


void drawBloguntermenu() {
  /*
  //Basis aus Schrift
  vga
  Print(str518, 20, 2, yellow);
  vgaPrint(str519, 38, 12, yellow);
  vgaPrint(str520, 38, 20, yellow);
  vgaPrint(str521, 38, 28, yellow);
  */
  //GameObject gameobject(12, 123);
  //String hello = String(t.xpos);

  //Die vier eck Pixel als "Border"
  vga.putpixel(0, 0, red);
  vga.putpixel(0, 59, red);
  vga.putpixel(119, 0, red);
  vga.putpixel(119, 59, red);


  bool isMoving;
  //Position des Objectes neu zeichnen und alte Position überzeichnen
  DrawGameObject(movingCube, &isMoving);
  if (isMoving) RandomPosition();


  movingCube.posYold = movingCube.posY;
  if (digitalRead(53) == 1)movingCube.posY++;             //Knopf hoch gedrückt
  if (digitalRead(50) == 1)movingCube.posY--;             //Knopf runter gedrückt
  if (movingCube.posY >= 59)movingCube.posY = 0;
  if (movingCube.posY < 0)movingCube.posY = 59;


  movingCube.posXold = movingCube.posX;
  if (digitalRead(52) == 1)movingCube.posX++;             //Knopf rechts gedrückt
  if (digitalRead(51) == 1)movingCube.posX--;             //Knopf links gedrückt
  if (movingCube.posX >= 119)movingCube.posX = 0;
  if (movingCube.posX < 0)movingCube.posX = 119;
}

//Wichtig Funktion

GameObject bird = GameObject(30, 30, 30, 30, 4, 4, yellow);

bool button_51LastPressed = false;


//int birdUpdateCounter = 0;
float currentjumpingCurve[3] = { 0,0,0 };
//-4,-5,-3
float defaultjumpingCurve[3] = { -0.5,-1,-0.25 };
float birdspeed = 0;
float birdposfloat = 0;

void BirdUpdate() {
  //Serial.println(digitalRead(51));

  //sprigen knopfdruck
  if (digitalRead(51)) {

    if (button_51LastPressed == false) {
      button_51LastPressed = true;

      //hier aktionen bei sprungknof einfügen
      //birdspeed -= 10;
      for (int i = 0; i < 3;i++) {
        currentjumpingCurve[i] += defaultjumpingCurve[i];
        birdspeed /= 3;
      }
    }
  }
  else {
    button_51LastPressed = false;
  }

  birdspeed += currentjumpingCurve[0];

  //Test

    //bird.posY += currentjumpingCurve[0];

    //Normale Gravität, auf den Vogel
    //bird.posY += 1;
  birdspeed += 0.1;

  //setzt currenjumpingcurve ein index weiter für, für den nächsten durchlauf
//currentjumpingCurve[a++];

  for (int i = 0; i < 2;i++) {
    //int ia = currentjumpingCurve[a++];
    //currentjumpingCurve[a] = ia;
    currentjumpingCurve[i] = currentjumpingCurve[i + 1];
  }

  //currentjumpingCurve[0] = currentjumpingCurve[1];
  //currentjumpingCurve[1] = currentjumpingCurve[2];
  currentjumpingCurve[2] = 0;


  //Maximal und Minimalwert der Geschwindigkeit
  if (birdspeed > 2) birdspeed = 2;
  else if (birdspeed < -2) birdspeed = -2;


  //Birdspeed wird auf die Position übertragen, zunächst als float
  birdposfloat += birdspeed;

  //Generelle Kontrollen damit der Bird im Sichbaren Bereich bleibt



  if (birdposfloat >= 59) {						//Nicht Tiefer/höher als 59 px
    birdposfloat = 0;
  }
  else if (birdposfloat < 0) {
    birdposfloat = 59;
  }




  bird.posY = round(birdposfloat);

  DrawGameObject(bird);

}

//pos x, posy , oldx, oldy, scalex, scaley, color 
GameObject block1_O = GameObject(60, 0, 60, 0, 1, 25, green);
GameObject block1_U = GameObject(60, 40, 60, 40, 1, 20, green);


GameObject block2_O = GameObject(120, 0, 120, 0, 1, 25, green);
GameObject block2_U = GameObject(120, 40, 120, 40, 1, 20, green);
void EnvironmentUpdate() {
  //Blöcke werden erstellt

  //DrawGameObject(block1);
  //DrawGameObject(block2);

  block1_O.posX--;
  block1_U.posX--;


  if (block1_O.posX == -1) {
    //das dritte ist ScaleX. Den muss man ändern, wenn man scale x von den Balken ändert
    vga.fillrect(0, 0, 1, 60, black);
    int rnd = random(5, 40);
    block1_O.scaleY = rnd;
    block1_U.scaleY = 45 - rnd;
    block1_U.posY = rnd + 15;
    block1_O.posX = 120;
    block1_U.posX = 120;

  }

  DrawGameObject(block1_O);
  DrawGameObject(block1_U);


  block2_O.posX--;
  block2_U.posX--;

  if (block2_O.posX == -1) {
    //das dritte ist ScaleX. Den muss man ändern, wenn man scale x von den Balken ändert
    vga.fillrect(0, 0, 1, 60, black);
    int rnd = random(5, 40);
    block2_O.scaleY = rnd;
    block2_U.scaleY = 45 - rnd;
    block2_U.posY = rnd + 15;
    block2_O.posX = 120;
    block2_U.posX = 120;

  }

  DrawGameObject(block2_O);
  DrawGameObject(block2_U);

}

int score = 0;
/*
sketch_apr26a.ino: In function 'void ScoreUpdate()':
sketch_apr26a:448: error: initializer fails to determine size of 'strScore'
sketch_apr26a:448: error: array must be initialized with a brace-enclosed initializer
initializer fails to determine size of 'strScore'
*/




//static const char str9[] PROGMEM = "9";
void ScoreUpdate() {
  if (block1_O.posX == 29) {
    score++;
  }
  //String _score = String(score);
  //char strScore[] = _score;
  //vgaPrint(strScore, 59, 20, yellow);

}

void FlappyBird() {
  vga.putpixel(0, 0, red);
  vga.putpixel(0, 59, red);
  vga.putpixel(119, 0, red);
  vga.putpixel(119, 59, red);


  BirdUpdate();


  EnvironmentUpdate();
  ScoreUpdate();

}


void setup() {
  vga.begin();
  randomSeed(1);
  Serial.begin(9600);


  //Game
  //RandomPosition();

}

void loop() {

  lolol = lolol + 1;


  if (lolol >= 2000) {
    lolol = 0;
    CurrentFrame++;
    //drawBloguntermenu();
    FlappyBird();
    //RandomPosition()
  }
}