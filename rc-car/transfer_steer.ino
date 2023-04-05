int transfer_steer(int duration)
{
  const int tol =23;
  const int pos_0 = 90;
  const int range = 40; //+- range

  int control = map(duration, duration_min, duration_max, pos_0 - range, pos_0 + range);
  // filter noise
  if (abs(control - pos_0) < tol)
  {
    control = 0;
  }
  return control;
}