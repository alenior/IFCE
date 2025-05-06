///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                             bme280.c                              ////
////                                                                   ////
//// Driver for Bosch BME280 Environmental sensor.  This sensor can    ////
//// read temperature, pressure and humidity.  This driver returns     ////
//// calibrated/compensated values by using the trim values on the     ////
//// sensor.  At this time this driver only supports I2C mode, it      ////
//// doesn't support SPI mode.                                         ////
////                                                                   ////
////                                                                   ////
//// API                                                               ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// bme280_prepare_defaults(bme280_init_t *pConfig)                   ////
////  Initialize the configuration vales to generally safe/default     ////
////  values (no oversampling, no IIR filter).                         ////
////                                                                   ////
//// ok = bme280_init_user(bme280_init_t config);                      ////
//// ok = bme280_init();                                               ////
////  Initializes the BME280 sensor.  There are two version of the     ////
////  function; the function with no parameters uses default config    ////
////  values, the function with a config parameter uses the user       ////
////  provided config values.  See bme280_init_t below for values      ////
////  that can be used.  Returns TRUE if device was initialized        ////
////  correctly.  Be aware that using filter slows the response time   ////
////  and lowers the resolution, see the datasheet for the module      ////
////  for specifics.  Filter only updates after a sample, so take      ////
////  care using the filter with BM280_MODE_FORCED run mode.           ////
////                                                                   ////
//// ok = bme280_ok();                                                 ////
////  Returns TRUE if device is initialized and responding over I2C.   ////
////  This function will cause an active read over I2C to verify.      ////
////  If this returns FALSE then the device should be re-initialized   ////
////  with bme280_init().                                              ////
////                                                                   ////
//// ok = bme280_set_mode(bme280_sensor_mode_t mode)                   ////
////  Configure the operating mode of the sensor.  See                 ////
////  bme280_sensor_mode_t below for more documentation.               ////
////  bm280_init() puts the module into sleep mode.                    ////
////                                                                   ////
//// mode = bme280_get_mode()                                          ////
////  Reads mode from the BME280 module and returns it in              ////
////  bme280_sensor_mode_t format.  Be aware that this module has      ////
////  2 valid values for the FORCED mode.  Returns -1 if there was     ////
////  an error.                                                        ////
////                                                                   ////
//// busy = bme280_busy();                                             ////
////  Returns TRUE if device is busy sampling or if there is an error  ////
////  communicating with the device.  If it returns FALSE, then        ////
////  the bme280_get_****() functions can be used to read the value    ////
////  from the device.  Only use this function if you are using the    ////
////  BM280_MODE_FORCED mode.  If you are in BM280_MODE_SLEEP it's     ////
////  never sampling.  If you are in BM280_MODE_NORMAL mode it's       ////
////  continously sampling and the results of this function will       ////
////  be invalid.                                                      ////
////                                                                   ////
//// ok = bme280_get_temperature(signed int32 *pTemp)                  ////
//// ok = bme280_get_pressure(signed int32 *pTemp,                     ////
////                    unsigned int32 *pPress)                        ////
//// ok = bme280_get_humidity(signed int32 *pTemp,                     ////
////                    unsigned int32 *pPress, unsigned int32 *pHum)  ////
////  Reads a set of values from the devices, returns TRUE if values   ////
////  were read correctly.  If you pass a NULL pointer then it won't   ////
////  save that result.  Temperature is stored in hundredths C (output ////
////  value of "5123" equals 51.23 DegC.).  Pressure is stored in      ////
////  Pa (output value of "96386" equals 96386 Pa = 963.86 hPa).       ////
////  Humidity is stored in relative humidity percent in 1024 steps    ////
////  (output value of "47445" represents 47445/1024 = 46.333 %RH).    ////
////                                                                   ////
////                                                                   ////
//// CONFIG OPTIONS                                                    ////
//// ----------------------------------------------------------------- ////
//// STREAM_I2C_BME280 - (Optional) If defined, this must be defined   ////
////  to an already created #use i2c() stream.  Doing so will inhibit  ////
////  the #use i2c() in this library, useful for sharing one I2C bus   ////
////  with several devices.                                            ////
////                                                                   ////
//// PIN_BME280_SCL, PIN_BME280_SDA - Configure the I2C pins to use,   ////
////  if the STREAM_I2C_BME280 option isn't used (see above).          ////
////                                                                   ////
//// BME280_I2C_ADDRESS - The I2C slave address of the BME280 module.  ////
////  It can be either 0xEC or 0xEE.                                   ////
////                                                                   ////
////                                                                   ////
//// EXAMPLE USAGE                                                     ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
////  #include "bme280.c"                                              ////
////                                                                   ////
////  signed int32 temp;                                               ////
////  unsigned int32 press;                                            ////
////  unsigned int32 hum;                                              ////
////  int1 ok;                                                         ////
////                                                                   ////
////  bme280_init();                                                   ////
////  bme280_set_mode(BM280_MODE_NORMAL);                              ////
////  for(;;) {                                                        ////
////     delay_ms(1000);                                               ////
////     ok = bme280_get_humidity(&temp, &press, &hum);                ////
////     if (!ok)                                                      ////
////        printf("Error reading BME280 sensor!\r\n");                ////
////     else                                                          ////
////        printf("Temp=%ld Press=%lu Hum=%lu\r\n", temp, press, hum);////
////  }                                                                ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2021 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __BME280_C__
#define __BME280_C__

