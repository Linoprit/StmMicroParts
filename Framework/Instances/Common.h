/*
 * Common.h
 *
 *  Created on: 20.05.2019
 *      Author: harald
 */

#ifndef INSTANCES_COMMON_H_
#define INSTANCES_COMMON_H_

#include <chrono>
#include <unistd.h>

class Common
{
public:

  Common() {};
  virtual ~Common() {};
  bool is_init_done(void);

  static void     init(void);
  static uint32_t get_tick(void);

  static inline void delay(uint32_t delay) { usleep(1000 * delay); }

/*
  // sockets
  static uart_socket	*display_comm;

  // public Devices
  static nextion	  	*nex_disp;
  static Rtc_device 	*rtc;

  // application Classes
  static Heiz_display 	*heiz_disp;
  static State_Machine	*machine;*/

  // testing interface
  static void set_tick_faking(bool val)   { isFakeTick =  val; };
  static void set_fakeTick (uint32_t val) { fakeTick   =  val; };
  static void add_fakeTick (uint32_t val) { fakeTick   += val; };

private:
  static bool 	initIsDone;
  static bool	isFakeTick;
  static uint32_t fakeTick;

  static std::chrono::milliseconds startTime;


};
#endif /* INSTANCES_COMMON_H_ */
