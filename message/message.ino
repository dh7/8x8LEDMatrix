/*
 * Show messages on an 8x8 led matrix,
 * scrolling from right to left.
 */

#include "font5x7.h"

#define DELAY 800

char message[] = "Et voyageant ainsi qu'on fait en reve, elle arrive au milieu d'une peuplade de negres. \
Et la, suivant la coutume qui s'attache aux fils de roi, l'enfant royal est nourri par la mere et par la nourrice. \
Mais à la nourricee on ne laisse qu'un sein. L'autre est sectionne et la poitrine est plate comme celle d'un homme \
(sauf le nœud de la cicatrice). La voyageuse, voyant cela, s'etonne. Alors le vice-roi : \
\"Vous avez bien remarque comme tout le monde, n'est-ce pas, que quand l'enfant tette, l'autre mamelle, \
il la touche constamment et la caresse. C'est ainsi que ça va le mieux. \
\"Or la nourrice nous en coupons une pour que l'enfant apprenne plus vite a parler. En effet, ce sein absent l'intrigue tellement \
qu'il n'a de cesse qu'il n'ait pu composer un mot et interroger la-dessus son entourage. \
\"Et le premier mot qui vient, c'est toujours : abricot. \"";

byte col = 0;
byte leds[8][8];

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[17]= {
  -1, 17, 8, 7, 19, 9, 13, 4, 10, 6, 5, 18, 3, 2, 11, 12, 16};

// col[xx] of leds = pin yy on led matrix
int cols[8] = {
  pins[13], pins[3], pins[4], pins[10], pins[06], pins[11], pins[15], pins[16]};

int rows[8] = {
  pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};


void setup() {
  // sets the pins as output
  for (int i = 1; i <= 16; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // set up cols and rows
  for (int i = 1; i <= 8; i++) {
    digitalWrite(cols[i - 1], LOW);
  }

  for (int i = 1; i <= 8; i++) {
    digitalWrite(rows[i - 1], LOW);
  }

  clearLeds();
/*
  // Turn off toggling of pin 11
  FrequencyTimer2::disable();
  // Set refresh rate (interrupt timeout period)
  FrequencyTimer2::setPeriod(2000);
  // Set interrupt routine to be called
  FrequencyTimer2::setOnOverflow(display);
*/
}

void scrollMsg( char *msg ) {
  char *ptr = msg;
  clearLeds();
  
  while( *ptr ) {
    slideChar( *ptr++, DELAY );
  }
  // A final space
  slideChar( ' ', DELAY) ;
}



void loop() {
  scrollMsg( message );
  //char L = 'L';
  //setChar(L);
}

void clearLeds() {
  // Clear display array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = 0;
    }
  }
}

void setChar(char ch) {

  for (int i = 0; i < 5; i++) {
    unsigned char bt = pgm_read_byte(&(smallFont [(ch-32)*5 + i] ));
    for (int j = 0; j < 8; j++) {
      leds[j][i+1] = (bt & 0x01);
      bt = bt >> 1;
    }
  }
}

void slideChar(char ch, int del) {
  for (int l = 0; l < 5; l++) {
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 8; j++) {
        leds[j][i] = leds[j][i+1];
      }
    }
    unsigned char bt = pgm_read_byte(&(smallFont [(ch-32)*5 + l] ));
    for (int j = 0; j < 8; j++) {
      leds[j][7] = (bt & 0x01);
      bt = bt >> 1;
    }
    //delay(del);
    for (int d=0;d<del;d++) display();
  }

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 8; j++) {
      leds[j][i] = leds[j][i+1];
    }
  }

  for (int j = 0; j < 8; j++) {
    leds[j][7] = 0;
  }
  //delay(del);
  for (int d=0;d<del;d++) display();
}


// Interrupt routine
void display() {
  digitalWrite(cols[col], LOW);  // Turn whole previous column off
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int row = 0; row < 8; row++) {
    //    if (leds[col][7 - row] == 1) {
    if (leds[col][ row] == 1) {
      digitalWrite(rows[row], LOW);  // Turn on this led
    }
    else {
      digitalWrite(rows[row], HIGH); // Turn off this led
    }
  }
  digitalWrite(cols[col], HIGH); // Turn whole column on at once (for equal lighting times)
}