#ifndef PIN_BME280_SCL
   #define PIN_BME280_SCL  PIN_C3
   #define PIN_BME280_SDA  PIN_C4
#endif

#ifndef BME280_I2C_ADDRESS
   #define BME280_I2C_ADDRESS 0xEC
#endif

#ifndef BME280_I2C_BAUD
   //spec says max i2c clock is 3.4MHz, but I wasn't getting anywhere near that fast.
   #define BME280_I2C_BAUD 1000000
#endif

#if !defined(BME280_IS_I2C) && !defined(BME280_IS_SPI)
   #define BME280_IS_I2C
#endif

typedef enum
{
   BM280_OSRS_SKIPPED = 0,  //skipped, output set to 0x8000
   BM280_OSRS_1X = 1,   //oversampling x1
   BM280_OSRS_2X = 2,   //oversampling x2
   BM280_OSRS_4X = 3,   //oversampling x4
   BM280_OSRS_8X = 4,   //oversampling x8
   BM280_OSRS_16X = 5   //oversampling x16
} bme280_oversampling_t;

typedef enum
{
   /**
      Device is in low power mode and not performing any sampling.  Still
      responds to serial communications.
   */
   BM280_MODE_SLEEP = 0,
   
   /**
      Device is performing one sample, and then it goes back to sleep mode.
   */
   
   BM280_MODE_FORCED = 1,
   
   /**
      Device is in high power mode, and is always sampling.  Sample rate
      is configured using the bme280_timing_t variable in bme280_init_t.
   */
   BM280_MODE_NORMAL = 3
} bme280_sensor_mode_t;

typedef enum
{
   BM280_TIMING_0P5 = 0,   //0.5ms
   BM280_TIMING_10 = 6,   //10ms
   BM280_TIMING_20 = 7,   //20ms
   BM280_TIMING_62P5 = 1,   //62.5ms
   BM280_TIMING_125 = 2,   //125ms
   BM280_TIMING_250 = 3,   //250ms
   BM280_TIMING_500 = 4,   //500ms
   BM280_TIMING_1000 = 5,   //1000ms
} bme280_timing_t;

typedef enum
{
   BM280_FILTER_OFF = 0,
   BM280_FILTER_2 = 1,  //filter coefficient = 2
   BM280_FILTER_4 = 2,  //filter coefficient = 4
   BM280_FILTER_8 = 3,  //filter coefficient = 8
   BM280_FILTER_1 = 4,  //filter coefficient = 16
} bme280_filter_t;

typedef struct
{
   bme280_oversampling_t humidityOversampling;
   bme280_oversampling_t temperatureOversampling;
   bme280_oversampling_t pressureOversampling;
   bme280_timing_t inactiveDuration;
   bme280_filter_t iirFilter;
} bme280_init_t;

/////////////

#if defined(BME280_IS_I2C) && defined(BME280_IS_SPI)
   #error Only define one comm method!
#endif

#if defined(BME280_IS_SPI)
   //max spi clock is 10MHz
   #error SPI isn't supported in this driver
