void main()
{ sleep(0.75);
  test_number++;
  if ((test_number > 8) | (test_number < 0))
    test_number = 0;
  while (test_number == 0)
    /*        0123456789ABCDEF0123456789ABCDEF */
    { printf("Rug Warrior Pro Behavior demos\n");
      sleep(1.5);
      printf("Press RESET to  select demo\n");
      sleep(1.5);
    }
  if      (test_number == 1)
    start_bugle();
  else if (test_number == 2)
    start_theremin();
  else if (test_number == 3)
    start_yoyo();
  else if (test_number == 4)
    start_wimp();
  else if (test_number == 5)
    start_follow();
  else if (test_number == 6)
    start_moth(1);
  else if (test_number == 7)
    start_echo();
  else if (test_number == 8)
    start_sonic();
}