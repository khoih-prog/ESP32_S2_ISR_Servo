/****************************************************************************************************************************
  ESP32_S2_FastTimerInterrupt.hpp
  For ESP32_S2 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_S2_ISR_Servo
  Licensed under MIT license

  Now with these new 16 ISR-based timers, the maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Loosely based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.2.0

  Version Modified By  Date        Comments
  ------- -----------  ----------  -----------
  1.1.0   K Hoang      03/08/2021  Initial coding for ESP32_S2
  1.1.1   K Hoang      16/06/2022  Add support to new Adafruit board QTPY_ESP32S2
  1.2.0   K Hoang      17/11/2022  Update to use with ESP32 core v2.0.5+. Convert to h-only style and allman astyle
 *****************************************************************************************************************************/

#pragma once

#ifndef ESP32_S2_FastTimerInterrupt_hpp
#define ESP32_S2_FastTimerInterrupt_hpp

////////////////////////////////////////

#include <driver/timer.h>

/*
  //ESP32 core v1.0.6, hw_timer_t defined in esp32/tools/sdk/include/driver/driver/timer.h:

  #define TIMER_BASE_CLK   (APB_CLK_FREQ)  //Frequency of the clock on the input of the timer groups 


 //@brief Selects a Timer-Group out of 2 available groups
 
typedef enum
{
  TIMER_GROUP_0 = 0, // Hw timer group 0
  TIMER_GROUP_1 = 1, // Hw timer group 1
  TIMER_GROUP_MAX,
} timer_group_t;


 //@brief Select a hardware timer from timer groups
 
typedef enum 
{
  TIMER_0 = 0, // Select timer0 of GROUPx
  TIMER_1 = 1, // Select timer1 of GROUPx
  TIMER_MAX,
} timer_idx_t;


 //@brief Decides the direction of counter
 
typedef enum 
{
  TIMER_COUNT_DOWN = 0, //Descending Count from cnt.high|cnt.low
  TIMER_COUNT_UP = 1,   //Ascending Count from Zero
  TIMER_COUNT_MAX
} timer_count_dir_t;


 //@brief Decides whether timer is on or paused
 
typedef enum 
{
  TIMER_PAUSE = 0,      //Pause timer counter
  TIMER_START = 1,      //Start timer counter
} timer_start_t;


 //@brief Decides whether to enable alarm mode
 
typedef enum 
{
  TIMER_ALARM_DIS = 0,  //Disable timer alarm
  TIMER_ALARM_EN = 1,   //Enable timer alarm
  TIMER_ALARM_MAX
} timer_alarm_t;


 //@brief Select interrupt type if running in alarm mode.
 
typedef enum 
{
  TIMER_INTR_LEVEL = 0,  //Interrupt mode: level mode
  //TIMER_INTR_EDGE = 1, //Interrupt mode: edge mode, Not supported Now
  TIMER_INTR_MAX
} timer_intr_mode_t;


 //@brief Select if Alarm needs to be loaded by software or automatically reload by hardware.
 
typedef enum 
{
  TIMER_AUTORELOAD_DIS = 0,  //Disable auto-reload: hardware will not load counter value after an alarm event
  TIMER_AUTORELOAD_EN = 1,   //Enable auto-reload: hardware will load counter value after an alarm event
  TIMER_AUTORELOAD_MAX,
} timer_autoreload_t;


 //@brief Data structure with timer's configuration settings
 
typedef struct 
{
  bool alarm_en;                    //Timer alarm enable 
  bool counter_en;                  //Counter enable 
  timer_intr_mode_t intr_type;      //Interrupt mode 
  timer_count_dir_t counter_dir;    //Counter direction  
  bool auto_reload;                 //Timer auto-reload 
  uint32_t divider;                 //Counter clock divider. The divider's range is from from 2 to 65536. 
} timer_config_t;

*/