#endif

#if defined(BME280_IS_I2C)
   #ifndef STREAM_I2C_BME280
      #use i2c(MASTER, SCL=PIN_BME280_SCL, SDA=PIN_BME280_SDA, STREAM=STREAM_I2C_BME280, NOINIT, FORCE_HW, FAST=BME280_I2C_BAUD)
   
      // returns TRUE if OK, FALSE if ERROR
      #define BME280_HW_CHECK() (input(PIN_BME280_SCL) && input(PIN_BME280_SDA))
      
      #define BME280_HW_INIT()      i2c_init(STREAM_I2C_BME280, TRUE)
      #define BME280_HW_DISABLE()   i2c_init(STREAM_I2C_BME280, FALSE); output_float(PIN_BME280_SCL); output_float(PIN_BME280_SDA)
   #else
      #define BME280_HW_CHECK()  TRUE
      
      #define BME280_HW_INIT()
      #define BME280_HW_DISABLE()
   #endif
#endif

#ifndef debug_bme280
   #define debug_bme280(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
#endif

#ifndef debug2_bme280
   #define debug2_bme280(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
#else
   #define __DO_DEBUG2_BME280
#endif

#include <stddef.h>

// returns TRUE if OK, FALSE if error
static int1 _bme280_read_bytes(unsigned int8 *p, unsigned int8 address, unsigned int8 n)
{
   int1 ret = TRUE;
   
   if (n == 0)
      return(TRUE);

  #if defined(BME280_IS_I2C)
   i2c_start(STREAM_I2C_BME280);
   if (i2c_write(STREAM_I2C_BME280, BME280_I2C_ADDRESS) != 0) ret = FALSE;
   if (ret)
   {
      if (i2c_write(STREAM_I2C_BME280, address) != 0) ret = FALSE;
   }
   if (ret)
   {
      i2c_start(STREAM_I2C_BME280);
      if (i2c_write(STREAM_I2C_BME280, BME280_I2C_ADDRESS|1) != 0) ret = FALSE;
   }
   if (ret)
   {
      while(n--)
      {
         *p++ = i2c_read(STREAM_I2C_BME280, (n!=0));
      }
   }
   i2c_stop(STREAM_I2C_BME280);
  #endif
   
   return(ret);
}

static unsigned int8 _bme280_read_byte(unsigned int8 address)
{
   unsigned int8 ret;
   
   if (!_bme280_read_bytes(&ret, address, 1))
   {
      ret = 0;
   }
   
   return(ret);
}

static int1 _bme280_write_byte(unsigned int8 address, unsigned int8 val)
{
   int1 ret = TRUE;
   
  #if defined(BME280_IS_I2C)
   i2c_start(STREAM_I2C_BME280);
   if (i2c_write(STREAM_I2C_BME280, BME280_I2C_ADDRESS) != 0) ret = FALSE;
   if (ret)
   {
      if (i2c_write(STREAM_I2C_BME280, address) != 0) ret = FALSE;
   }
   if (ret)
   {
      if (i2c_write(STREAM_I2C_BME280, val) != 0) ret = FALSE;
   }
   i2c_stop(STREAM_I2C_BME280);
  #endif
   
   return(ret);
}

typedef union
{
   struct
   {
      unsigned int mode:2;
      unsigned int osrs_p:3;
      unsigned int osrs_t:3;
   };
   unsigned int8 val;
} _bme280_ctrl_meas_t;

struct
{
   unsigned int8 isInit;
   _bme280_ctrl_meas_t ctrlMeas;
   
   struct
   {
      unsigned int16 dig_T1;  //0x88,0x89
      signed int16 dig_T2;  //0x8a,0x8b
      signed int16 dig_T3;  //0x8c,0x8d
      unsigned int16 dig_P1;  //0x8e,0x8f
      signed int16 dig_P2;  //0x90,0x91
      signed int16 dig_P3;  //0x92,0x93
      signed int16 dig_P4;  //0x94,0x95
      signed int16 dig_P5;  //0x96,0x97
      signed int16 dig_P6;  //0x98,0x99
      signed int16 dig_P7;  //0x9a,0x9b
      signed int16 dig_P8;  //0x9c,0x9d
      signed int16 dig_P9;  //0x9e,0x9f
      unsigned int8 dig_H1;   //0xa1
      signed int16 dig_H2;  //0xe1,0xe2
      unsigned int8 dig_H3;   //0xe3
      signed int16 dig_H4;  //0xe4,0xe5[3:0]
      signed int16 dig_H5;  //0xe5[7:4],0xe6
      signed int8 dig_H6;  //0xe7
      signed int32 t_fine;
   } comps;
} _g_Bme280 = {0};

