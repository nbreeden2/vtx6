/* Used to send various <esc> sequences to a VTxxx terminal
 * Neil Breeden - V0.6.03.28.2023 - March 28, 2023
 * Build with the BDSC C compiler
 * Requires vt100.h which defines the <esc> strings and functions
 * See vtxhelp.txt for documentation 
 * https://thehighnibble.com/vt132/
 */

#include <stdio.h>
#include <vt100.h>

#define BLENGTH 128 /* length of the file read buffer */

/* 1 to n attributes are processed, CP/M capitalizes them when
 *   passing them to the program.
 * When processing commands from a file the commands are capitalized
 *   while being processed.
 */

/* Yep - they are global - so sue me */
int color; /* color requested BLACK..WHITE       */
int brite; /* bright color requested ?           */
int fgclr; /* was a foreground color requested ? */
int bgclr; /* was a background color requested ? */
int i;
int loop;  /* flag - if true we loop the commands indefinitely */

/* Due to the globals above and the lack of an EXTERN directive
 * we include vtx.h here so they are in scope. These functions
 * have been broken out as all source in a single file was taking
 * too long to load on the IMSAI in the TE editor.
 */
#include <vtx.h>

/* love me some old school K&R syntax */
main(argc,argv)
int argc;
char **argv;
{
  /* initialize global variables */ 
  color = -1;   /* used in setting colors   */
  brite =  0;   /* 0 = bright not requested */
  fgclr = -2;   /* -2 foreground color is unknown */
  bgclr = -2;   /* -2 background color is unknown */
  loop  =  0;   /* in FILE mode do we loop over the file? */

  if (argc == 1)  /* no attributes defined  */
  {
    printf("Please review vtxhelp.txt for usage details\n");
    exit();
  }

  if (index(argv[1],"FIL") == 0)  /* process commands from a file */
  {    
    if (testFile(argv[2]) == 0)
    {
      exit();   /* file wasn't found or able to be opened */
    }
    do /* process the commands in the file */
    {
      doFile(argc,argv);
    } while (loop); /* if loop is set do it again */
  }
  else  /* process command line arguments */
  {
    doCmdLine(argc,argv);
  }
  exit();
}

