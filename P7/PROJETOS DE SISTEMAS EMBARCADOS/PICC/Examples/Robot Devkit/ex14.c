////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                           ex_ezapp_robot.c                         ////
////                                                                    ////
//// Example of using EZ App Lynx to control robot development kit.     ////
//// Follow EZ App Lynx tutorial for directions on installing app,      ////
//// launching app, and connecting app to board over Bluetooth.         ////
////                                                                    ////
//// The app has two screens, "Motors" and "Sensors".  The screen can   ////
//// be chosen using the pull-down at the bottom of the screen on the   ////
//// app.  The "Motors" screen provides a joystick control for driving  ////
//// the car (incoming joystick control from app is used to set the     ////
//// PWM duty and motor direction of the DC motors).  The "Sensors"     ////
//// screen provides a raw look at the current sensor values of all     ////
//// the sensors on the board.                                          ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2017 Custom Computer Services            ////
////                                                                    ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler or to users who purchased the rights to use this code.    ////
//// This source code may only be distributed to other licensed users   ////
//// of the CCS C compiler or other users who purchased the rights to   ////
//// this code.  The rights to use this code can only be granted        ////
//// by CCS.  No other use, reproduction or distribution is permitted   ////
//// without written permission.  Derivative programs created using     ////
//// this software in object code form are not restricted in any way.   ////
////                                                                    ////
////                     http://www.ccsinfo.com                         ////
////////////////////////////////////////////////////////////////////////////

//#define CONFIG_UART_CONSOLE_FROM_USB_CDC

#define MY_ROBOT_ID  1
#if defined(MY_ROBOT_ID)
   #if (MY_ROBOT_ID==1)
      #define BT_FRIENDLY_NAME "EZROBOT1"
   #elif (MY_ROBOT_ID==2)
      #define BT_FRIENDLY_NAME "EZROBOT2"
   #elif (MY_ROBOT_ID==3)
      #define BT_FRIENDLY_NAME "EZROBOT3"
   #elif (MY_ROBOT_ID==4)
      #define BT_FRIENDLY_NAME "EZROBOT4"
   #else
      #error fix this
   #endif
#endif

//////// end configuration

#include <devkit_picrobot.h>

#include <RN4020_MLDP_Bluetooth.h>
#include <bluetooth_serial.c>
#include <EZApp.c>

#include <tick.c>

#include <mag3110.c>

#define BUTTON0_PRESSED BUTTON_PRESSED
#define ADC_MAX   1023

struct
{
   enum
   {
      MY_SCREEN_MOTORS = 0,
      MY_SCREEN_SENSORS,
      
      MY_SCREENS_NUM
   } screen;
   ezapp_field_index_t screenChoiceField;
} g_MyEzApp;

struct
{
   ezapp_field_index_t joystick;
  #if defined(MOTOR_TASK_DEBUG_PWM)
   ezapp_field_index_t angle;
   ezapp_field_index_t strength;
   char angleStr[32];
   char strengthStr[32];
  #endif
} g_ScreenMotorFields;

void ScreenMotorInit(void)
{
   static ezapp_joystick_cfg_t joyCfg;

#if 0
   ezapp_image_cfg_t imgCfg;
   
   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)" ");
  
   memset(&imgCfg, 0, sizeof(imgCfg));
   //imgCfg.scaleType = EZAPP_IMAGE_SCALE_HALF;
   imgCfg.scaleType = EZAPP_IMAGE_SCALE_FULL;
   EZAppAddFieldImage(imgCfg, (rom char*)"http://www.ccsinfo.com/images/ccs-logo2000.png");
#else
   ezapp_field_index_t strIdx;
   static char str[32];
   strIdx = EZAppAddFieldString();
   EZAppSetStringStyle(strIdx, 0, +5);
   sprintf(str, "Robot car %u", MY_ROBOT_ID);
   EZAppSetValueString(strIdx, str);
#endif
   
   memset(&joyCfg, 0, sizeof(joyCfg));
   joyCfg.backgroundColor = EZAppCreateRGB(0x8A,0x89,0x8D);
   joyCfg.borderColor = EZAppCreateRGB(0x3D,0x3B,0x41);
   joyCfg.stickColor = EZAppCreateRGB(0x9F,0x35,0x34);
   joyCfg.borderWidth = 5;
   joyCfg.maxHeight = 30;
   joyCfg.autoReCenter = true;
   g_ScreenMotorFields.joystick = EZAppAddFieldJoystick(&joyCfg, NULL); //4   
   
  #if defined(MOTOR_TASK_DEBUG_PWM)
   sprintf(g_ScreenMotorFields.angleStr, "?");
   g_ScreenMotorFields.angle = EZAppAddFieldText((rom char*)"Angle:", 0);  //2
   EZAppSetValueString(g_ScreenMotorFields.angle, g_ScreenMotorFields.angleStr);
   
   sprintf(g_ScreenMotorFields.strengthStr, "?");
   g_ScreenMotorFields.strength = EZAppAddFieldText((rom char*)"Strength:", 0);  //3
   EZAppSetValueString(g_ScreenMotorFields.strength, g_ScreenMotorFields.strengthStr);   
  #endif
}

