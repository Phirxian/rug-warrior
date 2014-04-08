/* January 10,1994  KPU  */
/* Prints text strings to screen with auto word wrap */

char _printstring[62]="                                    ";
char _space[]=" ";

void wrap(char string[])
{
  int i,j;

  if (_array_size(string)<17) printf("%s\n",string);
  else {

    for(i=16;i>0;i--){                   /* Find the nearest space before */
      if (string[i]==_space[0]) break;   /* the 17th character */
    }
    
    for(j=0;j<i;j++) _printstring[j]=string[j];   /* Transfer all characters */
    /* before the space into */
    /* the print string */
    
    for(j=i;j<16;j++) _printstring[j]=_space[0];  /* Pad the rest of the */
    /* first line with spaces */
    
    /* Put in the rest of the string to be printed  */
    for(j=0;j<_array_size(string)-i-1;j++) _printstring[j+16]=string[j+i+1];
    
    /* Put it on the LCD  */
    printf("%s\n",_printstring);
  }
}