/* Process any commands in str1                     */
/* str2 may be used as part of str1     IE: bg blue */
/* returns: 0 = Only used str1          IE: bold    */
/*          1 = Used both str1 and str2 IE: fg red  */
int runCommands(str1,str2)
char *str1;
char *str2;
{
  int z;
  int x1;
  int y1;

  color = -1;  /* each pass we reset the requested color */
  brite =  0;  /* each pass we reset bright mode         */
  fgclr = -2;
  bgclr = -2;
  x1 = 0;      /* used in positionCursor() */
  y1 = 0;      /* used in positionCursor() */

  /* loop mode, reset i back to 1 so the commands will run again */
  if (index(str1,"LOO")  == 0) { i = 1;                   return(0); }

  /* pause mode, wait for keypress */
  if (index(str1,"PAU")  == 0) { getchar();               return(0); }

  /* sleep mode */
  if (index(str1,"SLE")  == 0) { sleep(atoi(str2));       return(1); }

  /* print a crlf ? */
  if (index(str1,"CRLF") == 0) { printf("\n");            return(0); }

  /* print a cr ? */
  if (index(str1,"DOCR") == 0) { printf("%c",13);         return(0); }

  /* print a lf ? */
  if (index(str1,"DOLF") == 0) { printf("%c",10);         return(0); }

  /* print a ff ? */
  if (index(str1,"DOFF") == 0) { printf("%c",12);         return(0); }

  /* print a character ? */
  if (index(str1,"DOXX") == 0) 
  { 
    printf("%c",atoi(str2));
    return(1);
  }

  /* position cursor ? */
  if (index(str1,"SETP") == 0)
  {
    x1=-1;
    y1=-1;
    if (sscanf(str2,"%d,%d",&x1,&y1) == 2)
    {
      if ((x1 > -1) && (y1 > -1))
      {
        positionCursor(x1,y1);
        return(1);
      }
    }
    return(0);
  }

  /* double width single height */
  if (index(str1,"DWS") == 0)  { sAttrib(DWSH);           return(0); }
    
  /* single width single height */
  if (index(str1,"SWS") == 0)  { sAttrib(SWSH);           return(0); }
    
  /* double height top */
  if (index(str1,"DHT") == 0)  { sAttrib(DHTOP);          return(0); } 
  /* double height bottom */
  if (index(str1,"DHB") == 0)  { sAttrib(DHBOT);          return(0); }
  
  /* 80 column ? */
  if (index(str1,"80") == 0)   { sAttrib(COL80);          return(0); }

  /* 132 column ? */
  if (index(str1,"13") == 0)   { sAttrib(COL132);         return(0); }

  /* Background color reset ? */
  if (index(str1,"BGR") == 0)  { sAttrib(BGRESET);        return(0); }

  /* dim text ? */
  if (index(str1,"DIM") == 0)  { sAttrib(DIM);            return(0); }

  /* blink text ? */
  if (index(str1,"BLI") == 0)  { sAttrib(BLINK);          return(0); }

  /* clear screen ? */
  if (index(str1,"CLS") == 0)  { cls();                   return(0); }

  /* reset attributes ? */
  if (index(str1,"RES") == 0)  { sAttrib(RESET);          return(0); }

  /* init terminal ? */
  if (index(str1,"INI") == 0)  
  { 
    sAttrib(RESET);    /* all attributes including color */ 
    sAttrib(COL80);    /* back to 80 column mode         */
    cls(); return(0);  /* clear the screen               */
  }

  /* bold ? */
  if (index(str1,"BOL") == 0)  { sAttrib(BOLD);           return(0); }

  /* italic ? */
  if (index(str1,"ITA") == 0)  { sAttrib(ITALIC);         return(0); }

  /* reverse ? */
  if (index(str1,"REV") == 0)  { sAttrib(REVERSE);        return(0); }

  /* underscore ? */
  if (index(str1,"UND") == 0)  { sAttrib(UNDERSCORE);     return(0); }

/*
 * Keyboard LED controls
 * https://thehighnibble.com/vt132/operation/vt100/programmer/#background
 */
  /* LEDs Off */
  if (index(str1,"LEDO") == 0) { sAttrib(LEDOFF);         return(0); }

  /* LED 1 On <Num lock>*/
  if (index(str1,"LED1") == 0) { sAttrib(LED1ON);         return(0); }

  /* LED 2 On <Caps lock>*/
  if (index(str1,"LED2") == 0) { sAttrib(LED2ON);         return(0); }

  /* LED 3 On <Scroll lock>*/
  if (index(str1,"LED3") == 0) { sAttrib(LED3ON);         return(0); }

  /* LED 4 On - not suported on VT132 and S-132 */
  if (index(str1,"LED4") == 0) { sAttrib(LED4ON);         return(0); }

/*
 * Character Set controls
 * https://thehighnibble.com/vt132/#vt100-terminal
 */
  /* Character Sets */
  if (index(str1,"UKG0") == 0) { sAttrib(UKG0);           return(0); }
  if (index(str1,"UKG1") == 0) { sAttrib(UKG1);           return(0); }
  if (index(str1,"USG0") == 0) { sAttrib(USG0);           return(0); }
  if (index(str1,"USG1") == 0) { sAttrib(USG1);           return(0); }
  if (index(str1,"G0SP") == 0) { sAttrib(G0SPEC);         return(0); }
  if (index(str1,"G1SP") == 0) { sAttrib(G1SPEC);         return(0); }
  if (index(str1,"G0AL") == 0) { sAttrib(G0ALT);          return(0); }
  if (index(str1,"G1AL") == 0) { sAttrib(G1ALT);          return(0); }
  if (index(str1,"G0RO") == 0) { sAttrib(G0ROM);          return(0); }
  if (index(str1,"G1RO") == 0) { sAttrib(G1ROM);          return(0); }

  /* dump the current char set ? */
  if (index(str1,"CSE") == 0)  /* CSET */
  {
    printf("\n");
    for (z=33; z<127; ++z)
    {
      printf("%c",z);
    }
    printf("\n");
    return(0); 
  }

  /* dump the current extended char set ? */
  if (index(str1,"CEX") == 0)  /* CEXTENDED */
  {
    printf("\n");
    for (z=33; z<127; ++z)
    {
      printf("%c",z);
    }
    for (z=128; z<256; ++z)
    {
      printf("%c",z);
    }
    printf("\n");
    return(0); 
  }

  /* dump the current char set ? */
  if (index(str1,"CSE") == 0)  /* CSET */
  {
    printf("\n");
    for (z=33; z<127; ++z)
    {
      printf("%c",z);
    }
    printf("\n");
    return(0); 
  }

  /* print a string ? */
  if (index(str1,"ECH") == 0)
  {
    printf("%s",str2);
    return(1);
  }

  /* print a double height banner */
  if (index(str1,"BAN") == 0)
  {
    sAttrib(DHTOP);
    printf("%s\n",str2);
    sAttrib(DHBOT);
    printf("%s\n",str2);
    sAttrib(SWSH);
    return(1);
  }

  /* color stuff ? */
  if (index(str1,"FG") == 0) { fgclr = -1;   }
  if (index(str1,"BG") == 0) { bgclr = -1;   }
  if (index(str2,"+") != -1) { brite=BRIGHT; }

  /* asking for a specific color ? */
  if (index(str2,"BLA") == 0)      { color=BLACK;   }
  else if (index(str2,"RED") == 0) { color=RED;     }
  else if (index(str2,"GRE") == 0) { color=GREEN;   }
  else if (index(str2,"YEL") == 0) { color=YELLOW;  }
  else if (index(str2,"BLU") == 0) { color=BLUE;    }
  else if (index(str2,"MAG") == 0) { color=MAGENTA; }
  else if (index(str2,"CYA") == 0) { color=CYAN;    } 
  else if (index(str2,"WHI") == 0) { color=WHITE;   }   
  if (color > -1)
  {
    if (fgclr == -1) /* set the FG color */
    {
      fgclr = color + brite + FG;
      sColor(fgclr);
    }
    if (bgclr == -1) /* set the BG color  */
    {
      bgclr = color + brite + BG;
      if (bgclr != fgclr)  /* don't set FG & BG to the same color */
      {
        sColor(bgclr);
        /* cls(); */          /* fill background with color */
      }
    }
    return(1); 
  }
  /* if we get to here the command wasn't recognized */
  if (strlen(str1) > 2)
  {
    printf("\n\nCommand unknown:%s\n\n",str1);
  }
  return(0);
}
