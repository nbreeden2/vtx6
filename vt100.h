/* VT100.H - Header file to setup defines and functions 
 *   for managing the VT100
 * V1.1 March 27, 2023 - NBBII 
 * https://thehighnibble.com/vt132/
 */

/* Colors */
#define BLACK     30
#define RED       31
#define GREEN     32
#define YELLOW    33
#define BLUE      34
#define MAGENTA   35
#define CYAN      36
#define WHITE     37

/*#ifdef VT100DBG
char *VT100CLR[8];
initptr(VT100CLR,"Black","Red"    ,"Green","Yellow",
                 "Blue" ,"Magenta","Cyan" ,"White" ,NULL);
#endif*/

/*
#ifndef VTDELAY
#define VTDELAY 0
#endif
*/

#define FG         0  /* Colors are for Foreground */
#define BG        10  /* Offset for Background     */
#define BRIGHT    60  /* Offset for bright colors  */
/*
   sColor(GREEN);         will set the character foreground : green
   sColor(GREEN+BG);      will set the character background : green
   sColor(RED+BRIGHT);    will set the character foreground : bright red
   sColor(RED+BG+BRIGHT); will set the character background : bright red
*/

/* Attributes */
#define RESET       0   /* VT100 - All  off   */
#define BOLD        1   /* VT100 - Bold       */
#define DIM         2   /*       - Dim        */
#define ITALIC      3   /*       - Italic     */
#define UNDERSCORE  4   /* VT100 - Underscore */
#define BLINK       5   /* VT100 - Blink      */
#define REVERSE     7   /* VT100 - Reverse    */
#define HIDDEN      8   /*       - Hidden     */

/* character and screen size */
#define COL132    100   /* VT100 - 132 Column mode */
#define COL80     101   /* VT100 -  80 Column mode */
#define ENACOL    102   /* VT100 - Allow   80/132 switching */
#define DISCOL    103   /* VT100 - Disable 80/132 switching */
#define DWSH      104   /* VT100 - Double width single height */
#define SWSH      105   /* VT100 - Single width single height */
#define DHTOP     106   /* VT100 - Double height top row    */
#define DHBOT     107   /* VT100 - Double height bottom row */

/* Character sets
 * For VT132, S-132 and Standalone 132 these may require toggling bit 8
 *   in setup on page 'Set-up D' Option 7 'Toggle MCS/NRCS'
 */
#define UKG0      120
#define UKG1      121
#define USG0      122
#define USG1      123
#define G0SPEC    124
#define G1SPEC    125
#define G0ALT     126
#define G1ALT     127
#define G0ROM     128
#define G1ROM     129

/* misc stuff */
#define BGRESET   140  /* reset the background color to default */

/* LEDs */
#define LEDOFF    160
#define LED1ON    161
#define LED2ON    162
#define LED3ON    163
#define LED4ON    164


/* used in vt100.h - left as global for speed */
char strVT100[15];

/* set both the foreground and background color */
sColors(fg,bg)
int fg,bg;
{
  sprintf(strVT100,"%c[%1d;%1dm",0x1B,fg,bg+BG);
  puts(strVT100);
  /*sleep(VTDELAY);*/
}

/* set any color */
sColor(clr)
int clr;
{
  sprintf(strVT100,"%c[%1dm",0x1B,clr);
  puts(strVT100);
  /*sleep(VTDELAY);*/
}

