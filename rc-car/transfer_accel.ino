int transfer_accel(int duration, int direction)
{
  const int tol = 5; 

  int forward_pwm = 0;
  int reverse_pwm = 0;
  forward_pwm = map(duration, duration_min, duration_max, -255, 255);
  reverse_pwm = map(duration, duration_min, duration_max, 255, -255);

  // filter noise and negative values
  if(forward_pwm < tol)
  {
    forward_pwm = 0;
  }
  if(reverse_pwm < tol)
  {
    reverse_pwm = 0;
  }

  if(direction)
  {
    return forward_pwm;
  }
  else
  {
    return reverse_pwm;
  }
}