void bme280_prepare_defaults(bme280_init_t *pConfig)
{
   memset(pConfig, 0, sizeof(bme280_init_t));
   
   pConfig->humidityOversampling = BM280_OSRS_1X;
   pConfig->temperatureOversampling = BM280_OSRS_1X;
   pConfig->pressureOversampling = BM280_OSRS_1X;
   pConfig->inactiveDuration = BM280_TIMING_125;
}

static int1 _bme280_version_check(void)
{
   return(_bme280_read_byte(0xD0) == 0x60);
}

int1 bme280_ok(void)
{
   return
   (
      (_g_Bme280.isInit != 0) &&
      _bme280_version_check()
   );
}

#define _DEBUG_BME280_DISPLAY_TRIMS()  \
      debug_bme280(debug_putc, "dig_T1=%lu dig_T2=%ld dig_T3=%ld ",  \
            _g_Bme280.comps.dig_T1, \
            _g_Bme280.comps.dig_T2, \
            _g_Bme280.comps.dig_T3  \
         ); \
      debug_bme280(debug_putc, "dig_P1=%lu dig_P2=%ld dig_P3=%ld dig_P4=%ld ",   \
            _g_Bme280.comps.dig_P1, \
            _g_Bme280.comps.dig_P2, \
            _g_Bme280.comps.dig_P3, \
            _g_Bme280.comps.dig_P4  \
         ); \
      debug_bme280(debug_putc, "dig_P5=%ld dig_P6=%ld dig_P7=%ld dig_P8=%ld dig_P9=%ld ", \
            _g_Bme280.comps.dig_P5, \
            _g_Bme280.comps.dig_P6, \  
            _g_Bme280.comps.dig_P7, \
            _g_Bme280.comps.dig_P8, \
            _g_Bme280.comps.dig_P9  \     
         ); \
      debug_bme280(debug_putc, "dig_H1=%u dig_H2=%ld dig_H3=%u dig_H4=%ld dig_H5=%ld dig_H6=%d ",  \
            _g_Bme280.comps.dig_H1, \
            _g_Bme280.comps.dig_H2, \
            _g_Bme280.comps.dig_H3, \
            _g_Bme280.comps.dig_H4, \
            _g_Bme280.comps.dig_H5, \
            _g_Bme280.comps.dig_H6  \
         )

