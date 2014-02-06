/*******************************menu.c*********************************/
/* Anne Wright:  Menu functions (which also allow variables to be set
   via the frob knob and selection buttons) for robot. 11/14/91 */

/* abstractions for chosen_button */
#define NEITHER_B 0
#define CHOOSE_B  1
#define ESCAPE_B  2

/* abstractions for wait_button */
#define UP_B      3
#define DOWN_B    4
#define CYCLE_B   5

/*****************************button routines*************************/
int chosen_button()
{
    /* returns which button is depressed.  If both, choose has precedence */
    if(choose_button())
        return CHOOSE_B;
    else if(escape_button())
        return ESCAPE_B;
    else
        return NEITHER_B;
}

/*
 * wait until button is depressed(DOWN_B), released(UP_B), or both(CYCLE_B) and
 * return which button if any activated the sequence
 */
int wait_button(int i)
{
    if(i==DOWN_B)
    {
        while(!(choose_button() || escape_button()));

        return chosen_button();
    }
    else if(i==UP_B)
    {
        int b;
        b=chosen_button();

        while(choose_button() || escape_button());

        return b;
    }
    else
    {
        int b;

        while(!(choose_button() || escape_button()));

        b=chosen_button();

        while(choose_button() || escape_button());

        return b;
    }
}

/**********************Menu selection routines*****************************/
/*
 * returns an integer value between min_val and max_val chosen using frob
 */
int select_int_value(char s[],int min_val,int max_val)
{
    int val, coarseness=(255)/(max_val-min_val),selection;
    printf("%s%d to %d\n",s,min_val, max_val);
    msleep(500L);
    wait_button(UP_B);

    while(!(choose_button() || escape_button()))
    {
        val=min((frob_knob())/coarseness+min_val,max_val);
        printf("%s %d\n",s,val);
        msleep(100L);
    }

    if(wait_button(UP_B)==ESCAPE_B)
        return -1; /** -1 means escape -- do not reset value **/
    else
        return min(val,max_val);
}

float select_float_value(char s[],float min_val,float max_val)
{
    float val, coarseness=(255.)/(max_val-min_val),selection;
    printf("%s %f to %f\n",s,min_val,max_val);
    msleep(500L);
    wait_button(UP_B);

    while(!(choose_button() || escape_button()))
    {
        val=fmin(((float)frob_knob())/coarseness+min_val,max_val);
        printf("%s %f\n",s,val);
        msleep(100L);
    }

    if(wait_button(UP_B)==ESCAPE_B)
        return -1.0; /** -1 means escape -- do not reset value **/
    else
        return fmin(val,max_val);
}

/*
 * menu selects a line of a string from a composite string with n lines
 */
int select_string(char choices[][],int n)
{
    int selection,last_selection=-1,coarseness;

    if(n>_array_size(choices))
        n=_array_size(choices);

    coarseness=255/(n-1);
    printf("Selecting a string...\n");
    msleep(500L);
    wait_button(UP_B);

    while(!(choose_button() || escape_button()))
    {
        selection=(frob_knob())/coarseness;

        if(selection!=last_selection)
        {
            printf("%s\n",choices[selection]);
            msleep(150L);
            last_selection=selection;
        }
    }

    if(wait_button(UP_B)==ESCAPE_B)
        return -1;
    else
        return min(selection,n-1);

    return selection;
}
