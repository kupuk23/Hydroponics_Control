void exhaustControl(float humid)
{
  if (humid < 50) analogWrite(exhaust_fan, 40);
  else if (humid < 55) analogWrite(exhaust_fan, pwmMapper(35));
  else if (humid < 60) analogWrite(exhaust_fan, pwmMapper(50));
  else if (humid < 65) analogWrite(exhaust_fan, pwmMapper(55));
  else if (humid < 70) analogWrite(exhaust_fan, pwmMapper(65));

}

int pwmMapper(int fan_spd)
{
  return map(fan_spd, 0, 100, 0, 255);
}