int1 bme280_init_user(bme280_init_t config)
{
   union
   {
      struct
      {
         unsigned int osrs_h:3;
      };
      unsigned int8 val;
   } ctrl_hum;
   
   union
   {
      struct
      {
         unsigned int spi3w_en:1;
         unsigned int:1;
         unsigned int filter:3;
         unsigned int t_sb:3;
      };
      unsigned int8 val;
   } ctrl_config;
   
   unsigned int32 scr32;
   unsigned int16 scr16, j16;
   
   _g_Bme280.isInit = 0;
   
   debug_bme280(debug_putc, "bme280_init() ");
   
   BME280_HW_DISABLE();
  
   if (!BME280_HW_CHECK())
   {
      debug_bme280(debug_putc, "HW_CHECK_FAIL\r\n");
      return(0);
   }
   
   BME280_HW_INIT();
  
   delay_ms(2);
  
   if (_bme280_version_check())  //0xD0=chip_id location, 0x60=bme280 chip id
   {
      debug_bme280(debug_putc, "VER_OK ");
      
      _bme280_write_byte(0xE0, 0xB6);  //0xE0=reset location, 0xB6=reset value
      
      delay_ms(2);
      
      ctrl_hum.val = 0;
      ctrl_hum.osrs_h = config.humidityOversampling;
      _bme280_write_byte(0xF2, ctrl_hum.val);
      
      _g_Bme280.ctrlMeas.val = 0;
      _g_Bme280.ctrlMeas.mode = BM280_MODE_SLEEP;
      _g_Bme280.ctrlMeas.osrs_p = config.pressureOversampling;
      _g_Bme280.ctrlMeas.osrs_t = config.temperatureOversampling;
      _bme280_write_byte(0xF4, _g_Bme280.ctrlMeas.val);
      
      ctrl_config.val = 0;
     #if defined(BME280_IS_SPI)
      ctrl_config.spi3w_en = 1;
     #endif
      ctrl_config.filter = config.iirFilter;
      ctrl_config.t_sb = config.inactiveDuration;
      _bme280_write_byte(0xF5, ctrl_config.val);
      
      // CCS C Compiler is little endian, and the compensation values are little endian... so no swaps needed.
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_T1, 0x88, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_T2, 0x8a, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_T3, 0x8c, 2);
      
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P1, 0x8e, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P2, 0x90, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P3, 0x92, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P4, 0x94, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P5, 0x96, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P6, 0x98, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P7, 0x9a, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P8, 0x9c, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_P9, 0x9e, 2);
      
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_H1, 0xa1, 1);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_H2, 0xe1, 2);
      _bme280_read_bytes((unsigned int8 *)&_g_Bme280.comps.dig_H3, 0xe3, 1);
      
      scr32 = 0;
      _bme280_read_bytes((unsigned int8 *)&scr32, 0xe4, 3);
      
      scr16 = make8(scr32, 0);
      scr16 <<= 4;
      scr16 &= 0xFF0;
      scr16 += (make8(scr32, 1) & 0xF);
      if (bit_test(scr16, 11))  scr16 |= 0xF000;
      _g_Bme280.comps.dig_H4 = scr16;
      
      scr16 = make8(scr32, 1);
      scr16 >>= 4;
      scr16 &= 0xF;
      j16 = make8(scr32, 2);
      j16 <<= 4;
      j16 &= 0xFF0;
      scr16 += j16;
      if (bit_test(scr16, 11))  scr16 |= 0xF000;
      _g_Bme280.comps.dig_H5 = scr16;     
    
      _g_Bme280.isInit = _bme280_read_bytes(&_g_Bme280.comps.dig_H6, 0xe7, 1);
      
      _DEBUG_BME280_DISPLAY_TRIMS();
   }
   
   debug_bme280(debug_putc, "RET=%U\r\n", (_g_Bme280.isInit != 0));
   
   return(_g_Bme280.isInit != 0);
}

int1 bme280_init(void)
{
   bme280_init_t config;
   
   bme280_prepare_defaults(&config);
   
   return(bme280_init_user(config));
}

int1 bme280_set_mode(bme280_sensor_mode_t mode)
{
   if (!bme280_ok())
      return(FALSE);
      
   _g_Bme280.ctrlMeas.mode = mode;
   return(_bme280_write_byte(0xF4, _g_Bme280.ctrlMeas.val));
}

unsigned int8 bme280_get_mode(void)
{
   unsigned int8 read;
   
   if (!bme280_ok())
      return(-1);
   
   if (!_bme280_read_bytes(&read, 0xF4, 1))
      return(-1);
   
   _g_Bme280.ctrlMeas.val = read;
   return(_g_Bme280.ctrlMeas.mode);
}

int1 bme280_busy(void)
{
   unsigned int8 status;
   
   if (!bme280_ok())
      return(TRUE);
      
   status = _bme280_read_byte(0xF3);   //0xF3=status location

   return(bit_test(status, 3));
}

