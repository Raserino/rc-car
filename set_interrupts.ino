set_interrupts()
{
  int prescaler = 1;
  int interrupt_frequency = 10^4;

  cli();

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0

  // set compare match register for 1hz increments
  OCR1A = (16*10^6)/(prescaler * frequency) - 1 (must be <65536)

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1 prescaler
  TCCR1B |= (1 << CS10);  

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
}
