int transfer_steer(int duration)
{
  const int tol = 5;

  int control = map(duration, duration_min, duration_max, 0, 255);
  // filter noise
  if abs(control < tol)
  {
    control = 0;
  }
  return control;
}