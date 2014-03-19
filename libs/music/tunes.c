/**
 * basic song
 */
char pp_song[] = "1#d 4e3r1#f4g3r1#d 3e1#f3g1c3bD1e3g1b 8&b2b2a2g2e2d10e 7r1#d 4e3r1#f4g3r1#d 3e1#f3g1c3b1g3b1e 28&e D3r1#d 4e3r1#f4g3r1#d 3e1#f3g1c3bD1e3g1b 8&b2b2a2g2e2d10e 12r U3e1d3b1a3g1#f 1&b3a1&b3a1&b3a1&b3a 2g2e2d2e18e 8r 2g2e2d2e18e 8r 2g2e2d2e18e";

/**
 * basic song
 */
char w_song[] = "1c2f1f2f1g2a1a2a1f2g1g2g1e4f1r 1f2a1a2a1&b2c1c2c1c2d1c2&b1a4g1r 1gU2d1d2d1c2d1e2f1d2c1d2c1&b4a1r D1c2f1f2f1g2a1a2a1f2g1g2f1e5f";

/**
 * basic song
 */
char classics_song[] = "4g2c2d4c4g4a2g2f2e2c2d2e2f2g2a2b10c2r 4c4b4c2d2b2c2d2e2#f4g2#f2e4d2g2#f4g4d4e2d2c2b2g2a2b2c2b2c2d2e2d2e2#f12g";

/**
 * basic song
 */
char charge_song[] = "1c1e1g2c1r1g4c";

/**
 * basic song
 */
char benny_song[] = "1c1d1cU2f2f1d1c1#g1a2c2d1c1a1#g1g2f1f1f1a1c2d5c1c1d1c2f2f1d1c1#g1a2c2d1c1a1#g1g2c1c1c1e1g2e5c1c1d1c2f2f2f2f2f2f1d1c1g1a2#a2#a2#a2#a1d1#a1d1#aU1f3g1a1#g1a1#g1a3c1a1#g1#a5f1a1#f1#d1cU1#g1f1d1b4f";

/**
 * basic song
 */
char mb_song1[] = "2c2e2g2eU2c2g2e2gD2c2e2g2eU2c2g2e 2c2c2g2c2e2c2e2gD2cU2c2b2a7g1r 2g2g2f2d2b2g2b2d2f2e2cU2a7g1r D2c2c2g2c2e2c2e2gD2cU2c2b2a7g1r 2g2g2f2d2b2g2b2d2b2c2gU2e4c 2c2e2g2c2a2f2c2a2c2f2a2gD2cU2a4g D2g2b2d2g2f2d2b2g2b2d2f2e2cU2a4g D2c2e2g2c2a2f2c2a2c2f2a2gD2cU2a4g D2g2b2d2g2f2d2b2g2b2d2b2c2gU2e7c";

/**
 * basic song
 */
char mb_song2[] = "2c2e2g2eU2c2g2e2gD2c2e2g2eU2c2g2e 2c2c2g2c2e2c2e2gD2cU2c2b2a7g1r U2g2g2f2d2b2g2b2d2f2e2cU2a7g1r DD2c2c2g2c2e2c2e2gD2cU2c2b2a7g1r U2g2g2f2d2b2g2b2d2b2c2gU2e4c 2c2e2g2c2a2f2c2a2c2f2a2gD2cU2a4g D2g2b2d2g2f2d2b2g2b2d2f2e2cU2a4g DD2c2e2g2c2a2f2c2a2c2f2a2gD2cU2a4g U2g2b2d2g2f2d2b2g2b2d2b2c2gU2e7c";

/**
 * basic song
 */
char looney_tune_song[]= "U3e1d2c2d2e2d2e2c2d2d2d6d2r 3d1c2b2c2d2#c2d2b2c2c2c6c";

/**
 * tempo for play(char song[]) function
 */
int tempo = 12;

/**
 * play(char[]) global timer variable
 */
long time, newtime;

/**
 * play(char[]) global current command variable
 */
int music_current_command= 'o';

/**
 * play(char[]) global next command variable
 */
int music_next_command= 0;

/**
 * play a music with 8bit array @song
 */
