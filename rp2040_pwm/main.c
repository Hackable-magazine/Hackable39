#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>

#define VERSION "0.0.1"
#define DESCRIPTION "Démonstration PWM"
#define URL "http://connect.ed-diamond.com/Hackable"
#include "binfo.h"

#define PWMOUT 2
#define PWMIN 5

void printpwminfo(uint gpio)
{
  printf("GPIO %d: slice/chan %d%c\n", gpio, pwm_gpio_to_slice_num(gpio),
         pwm_gpio_to_channel(gpio) == PWM_CHAN_A ? 'A' : 'B');
}

void initcompte(uint gpio)
{
  // Vérif si on est sur canal B
  assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
  // Récup slice
  uint slice = pwm_gpio_to_slice_num(gpio);
  // Config PWM
  pwm_config cfg = pwm_get_default_config();
  // On compte les fronts montant
  pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
  // Diviseur, divise les événement par 200
  pwm_config_set_clkdiv(&cfg, 200.0);
  // Applique config sans auto-start
  pwm_init(slice, &cfg, false);
  // Choix fonction
  gpio_set_function(gpio, GPIO_FUNC_PWM);
}

uint16_t compte(uint gpio)
{
  uint slice = pwm_gpio_to_slice_num(gpio);

  /// Compteur à 0
  pwm_set_counter(slice, 0);
  sleep_ms(1);

  // Active PWM
  pwm_set_enabled(slice, true);
  // Pause 100ms = 1/10 sec
  sleep_ms(100);
  // Désactive PWM
  pwm_set_enabled(slice, false);

  return pwm_get_counter(slice);
}

int main()
{
  bi_decl(bi_program_description(BI_DESCRIPTION));
  bi_decl(bi_program_version_string(BI_VERSION));
  bi_decl(bi_program_url(BI_URL));
  bi_decl(bi_2pins_with_func(PWMOUT, PWMIN, GPIO_FUNC_PWM));
  bi_decl(bi_2pins_with_names(PWMOUT, "Led jaune", PWMIN, "PWM in"));

  // Initialisation I/O
  stdio_init_all();

  // Fonction des GPIO
  gpio_set_function(PWMOUT, GPIO_FUNC_PWM);

  // NON ! GPIO en PWM = out par défaut -> signal in sur pgio out = mal
  // gpio_set_function(PWMIN, GPIO_FUNC_PWM);

  // pwm_set_gpio_level(PWMOUT, (0xffff / 2));

  // compteur wrap sur 10 cycles (0..9)
  pwm_set_wrap(pwm_gpio_to_slice_num(PWMOUT), 9);
  // out haut pour 5 cycles
  pwm_set_chan_level(pwm_gpio_to_slice_num(PWMOUT), pwm_gpio_to_channel(PWMOUT),
                     5);
  // Réglage diviseur
  pwm_set_clkdiv(pwm_gpio_to_slice_num(PWMOUT), 125.0);
  // pwm_set_clkdiv_int_frac(pwm_gpio_to_slice_num(PWMOUT), 12, 8);

  // Zou
  pwm_set_enabled(pwm_gpio_to_slice_num(PWMOUT), true);

  printf("\033[2J\033[H");
  printpwminfo(PWMOUT);
  printpwminfo(PWMIN);

  initcompte(PWMIN);

  while (true) {
    for (int i = 0; i < 10; i++) {
      //pwm_set_clkdiv(pwm_gpio_to_slice_num(PWMOUT), 125.0 + (i * 10));
      pwm_set_wrap(pwm_gpio_to_slice_num(PWMOUT), 9+i);
      printf("Compte: %lu kHz / %lu kHz\n", compte(PWMIN) * 2,
             clock_get_hz(clk_sys) / 1000);
      sleep_ms(2000);
    }
  }
}