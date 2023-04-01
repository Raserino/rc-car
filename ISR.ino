ISR(TIMER1_COMPA_vect) 
{ 
  // check for all four wheels
  for(int i=0; i<4; i++)
  {
    // get input from hall sensor (active LOW)
    bool sensor = digitalRead(RPM[i])

    // reset semaphor if the magnet is not detected
    if(sensor && !sema[i])
    {
      sema[i] = 1;
    }

    // measure cycle time if the magnet is detected
    if(!sensor && sema[i] )
    {
      long T[i] = micros() - starttime[i];
      starttime[i] = micros();
      sema[i] = 0; // reset semaphor
    }

    // timeout after 1 second
    if(10^6 < micros() - starttime[i]) 
    {
      T[i] = 2147483647; // infinite cycle time gives zero speed
    }
  }
}