/*
  //ESP32 core v2.0.4, timer_config_t defined in tools/sdk/esp32/include/hal/include/hal/timer_types.h:
  #if SOC_TIMER_GROUP_SUPPORT_XTAL
  typedef enum {
    TIMER_SRC_CLK_APB = 0,  // Select APB as the source clock
    TIMER_SRC_CLK_XTAL = 1, // Select XTAL as the source clock
  } timer_src_clk_t;
  #endif
  typedef struct {
    timer_alarm_t alarm_en;           // Timer alarm enable
    timer_start_t counter_en;         // Counter enable
    timer_intr_mode_t intr_type;      // Interrupt mode
    timer_count_dir_t counter_dir;    // Counter direction
    timer_autoreload_t auto_reload;   // Timer auto-reload
    uint32_t divider;                 // Counter clock divider. The divider's range is from from 2 to 65536
  #if SOC_TIMER_GROUP_SUPPORT_XTAL
    timer_src_clk_t clk_src;          // Use XTAL as source clock
  #endif
  } timer_config_t;

*/

////////////////////////////////////////

#ifndef USE_ESP32_TIMER_NO
  #define USE_ESP32_TIMER_NO          3
#endif

////////////////////////////////////////

class ESP32FastTimerInterrupt;

////////////////////////////////////////

typedef ESP32FastTimerInterrupt ESP32FastTimer;

#define MAX_ESP32_NUM_TIMERS      4

#define TIMER_DIVIDER             80                                //  Hardware timer clock divider
// TIMER_BASE_CLK = APB_CLK_FREQ = Frequency of the clock on the input of the timer groups
#define TIMER_SCALE               (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds

////////////////////////////////////////

// In esp32/2.0.0-rc1/tools/sdk/esp32c3/include/driver/include/driver/timer.h
// typedef bool (*timer_isr_t)(void *);
//esp_err_t timer_isr_callback_add(timer_group_t group_num, timer_idx_t timer_num, timer_isr_t isr_handler, void *arg, int intr_alloc_flags);
//esp_err_t timer_isr_callback_remove(timer_group_t group_num, timer_idx_t timer_num);
//timer_deinit(timer_group_t group_num, timer_idx_t timer_num);
//esp_err_t timer_group_intr_enable(timer_group_t group_num, timer_intr_t intr_mask);
//esp_err_t timer_group_intr_disable(timer_group_t group_num, timer_intr_t intr_mask);

////////////////////////////////////////

typedef bool (*timer_callback)  (void *);

////////////////////////////////////////

typedef struct
{
  timer_idx_t         timer_idx;
  timer_group_t       timer_group;
  //int                 alarm_interval;
  //timer_autoreload_t  auto_reload;
} timer_info_t;

////////////////////////////////////////

class ESP32FastTimerInterrupt
{
  private:
  
    timer_config_t stdConfig = 
    {
      .alarm_en     = TIMER_ALARM_EN,       //enable timer alarm
      .counter_en   = TIMER_START,          //starts counting counter once timer_init called
      .intr_type    = TIMER_INTR_MAX,
      .counter_dir  = TIMER_COUNT_UP,       //counts from 0 to counter value
      .auto_reload  = TIMER_AUTORELOAD_EN,  //reloads counter automatically
      .divider      = TIMER_DIVIDER,
      .clk_src      = TIMER_SRC_CLK_APB     //Use APB as source clock  
    };

		////////////////////////////////////////

    timer_idx_t       _timerIndex;
    timer_group_t     _timerGroup;
    uint32_t          interruptFlag;        // either TIMER_INTR_T0 or TIMER_INTR_T1
    
    uint8_t           _timerNo;

    timer_callback 		_callback;        // pointer to the callback function
    float             _frequency;       // Timer frequency
    uint64_t          _timerCount;      // count to activate timer

  public:
 
		////////////////////////////////////////

    ESP32FastTimerInterrupt(uint8_t timerNo)
    {     
      _callback = NULL;
        
      if (timerNo < MAX_ESP32_NUM_TIMERS)
      {
        _timerNo  = timerNo;

        _timerIndex = (timer_idx_t)   (_timerNo % TIMER_MAX);
        
        _timerGroup = (timer_group_t) (_timerNo / TIMER_MAX);             
      }
      else
      {
        _timerNo  = MAX_ESP32_NUM_TIMERS;
      }
    };
 