/* set attribute */
sAttrib(at)
int at;
{
  if (at<=8)
  {
    sprintf(strVT100,"%c[%1dm",0x1B,at);
    puts(strVT100);
    /*sleep(VTDELAY);*/
    return;
  }
  switch(at) {
    case COL132:  sprintf(strVT100,"%c[?3h",0x1B);  break;
    case COL80:   sprintf(strVT100,"%c[?3l",0x1B);  break;
    case ENACOL:  sprintf(strVT100,"%c[?40h",0x1B); break;
    case DISCOL:  sprintf(strVT100,"%c[?40l",0x1B); break;
    case DWSH:    sprintf(strVT100,"%c#6",0x1B);    break;
    case SWSH:    sprintf(strVT100,"%c#5",0x1B);    break;
    case DHTOP:   sprintf(strVT100,"%c#3",0x1B);    break;
    case DHBOT:   sprintf(strVT100,"%c#4",0x1B);    break;
    case UKG0:    sprintf(strVT100,"%c(A",0x1B);    break;
    case UKG1:    sprintf(strVT100,"%c)A",0x1B);    break;
    case USG0:    sprintf(strVT100,"%c(B",0x1B);    break;
    case USG1:    sprintf(strVT100,"%c)B",0x1B);    break;
    case G0SPEC:  sprintf(strVT100,"%c(0",0x1B);    break;
    case G1SPEC:  sprintf(strVT100,"%c)0",0x1B);    break;
    case G0ALT:   sprintf(strVT100,"%c(1",0x1B);    break;
    case G1ALT:   sprintf(strVT100,"%c)1",0x1B);    break;
    case G0ROM:   sprintf(strVT100,"%c(2",0x1B);    break;
    case G1ROM:   sprintf(strVT100,"%c)2",0x1B);    break;
    case BGRESET: sprintf(strVT100,"%c[49m",0x1B); break;
    case LEDOFF:  sprintf(strVT100,"%c[0q",0x1B);  break;
    case LED1ON:  sprintf(strVT100,"%c[1q",0x1B);  break;
    case LED2ON:  sprintf(strVT100,"%c[2q",0x1B);  break;
    case LED3ON:  sprintf(strVT100,"%c[3q",0x1B);  break;
    case LED4ON:  sprintf(strVT100,"%c[4q",0x1B);  break;
    default:      return;
  }
  puts(strVT100);
  /*sleep(VTDELAY);*/
}

/* position the cursor */
positionCursor(row,col)
int row, col;
{
  sprintf(strVT100,"%c[%1d;%1dH",0x1B,row,col);
  puts(strVT100);
  /*sleep(VTDELAY);*/
}

/* clear screen */
clearScreen()
{
  bios(4,0x1B);
  puts("[2J");
  /*sleep(VTDELAY);*/
}

/* Home cursor to 1,1 */
homeCursor()
{
   bios(4,0x1B);
   puts("[H");
   sleep(1);
}

/* clear the screen and home the cursor */
cls()
{
  clearScreen();
  homeCursor();
}

/* Tell VT100 to change to AUX output.  */
/* Output to the VT100 is still parsed  */
/*  by the VT100 but is sent to the aux */
/*  port on the VT100 instead of to     */
/*  the screen.                         */
auxOn(delay)
int delay;
{
  printf("%c[?38h",27);   /* <ESC>[?38h */
  if (delay > 0) sleep(delay);
}

/* Tell VT100 to change to AUX off      */
auxOff(delay)
int delay;
{
  printf("%c%c",27,03);   /* <ESC><ETX> */
  if (delay > 0) sleep(delay);
}

/* used for debugging and demos */
#ifdef VT100DBG
pcolor(clr)
int clr;
{
  switch(clr){
    case BLACK:   printf("Black");   break;
    case RED:     printf("Red");     break;
    case GREEN:   printf("Green");   break;
    case YELLOW:  printf("Yellow");  break;
    case BLUE:    printf("Blue");    break;
    case MAGENTA: printf("Magenta"); break;
    case CYAN:    printf("Cyan");    break;
    case WHITE:   printf("White");   break;
  }
}

pattrib(att)
int att;
{
  switch(att){
    case RESET:      printf("Reset   "); break;
    case BOLD:       printf("Bold    "); break;
    case DIM:        printf("Dim     "); break;
    case ITALIC:     printf("Italic  "); break;
    case UNDERSCORE: printf("Underln "); break;
    case BLINK:      printf("Blink   "); break;
    case REVERSE:    printf("Rever   "); break;
    case HIDDEN:     printf("Hidden  "); break;
    case UKG0:       printf("UK G0   "); break;
    case UKG1:       printf("UK G1   "); break;
    case USG0:       printf("US G0   "); break;
    case USG1:       printf("US G1   "); break;
    case G0SPEC:     printf("G0 Spec "); break;
    case G1SPEC:     printf("G1 Spec "); break;
    case G0ALT:      printf("G0 Alt  "); break;
    case G1ALT:      printf("G1 Alt  "); break;
    case G0ROM:      printf("G0 ROM  "); break;
    case G1ROM:      printf("G1 ROM  "); break;
    case BGRESET:    printf("BGReset "); break;
  }
}

#endif
