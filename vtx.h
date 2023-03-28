
/* based on any attributes set in 'str1' update 'str2' */
int fixCase(str1,str2)
char *str1;
char *str2;
{
  int c;
  
  /* convert string to upper case ? - not needed as CPM does this */
  if (index(str1,"+UP") != -1)  /* UU UUU UUUUU UU UUUU */
  {
    for (c=0; c<strlen(str2); c++)
    {
      str2[c] = toupper(str2[c]);
    }
    return(1);
  }

  /* convert string to lower case ? */
  if (index(str1,"+LO") != -1)  /* lll ll lllll ll llll */
  {
    for (c=0; c<strlen(str2); c++)
    {
      str2[c] = tolower(str2[c]);
    }
    return(1);
  }

  /* convert string to Sentence */
  if (index(str1,"+SE") != -1)  /* "Ulll lll lllll ll lll" */
  {
    str2[0] = toupper(str2[0]);
    for (c=1; c<strlen(str2); c++)
    {
      str2[c] = tolower(str2[c]);
    }
    return(1);
  }

  /* convert string to each word is capitalized */
  if (index(str1,"+CA") != -1)  /* "Ulll Ull Ullll Ul Ull" */
  {
    str2[0] = toupper(str2[0]);
    for (c=1; c<strlen(str2); c++)
    { 
      if (str2[c-1] == ' ') /* Prev chr==<space> then upper case */
      {
        str2[c] = toupper(str2[c]);
      }
      else                       /* else make lower case */
      {
        str2[c] = tolower(str2[c]);
      }
    }
    return(1);
  }
  return(0);
}

/* Does the file defined on the command line exist and can
 * it be opened?
 */
int testFile(fname)
char *fname;
{
  FILE *infile;
  /* do a test file open to make sure it exists */
  infile = fopen(fname,"r");  /* open for read */
  if (!infile)
  {
    printf("Error:'%s' not found or unable to open\n",fname);
    return(0);  
  }
  fclose(infile);
  return(1);  
}

/* Process the commands from the command line */
doCmdLine(argc,argv)
int argc;
char **argv;
{
  /* modify strings inplace as needed */
  for (i=1; i<argc; i++)  /* loop over arguments */
  {
    i = i + fixCase(argv[i],argv[i+1]);
  }
  /* run the commands and do the stuff */
  for (i=1; i<argc; i++)  /* loop over arguments */
  {
    i = i + runCommands(argv[i],argv[i+1]);
    if (index(argv[i],"LOO") == 0) { i = 0; }  /* back to step 1 */
  }
}

/* Process the commands from a file
 * see vtxhelp.txt for details on the file contents and layout
 */
doFile(argc,argv)
int argc;
char **argv;
{
  int p; /* used to index over strings */
  
  FILE *infile;
  char  str1[BLENGTH+1]; /* need room for a 0x00 at the end */
  char *str2;            /* used in parsing the line */
  
  color = -1;   /* used in setting colors   */
  brite =  0;   /*  0 = bright not requested */
  fgclr = -2;   /* -2 = mark foreground color as unknown */
  bgclr = -2;   /* -2 = mark background color as unknown */
  loop  =  0;   /* do we loop over the file? */

  infile = fopen(argv[2],"r");  /* open for read */
  do
  {
    fgets(str1,BLENGTH,infile);
    if ((str1[0] != '#') && (strlen(str1) > 2)) /* # = comment */
    {
      str2 = NULL;

      for (p=0; p<strlen(str1); p++) /* split on first space */
      {
        if (str1[p] == ' ')
        {
          str1[p] = 0;       /* terminate command string */
          str2 = &str1[p+1]; /* point at start of parameters */
          p = BLENGTH+1;     /* exit for (p=0;.... loop ) */
        }
      }
      for (p=0; p<strlen(str1); p++) /* cmds are capitalized */
      {
        str1[p]=toupper(str1[p]);    /* force all uppercase */
      }
      /* if not ECHO or BANNER commands */
      if (!((index(str1,"ECH") == 0) || (index(str1,"BAN") == 0)))
      {
        for (p=0; p<strlen(str2); p++) /* cmds are capitalized */
        {
          str2[p]=toupper(str2[p]);    /* force all uppercase */
        }
      } /* of if not ECHO or BANNER command */
      else  
      { /* it is an ECHO or BANNER command */
        str2[strlen(str2)-1] = 0;      /* remove CR from string */
      }
      fixCase(str1,str2); /* Deals with how CP/M CAPs arguments */
      runCommands(str1,str2);
      if (index(str1,"LOO") != -1) { loop = 1; } /* set loop flag */
    }
  }
  while (!feof(infile));  
  fclose(infile);  
}

