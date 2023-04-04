int launch(int* rpm)
{
  /*
  P-controller which maintains a constant ideal slip (around 12%).
  This means the controller controls rear axle should have
  (1+0.12)-times the speed of the front axle.
  */
  const float s = 0.12; // slip
  const float K = 0.2; // P-controller gain

  int rpm_front = 0.5*(rpm[0] + rpm[1]);
  int rpm_rear = 0.5*(rpm[2] + rpm[3]);

  if(rpm_front < 300) // at the start (1+s)*rpm_front would be 0
  {
    int control = 100; // empiric ideal start value
    return control;
  }
  else // we do the actual control
  {
    int rpm_desired = rpm_front * (1+s);
    int error = rpm_desired - rpm_rear;
    int control = K*error;
    return control;
  }
}