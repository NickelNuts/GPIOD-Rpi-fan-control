/*

  Turns on fan at GPIO pin when temperature exceeds  SP  

*/

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  const char path[] = "/dev/gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *lineFan;    // logic switch for fan
  FILE *temperatureFile;
  double T;
  double SP;
  int ret, val, pin, tmr;

pin=atoi(argv[1]);

  // Open GPIO chip
  chip = gpiod_chip_open(path);
  if (!chip) {
    perror("Open chip failed\n");
    return 1;
  }

  // Open GPIO lines
  lineFan = gpiod_chip_get_line(chip, pin);
  if (!lineFan) {
    perror("Get line failed\n");
    return 1;
  }


  /* config as output and set a description */
	gpiod_line_request_output(lineFan, "gpio-fan",
		GPIOD_LINE_ACTIVE_STATE_HIGH);
  if (ret < 0) {
    perror("Request line as output failed\n");
    return 1;
  }

tmr=atoi(argv[2]);
SP=atof(argv[3]);

  // Turn on fan when temperature exceeds threshold
  while (true) {
  sleep(tmr);
  temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
  fscanf (temperatureFile, "%lf", &T);
  T /= 1000;
  fclose (temperatureFile);
  if (T < SP){
	gpiod_line_set_value(lineFan,0);
	}else{
	gpiod_line_set_value(lineFan,1);
	}
  }

  // Release lines and chip
  gpiod_line_release(lineFan);
  gpiod_chip_close(chip);
  return 0;
}