		////////////////////////////////////////

    // frequency (in hertz)
    // No params and duration now. To be added in the future by adding similar functions here or to esp32-hal-timer.c
    bool setFrequency(const float& frequency, timer_callback callback)
    {
      if (_timerNo < MAX_ESP32_NUM_TIMERS)
      {      
        // select timer frequency is 1MHz for better accuracy. We don't use 16-bit prescaler for now.
        // Will use later if very low frequency is needed.
        _frequency  = TIMER_BASE_CLK / TIMER_DIVIDER;   //1000000;
        _timerCount = (uint64_t) _frequency / frequency;
        // count up

        ISR_SERVO_LOGERROR3(F("ESP32_S2_TimerInterrupt: _timerNo ="), _timerNo, F(", _fre ="), TIMER_BASE_CLK / TIMER_DIVIDER);
        ISR_SERVO_LOGERROR3(F("TIMER_BASE_CLK ="), TIMER_BASE_CLK, F(", TIMER_DIVIDER ="), TIMER_DIVIDER);
        ISR_SERVO_LOGERROR3(F("_timerIndex ="), _timerIndex, F(", _timerGroup ="), _timerGroup);
        ISR_SERVO_LOGERROR3(F("_count ="), (uint32_t) (_timerCount >> 32) , F("-"), (uint32_t) (_timerCount));
        ISR_SERVO_LOGERROR1(F("timer_set_alarm_value ="), TIMER_SCALE / frequency);

        timer_init(_timerGroup, _timerIndex, &stdConfig);
        
        // Counter value to 0 => counting up to alarm value as .counter_dir == TIMER_COUNT_UP
        timer_set_counter_value(_timerGroup, _timerIndex , 0x00000000ULL);       
        
        timer_set_alarm_value(_timerGroup, _timerIndex, TIMER_SCALE / frequency);
               
        // enable interrupts for _timerGroup, _timerIndex
        timer_enable_intr(_timerGroup, _timerIndex);
        
        _callback = callback;
        
        // Register the ISR handler       
        // If the intr_alloc_flags value ESP_INTR_FLAG_IRAM is set, the handler function must be declared with IRAM_ATTR attribute
        // and can only call functions in IRAM or ROM. It cannot call other timer APIs.
       //timer_isr_register(_timerGroup, _timerIndex, _callback, (void *) (uint32_t) _timerNo, ESP_INTR_FLAG_IRAM, NULL);
        timer_isr_callback_add(_timerGroup, _timerIndex, _callback, (void *) (uint32_t) _timerNo, 0);

        timer_start(_timerGroup, _timerIndex);
  
        return true;
      }
      else
      {  
        ISR_SERVO_LOGERROR(F("Error. Timer must be 0-3"));
        
        return false;
      }
    }

  
		////////////////////////////////////////

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool attachInterruptInterval(const unsigned long& interval, timer_callback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }
  
		////////////////////////////////////////

    void detachInterrupt()
    {
      timer_group_intr_disable(_timerGroup, (_timerIndex == 0) ? TIMER_INTR_T0 : TIMER_INTR_T1);
    }
  
		////////////////////////////////////////
		
		void disableTimer()
    { 
      timer_group_intr_disable(_timerGroup, (_timerIndex == 0) ? TIMER_INTR_T0 : TIMER_INTR_T1);
    }

		////////////////////////////////////////
		
    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      if ( (_frequency != 0) && (_timerCount != 0) && (_callback != NULL) )
        timer_group_intr_enable(_timerGroup, (_timerIndex == 0) ? TIMER_INTR_T0 : TIMER_INTR_T1);
    }
}; // class ESP32FastTimerInterrupt

////////////////////////////////////////


#endif      // ESP32_S2_FastTimerInterrupt_h