void play(char song[])
{
    int i, duration, accidental, delta, note, rest;
    int notes[]= {0,2,3,5,7,8,10};
    int old_note= 30;
    play_reset();
    i= 0;

    while(song[i])
    {
        while(1)
        {
            while(song[i] == ' ') ++i;

            if(song[i] == 'X')
            {
                ++i;
                music_next_command= song[i];
                ++i;
            }
            else break;
        }

        if(!song[i]) break;

        while(song[i] == 'D')
        {
            old_note -= 12;
            ++i;
        }

        while(song[i] == 'U')
        {
            old_note += 12;
            ++i;
        }

        if('0' <= song[i] && song[i] <= '9')
        {
            duration= 0;

            while('0' <= song[i] && song[i] <= '9')
            {
                duration = duration * 10 + song[i]-'0';
                ++i;
            }
        }

        if(song[i] == '#')
        {
            accidental= 1;
            ++i;
        }
        else if(song[i] == '&')
        {
            accidental= -1;
            ++i;
        }
        else
        {
            accidental= 0;
        }

        if(song[i] == 'r') rest= 1;
        else
        {
            if(song[i] < 'a' || song[i] > 'g')
            {
                printf("\nBad note:%c\n", song[i]);
                beep();
                beep();
                beep();
                sleep(5.0);
                return;
            }

            note= notes[song[i]-'a'] + accidental;
            rest= 0;
        }

        i++;

        if(rest)
        {
            play_note(0, duration);
        }
        else
        {
            delta= note - (old_note % 12);
            old_note += delta;

            if(delta > 5) old_note -= 12;

            if(delta < -5) old_note += 12;

            play_note(old_note, duration);
        }
    }

    play_note(0, 1);
    music_command('o');
}

/**
 * set the music command @see play(char[])
 */
void music_command(int c)
{
    music_current_command = c;
}

/**
 * reset timer of play(char[])
 */
void play_reset()
{
    time = mseconds();
    newtime = time+100L;
}

/**
 * play_note_2(note, duration*7);
 * play_note_2(0, duration);
 */
void play_note(int note, int duration)
{
    play_note_2(note, duration*7);
    play_note_2(0, duration);
}

/**
 * enable/disable beeper
 * and play appropriate note with given duration
 */
void play_note_2(int note, int duration)
{
    float freq;
    int   period;

    if(note)
    {
        freq= 55.0 * (2. ^ (((float) note) / 12.));
    }

    while(mseconds() < newtime);

    if(note)
    {
        set_beeper_pitch(freq);
        beeper_on();
    }
    else
    {
        beeper_off();
    }

    if(music_next_command &&
            music_current_command != music_next_command)
    {
        music_command(music_next_command);
        music_next_command= 0;
    }

    newtime += (long)(duration * tempo);
}

/**
 * Play a basic music for test
 */
void bicycle()
{
    tone(2093.0,0.769);
    tone(1760.0,0.769);
    tone(1396.9,0.769);
    tone(1046.5,0.769);
    tone(1174.7,0.192);
    tone(1318.5,0.192);
    tone(1396.9,0.192);
    tone(1174.7,0.385);
    tone(1396.9,0.192);
    tone(1046.5,0.962);
    sleep(0.385);
    tone(1568.0,0.769);
    tone(2093.0,0.769);
    tone(1760.0,0.769);
    tone(1396.9,0.769);
    tone(1174.7,0.192);
    tone(1318.5,0.192);
    tone(1396.9,0.192);
    tone(1568.0,0.385);
    tone(1760.0,0.192);
    tone(1568.0,0.962);
    sleep(0.385);
    tone(1760.0,0.192);
    tone(1864.7,0.192);
    tone(1760.0,0.192);
    tone(1568.0,0.192);
    tone(2093.0,0.385);
    tone(1760.0,0.192);
    tone(1568.0,0.192);
    tone(1396.9,0.962);
    tone(1568.0,0.192);
    tone(1760.0,0.385);
    tone(1396.9,0.192);
    tone(1174.7,0.385);
    tone(1396.9,0.192);
    tone(1174.7,0.192);
    tone(1046.5,0.962);
    tone(1046.5,0.192);
    tone(1396.9,0.385);
    tone(1760.0,0.192);
    tone(1568.0,0.385);
    tone(1046.5,0.192);
    tone(1396.9,0.385);
    tone(1760.0,0.192);
    tone(1568.0,0.192);
    tone(1760.0,0.192);
    tone(1864.7,0.192);
    tone(2093.0,0.192);
    tone(1760.0,0.192);
    tone(1396.9,0.192);
    tone(1568.0,0.385);
    tone(1046.5,0.192);
    tone(1396.9,0.962);
}