#include <math.h>

unsigned int32 _round(float f)
{
   f += (float)0.5;
   return((unsigned int32)f);
}

void ScreenMotorTask(void)
{
   float slope, f;
   int quadrant;
   int l, r;
   
   uint16_t val;
   uint16_t angle;
   uint8_t strength;
   
   if (EZAppGetKbhit(g_ScreenMotorFields.joystick))
   {
      // get the new value and extract the angle/strenth parts
      val = EZAppGetValue(g_ScreenMotorFields.joystick);
      strength = EZAppGetJoystickStrength(val);
      angle = EZAppGetJoystickAngle(val);
      
      quadrant = 0;
      while(angle > 90)
      {
         angle -= 90;
         quadrant++;
      }
      
      if ((quadrant == 2) && (angle == 90))
      {
         angle = 0;
         quadrant++;
      }
      
      if (strength == 0)
      {
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);
      }
      else if (quadrant == 0)
      {
         r = strength;
         slope = -100.0 / 90.0;
         f = ((100.0 + slope * (float)angle)*((float)strength/100.0));
         l = _round(f);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, l);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, r);
      }
      else if (quadrant == 1)
      {
         r = strength;
         slope = 100.0 / 90.0;
         f = (slope * (float)angle)*((float)strength/100.0);
         l = _round(f);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -l);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -r);
      }
      else if (quadrant == 2)
      {
         l = strength;
         slope = -100.0 / 90.0;
         f = ((100.0 + slope * (float)angle)*((float)strength/100.0));
         r = _round(f);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -l);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -r);
      }
      else if (quadrant == 3)
      {
         l = strength;
         slope = 100.0 / 90.0;
         f = (slope * (float)angle)*((float)strength/100.0);
         r = _round(f);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, l);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, r);
      }
      
     #if defined(MOTOR_TASK_DEBUG_PWM)
      // display the current Angle (in degrees)
      //sprintf(g_ScreenMotorFields.angleStr, "%lu",angle);
      sprintf(g_ScreenMotorFields.angleStr, "%lu", l);
      EZAppSetValueString(g_ScreenMotorFields.angle, g_ScreenMotorFields.angleStr);
      
      // display the current Strength (in percent)
      //sprintf(g_ScreenMotorFields.strengthStr,"%lu",strength);
      sprintf(g_ScreenMotorFields.strengthStr,"%lu", r);
      EZAppSetValueString(g_ScreenMotorFields.strength, g_ScreenMotorFields.strengthStr);
     #endif  
   }
}

struct
{
   ezapp_field_index_t leftLine;
   ezapp_field_index_t middleLine;
   ezapp_field_index_t rightLine;
   ezapp_field_index_t leftProximity;
   ezapp_field_index_t rightProximity;
   ezapp_field_index_t battery;
   ezapp_field_index_t button;
   ezapp_field_index_t compass;
} g_ScreenSensorFields;

