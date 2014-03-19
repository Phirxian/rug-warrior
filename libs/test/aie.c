/**
 * play all playable frequency
 */
void main()
{
    int i = 1;

    for(; i<100; ++i)
        note((float)i*440.,1.);
}