// THIS FUNCTION MOSTLY WRITTEN BY BOSCH, PROVIDED IN THEIR DATASHEETS.
// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC.
// t_fine carries fine temperature as global value
static signed int32 _bme280_compensate_T_int32(signed int32 adc_T)
{
   signed int32 var1, var2, T;
   var1 = ((((adc_T/8) - ((signed int32)_g_Bme280.comps.dig_T1*2))) * ((signed int32)_g_Bme280.comps.dig_T2)) / 2048;
  #if 0
   var2 = (((((adc_T/16) - ((signed int32)_g_Bme280.comps.dig_T1)) * ((adc_T/16) - ((signed int32)_g_Bme280.comps.dig_T1))) / 4096) * ((signed int32)_g_Bme280.comps.dig_T3)) / 16384;
  #else
   signed int32 var3, var4, var5, var6;
   var3 = ((adc_T/16) - ((signed int32)_g_Bme280.comps.dig_T1));
   var4 = 0;
   var5 = ((var3 * var3) / 4096);
   var6 = (var5 * ((signed int32)_g_Bme280.comps.dig_T3));
   var2 = var6 / 16384;
   debug_bme280(debug_putc, "_comp_t %ld %ld %ld %ld\r\n", var3, var4, var5, var6);
  #endif
   
   _g_Bme280.comps.t_fine = var1 + var2;
   T = (_g_Bme280.comps.t_fine * 5 + 128) / 256;
   
   debug_bme280(debug_putc, "_bme280_compensate_T_int32(%ld) %ld %ld %ld %ld\r\n", adc_T, T, _g_Bme280.comps.t_fine, var1, var2);
   
   return T;
}

// THIS FUNCTION MOSTLY WRITTEN BY BOSCH, PROVIDED IN THEIR DATASHEETS.
// Returns pressure in Pa as unsigned 32 bit integer. Output value of "96386" equals 96386 Pa = 963.86 hPa
static unsigned int32 _bme280_compensate_P_int32(signed int32 adc_P)
{
   signed int32 var1, var2;
   unsigned int32 p;

   var1 = (((signed int32)_g_Bme280.comps.t_fine)/2) - (signed int32)64000;
   var2 = (((var1/4) * (var1/4)) / 2048 ) * ((signed int32)_g_Bme280.comps.dig_P6);
   debug2_bme280(debug_putc, "P STEP1 %ld %ld\r\n", var1, var2);
   var2 = var2 + ((var1*((signed int32)_g_Bme280.comps.dig_P5))*2);
   debug2_bme280(debug_putc, "P STEP2a %ld %ld\r\n", var1, var2);
  #if 0
   var2 = (var2/4)+(((signed int32)_g_Bme280.comps.dig_P4)*65536);
   debug2_bme280(debug_putc, "P STEP2b %ld %ld %ld\r\n", var1, var2, (((signed int32)_g_Bme280.comps.dig_P4)*65536));
  #else
   var2 /= 4;
   debug2_bme280(debug_putc, "P STEP2- %ld\r\n", var2);
   var2 += ((signed int32)_g_Bme280.comps.dig_P4) * 65536;
   debug2_bme280(debug_putc, "P STEP2b %ld\r\n", var2);
  #endif
   var1 = ((((signed int32)_g_Bme280.comps.dig_P3 * (((var1/4) * (var1/4)) / 8192 )) / 8) + ((((signed int32)_g_Bme280.comps.dig_P2) * var1)/2))/262144;
   debug2_bme280(debug_putc, "P STEP3a %ld %ld\r\n", var1, var2);
   var1 =((((32768+var1))*((signed int32)_g_Bme280.comps.dig_P1))/32768);
   debug2_bme280(debug_putc, "P STEP3b %ld %ld\r\n", var1, var2);
   if (var1 == 0)
   {
      return 0; // avoid exception caused by division by zero
   }
   p = (((unsigned int32)(((signed int32)1048576)-adc_P)-(var2/4096)))*3125;
   debug2_bme280(debug_putc, "P STEP4 %lu\r\n", p);
   if (p < 0x80000000)
   {
      p = (p * 2) / ((unsigned int32)var1);
   }
   else
   {
      p = (p / (unsigned int32)var1) * 2;
   }
   debug2_bme280(debug_putc, "P STEP5 %lu\r\n", p);
   var1 = (((signed int32)_g_Bme280.comps.dig_P9) * ((signed int32)(((p/8) * (p/8))/8192)))/4096;
   var2 = (((signed int32)(p/4)) * ((signed int32)_g_Bme280.comps.dig_P8))/8192;
   p = (unsigned int32)((signed int32)p + ((var1 + var2 + (signed int32)_g_Bme280.comps.dig_P7) / 16));
   debug2_bme280(debug_putc, "P STEP6 %ld %ld %lu\r\n", var1, var2, p);
   
   debug_bme280(debug_putc, "_bme280_compensate_P_int32(%ld) %ld\r\n", adc_P, p);
   
   return p;
}

