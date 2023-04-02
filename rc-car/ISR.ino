ISR(TIMER1_COMPA_vect) 
{ 
  long time = millis();
  // check for all four wheels
  for(int i=0; i<4; i++)
  {
    // get input from hall sensor (active LOW)
    bool sensor = digitalRead(RPM[i]);

    // reset semaphor if no magnet and 10ms have passed
    if(sensor && time - starttime[i] > 10)
    {
      sema[i] = 1;
    }

    // measure cycle time if the magnet is detected
    if(!sensor && sema[i])
    {
      T_rpm[i] = time - starttime[i];
      starttime[i] = time;
      sema[i] = 0; // reset semaphor
    }

    // timeout after 1 second
    if(time - starttime[i] > 1000) 
    {
      T_rpm[i] = 2147483647; // infinite cycle time gives zero speed
      starttime[i] = time;
    }
  }
}