void ScreenSensorsInit(void)
{
   ezapp_field_index_t strIdx;
   strIdx = EZAppAddFieldString();
   EZAppSetStringStyle(strIdx, 0, +5);
   EZAppSetValueStringROM(strIdx, (rom char*)"Robot sensor readings");
   
   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)" ");
   
   g_ScreenSensorFields.leftLine = EZAppAddFieldAnalogValue(
         (rom char *)"LEFT LINE",
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
      
   g_ScreenSensorFields.middleLine = EZAppAddFieldAnalogValue(
         (rom char *)"MID LINE",
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
      
   g_ScreenSensorFields.rightLine = EZAppAddFieldAnalogValue(
         (rom char *)"RIGHT LINE",
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
      
   g_ScreenSensorFields.leftProximity = EZAppAddFieldAnalogValue(
         (rom char *)"LEFT PROX",
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
      
   g_ScreenSensorFields.rightProximity = EZAppAddFieldAnalogValue(
         (rom char *)"RIGHT PROX",
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );

   g_ScreenSensorFields.battery = EZAppAddFieldAnalogValueScaled(
      (rom char*)"BATTERY",      //rom char * prLabel,
      EZAPP_ANALOG_TYPE_SLIDER,  //ezapp_analog_type_t analogType, 
      0,                         //uint16_t minValue,
      ADC_MAX,                   //uint16_t maxValue,
      660,                       //uint16_t scaling,
      2,                         //uint8_t decimalPlaces
   );

   g_ScreenSensorFields.button = EZAppAddFieldAnalogValue(
         (rom char *)"BUTTON",
         EZAPP_ANALOG_TYPE_TEXT_VALUE,
         1
      );
      
   g_ScreenSensorFields.compass = EZAppAddFieldText(
         (rom char*)"COMPASS",
         0
      );
   EZAppSetValueStringROM(g_ScreenSensorFields.compass, (rom char*)"?");
   
   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)" ");
}

unsigned int16 MyAdcRead(unsigned int8 chan)
{
   set_adc_channel(chan);
   delay_us(100);
   return(read_adc());
}

void ScreenSensorsTask(void)
{
   static TICK t;
   mag3110_axis_t axis;
   static char axisString[64];
   
   if (TickIsExpired(t, TICKS_PER_SECOND/4))
   {
      UARTConsolePutc('.');
      
      t = TickGet();
      
      EZAppSetValue(g_ScreenSensorFields.leftLine, MyAdcRead(AN_CHANNEL_LEFT_LINE));
      EZAppSetValue(g_ScreenSensorFields.rightLine, MyAdcRead(AN_CHANNEL_RIGHT_LINE));
      EZAppSetValue(g_ScreenSensorFields.middleLine, MyAdcRead(AN_CHANNEL_MIDDLE_LINE));
      EZAppSetValue(g_ScreenSensorFields.leftProximity, MyAdcRead(AN_CHANNEL_LEFT_PROXIMITY));
      EZAppSetValue(g_ScreenSensorFields.rightProximity, MyAdcRead(AN_CHANNEL_RIGHT_PROXIMITY));
      EZAppSetValue(g_ScreenSensorFields.battery, MyAdcRead(AN_CHANNEL_BATTERY));
      
      if (Mag3110IsReady())
      {
         Mag3110Read(&axis);      
         sprintf(axisString, "X=%ld\nY=%ld\nZ=%ld", axis.x, axis.y, axis.z);
         EZAppSetValueString(g_ScreenSensorFields.compass, axisString);
      }
   }
   
   EZAppSetValue(g_ScreenSensorFields.button, BUTTON0_PRESSED());
}

rom unsigned int8 g_Style1[] =
{
   EZAPP_STYLE_DEFAULT_ALL(),
   EZAPP_STYLE_ROW_SPACINGS_CREATE(40, EZAPP_ALIGN_HOR_RIGHT|EZAPP_ALIGN_VER_TOP, EZAPP_ALIGN_HOR_LEFT|EZAPP_ALIGN_VER_TOP, 0, 25, 0)
};

void MyEzAppGotoScreen(unsigned int8 val)
{
   printf(UARTConsolePutc, "MyEzAppGotoScreen(%u)\r\n", val);

   g_MyEzApp.screen = val;

   WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
   WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);
 
   EZAppFieldsClearAll();
   
   EZAppSetFGColor(0);
   EZAppSetBGColor(-1);
   EZAppSetAppColor(-1);
    
   EZAppSetTitleROM((rom char *)"CCS Robot Development Kit");
   
   EZAppAddStylesROM(g_Style1, sizeof(g_Style1));
 
   if (g_MyEzApp.screen == MY_SCREEN_MOTORS)
   {
      ScreenMotorInit();
   }
   else
   {
      ScreenSensorsInit();
   }
   
   g_MyEzApp.screenChoiceField = EZAppAddFieldPulldownValue(
         (rom char*)"Demo Select",
         MY_SCREENS_NUM,
         (rom char*)"Motors\tSensors"
      );   
      
   EZAppSetValue(g_MyEzApp.screenChoiceField, g_MyEzApp.screen);
}

void MyEzAppInit(void)
{
   EZAppInit();
   
   memset(&g_MyEzApp, 0, sizeof(g_MyEzApp));
   
   //MyEzAppGotoScreen(MY_SCREEN_SENSORS);
   MyEzAppGotoScreen(MY_SCREEN_MOTORS);
}

void MyEzAppTask(void)
{
   EZAppTask();
   
   if (EZAppGetKbhit(g_MyEzApp.screenChoiceField))
   {
      if (EZAppGetValue(g_MyEzApp.screenChoiceField) != g_MyEzApp.screen)
      {
         printf(UARTConsolePutc, "EZAppGetKbhit screen=%u\r\n", EZAppGetValue(g_MyEzApp.screenChoiceField));
      
         MyEzAppGotoScreen(EZAppGetValue(g_MyEzApp.screenChoiceField));
      }
   }
   else
   {
      if (g_MyEzApp.screen == MY_SCREEN_MOTORS)
      {
         ScreenMotorTask();
      }
      else
      {
         ScreenSensorsTask();
      }
   }
}

void main(void)
{
   rom char consoleHeaderStr[] = "\r\n\r\nex_ezapp_robot.c running\r\n\r\n";
   
   HW_INIT();
   
   UARTConsoleInit(9600);
   UARTConsolePutsROM(consoleHeaderStr);
   
   Mag3110Init();
   
   WheelMotorInit();
   
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_2);
   
   MyEzAppInit();
   
   for(;;)
   {
      restart_wdt();
      
      UARTConsoleTask();
      
      HW_TASK();
      
      MyEzAppTask();
      
      if (UARTConsoleKbhit() && (UARTConsoleGetc() == '?'))
      {
         UARTConsolePutsROM(consoleHeaderStr);
      }
   }
}