// THIS FUNCTION MOSTLY WRITTEN BY BOSCH, PROVIDED IN THEIR DATASHEETS.
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of "47445" represents 47445/1024 = 46.333 %RH
static unsigned int32 _bme280_compensate_H_int32(signed int32 adc_H)
{
   signed int32 v_x1_u32r;
   unsigned int32 H;
   
   v_x1_u32r = (_g_Bme280.comps.t_fine - ((signed int32)76800));
   
   v_x1_u32r = (((((adc_H * 16384) - (((signed int32)_g_Bme280.comps.dig_H4) * 1048576) - (((signed int32)_g_Bme280.comps.dig_H5) * v_x1_u32r)) +
      ((signed int32)16384)) / 32768) * (((((((v_x1_u32r * ((signed int32)_g_Bme280.comps.dig_H6)) / 1024) * (((v_x1_u32r *
      ((signed int32)_g_Bme280.comps.dig_H3)) / 2048) + ((signed int32)32768))) / 1024) + ((signed int32)2097152)) *
      ((signed int32)_g_Bme280.comps.dig_H2) + 8192) / 16384));
      
   v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r / 32768) * (v_x1_u32r / 32768)) / 128) * ((signed int32)_g_Bme280.comps.dig_H1)) / 16));
   v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
   v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
   
   H = (unsigned int32)(v_x1_u32r/4096);
   
   debug_bme280(debug_putc, "_bme280_compensate_H_int32(%ld) %ld\r\n", adc_H, H);
   
   return H;
}

static unsigned int32 _bme280_get20bits(unsigned int8 *p)
{
   unsigned int32 ret = 0;
   unsigned int8 c;
   
   ret += *p++;
   ret *= 256;
   
   ret += *p++;
   ret *= 16;
   
   c = *p;
   c >>= 4;
   c &= 0xF;
   ret += c;
   
   return(ret);
}

int1 bme280_get_humidity(signed int32 *pTemp, unsigned int32 *pPress, unsigned int32 *pHum)
{
   unsigned int8 read[8];
   unsigned int32 scr32;
   
   debug_bme280(debug_putc, "bme280_get_humidity() ");
   
   if (!bme280_ok())
   {
      debug_bme280(debug_putc, "NOT_INIT\r\n");
      return(FALSE);
   }
   
   if (!_bme280_read_bytes(read, 0xF7, sizeof(read)))
   {
      debug_bme280(debug_putc, "READ_FAIL\r\n");
      return(FALSE);
   }
   
   #if defined(__DO_DEBUG2_BME280)
   _DEBUG_BME280_DISPLAY_TRIMS();
   #endif
   
   debug_bme280(debug_putc, "%X %X %X %X %X %X %X %X\r\n", read[0], read[1], read[2], read[3], read[4], read[5], read[6], read[7]);
   
   scr32 = _bme280_get20bits(&read[3]);   //raw temp adc value
   
   //#warning !! DEBUG VALUE BEING USED
   //scr32 = 532560;
   
   scr32 = _bme280_compensate_T_int32(scr32);
   if (pTemp != 0)   *pTemp = scr32;
   
   if (pPress != 0)
   {
      scr32 = _bme280_get20bits(read); //raw press adc value

      //#warning !! DEBUG VALUE BEING USED
      //scr32 = 329712;
      
      scr32 = _bme280_compensate_P_int32(scr32);
      *pPress = scr32;
   }
   
   if (pHum != 0)
   {
      scr32 = 0;
      scr32 = make16(read[6], read[7]);
      
      //#warning !! DEBUG VALUE BEING USED
      //scr32 = 27227;
      
      scr32 = _bme280_compensate_H_int32(scr32);
      *pHum = scr32;
   }
   
   return(TRUE);
}

int1 bme280_get_temperature(signed int32 *pTemp)
{
   return(bme280_get_humidity(pTemp, (unsigned int32 *)0, (unsigned int32 *)0));
}

int1 bme280_get_pressure(signed int32 *pTemp, unsigned int32 *pPress)
{
   return(bme280_get_humidity(pTemp, pPress, (unsigned int32 *)0));
}

#endif
