///////////////////////////////////////////////////////////////////////////////
//                            ds18b20_multidrop.c                            //
//                                                                           //
// Driver for multiple DS18B20 Digital Thermometers one wire interface.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// API:                                                                      //
//                                                                           //
//    ds18b20_init() - Initialize driver, must be called before any other    //
//                     driver functions.                                     //
//                                                                           //
//    ds18b20_search() - Searches for the ROM codes of DS18B20 sensors       //
//                       connected to the bus, and returns the ROM codes and //
//                       count.  Search type can ROM, return ROM codes and   //
//                       count for all sensors, or ALARM, only return ROM    //
//                       codes count for sensors in alarm.                   //
//                                                                           //
//    ds18b20_search_rom() - Searches for the ROM codes of all DS18B20       //
//                           sensors connected to the bus and returns the    //
//                           ROM codes and count.                            //
//                                                                           //
//    ds18b20_search_alarm() - Searches for the ROM codes of DS18B20 sensors //
//                             connected to the bus that are in alarm, and   //
//                             returns the ROM codes and count.              //
//                                                                           //
//    ds18b20_start_conversion() - Start a temperature conversion.  The only //
//                                 parameter for this function is optional.  //
//                                 If not passed or set to NULL it will      //
//                                 cause all DS18B20 devices on bus to start //
//                                 conversion, otherwise it will only start  //
//                                 the conversion for the device with the    //
//                                 specified ROM code.                       //
//                                                                           //
//    ds18b20_done() - Used do determine if conversion is finished.  Returns //
//                     TRUE when done, FALSE if not done.  Function should   //
//                     not be used when using parasite power, see Parasite   //
//                     Power section for an example of how to perform and    //
//                     read a conversion when using parasite power.          //
//                                                                           //
//   ds18b20_get_conversion() - Returns the conversion result for the device //
//                              with the specified ROM code.  The RAW        //
//                              reading read from sensor is returns, device  //
//                              by 16 to get temperature in Celsius.         //
//                                                                           //
//   ds18b20_read() - Initiates a conversion and reads the result.  The RAW  //
//                    reading read from sensor is returned, divide by 16 to  //
//                    temperature in Celsius.  Warning this function will    //
//                    deadlock the PIC while waiting for the DS18B20 to      //
//                    do a conversion, which can take over 750ms.  To limit  //
//                    the deadlocking, use ds18b20_start_conversion(),       //
//                    ds18b20_done() and ds18b20_get_conversion() instead.   //
//                                                                           //
//    ds18b20_convert_temp() - Converts RAW int16_t reading to float value   //
//                             in degrees Celsius.                           //
//                                                                           //
//    ds18b20_read_temp() - Initiates a conversion and reads the result.     //
//                          The reading is returned as a float value in      //
//                          degrees Celsius.  Warning this function will     //
//                          deadlock the PIC while waiting for the DS18B20   //
//                          to do a conversion, which can take over 750 ms.  //
//                          To limit the deadlocking, use                    //
//                          ds18b20_start_conversion(), ds18b20_done(),      //
//                          ds18b20_get_conversion() and                     //
//                          ds18b20_convert_temp() instead.                  //
//                                                                           //
//    ds18b20_read_config() - Read the TH, TL and resolution from the        //
//                            specified DS18B20 device's EEPROM.             //
//                                                                           //
//    ds18b20_write_config() - Write the TH, TL and resolution to the        //
//                             specified DS18B20 device's EEPROM.            //
//                                                                           //
//    ds18b20_read_scratchpad() - Read data from the specified DS18B20's     //
//                                scratchpad.                                //
//                                                                           //
//    ds18b20_write_scratchpad() - Write 3 bytes of data to the specified    //
//                                 DS18B20's scratchpad.                     //
//                                                                           //
//    ds18b20_parasite_powered() - Used to determine if more of more         //
//                                 DS18B20 devices are parasite powered.     //
//                                                                           //
// See the comments before each function for more info about the parameters  //
// and return values for them.  Additionally there are overloaded version of //
// the ds18b20_read() and ds18b20_read_temp() functions for initiating and   //
// reading more then one device at a time.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Defines:                                                                  //
//                                                                           //
//    PIN_DS18B20_DATA - the PIC pin that is being used for the one wire     //
//                       interface.  Default is PIN_B5 if not defined.       //
//                                                                           //
//    MAX_DS18B20_SENSORS - the max number of sensor connected to one wire   //
//                          interface.  Default is 8 if not defined.         //
//                                                                           //
//    DS18B20_DISABLE_INTS - the timing for the DS18B20 is very precise.  If //
//                           this is defined before this driver is included  //
//                           in the project it will disable and restore the  //
//                           global and peripheral interrupts during time    //
//                           sensitive code.                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Types:                                                                    //
//                                                                           //
//    DS18B20_ROM_CODE - structure use for storing the unique 64-bit ROM     //
//                       code for the DS18B20 sensors.                       //
//                                                                           //
//    SEARCH_TYPE - used for specifying the search type done by the          //
//                  ds18b20_search() function.                               //
//                                                                           //
//    DS18B20_CONFIG - structure used for read/writing the TH, TL and        //
//                     Resolution configuration registers.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Parasite Power:                                                           //
//                                                                           //
// The ds18b20_read() and ds18b20_read_temp() functions can only be used if  //
// not using parasite power mode.  If using parasite power then a strong     //
// pull-up must be enabled within 10us after send the Convert Temperature    //
// command.  The following is an example of initiating a conversion and      //
// reading the results when using parasite power for a single sensor:        //
//                                                                           //
//    ds18b20_start_conversion(&ROMCode);                                    //
//    output_high(PARASITE_POWER_PIN); //level to drive pin depends on       //
//                                     //hardware used.  Needs to be done    //
//                                     //within 10 us.                       //
//    delay_ms(750);                   //delay length depends on sensor      //
//                                     //resolution.                         //
//    output_low(PARASITE_POWER_PIN);  //level to drive pin depends on       //
//                                     //hardware used.                      //
//    ds18b20_get_conversion(&Result, &ROMCode);                             //
//    fTemp = ds18b20_convert_temp(Result);  //for ds18b20_read_temp()       //
//                                                                           //
// Additionally the ds18b20_write_config() function requires a 10 ms delay   //
// after it to write the data to EEPROM.  In using parasite power it is also //
// required to do a strong pull-up within 10us, wait the 10 ms and release   //
// the strong pull-up.                                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Update History:                                                           //
//                                                                           //
//    11/04/20 - Added DS18B20_DISABLE_INTS option to code to disable global //
//               global and peripheral interrupts before executing time      //
//               sensitive code, and to restore them after the code has been //
//               executed.                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//             (C) Copyright 1996, 2020 Custom Computer Services             //
//   This source code may only be used by licensed users of the CCS C        //
//   compiler.  This source code may only be distributed to other licensed   //
//   users of the CCS C compiler.  No other use, reproduction or             //
//   distribution is permitted without written permission.  Derivative       //
//   programs created using this software in object code form are not        //
//   restricted in any way.                                                  //
//                           http://www.ccsinfo.com                          //
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stddef.h>

#ifdef DS18B20_DISABLE_INTS
#include <critical.h>
#endif

#ifndef PIN_DS18B20_DATA
 #define PIN_DS18B20_DATA     PIN_B5
#endif

#ifndef MAX_DS18B20_SENSORS
 #define MAX_DS18B20_SENSORS  8
#endif

/////////////////////////////////// Defines ///////////////////////////////////

#define DS18B20_FAMILY_CODE            0x28

#define DS18B20_CMD_SEARCH_ROM         0xF0
#define DS18B20_CMD_READ_ROM           0x33
#define DS18B20_CMD_MATCH_ROM          0x55
#define DS18B20_CMD_SKIP_ROM           0xCC

#define DS18B20_CMD_ALARM_SEARCH       0xEC
#define DS18B20_CMD_CONVERT_T          0x44
#define DS18B20_CMD_WRITE_SCRATCHPAD   0x4E
#define DS18B20_CMD_READ_SCRATCHPAD    0xBE
#define DS18B20_CMD_COPY_SCRATCHPAD    0x48
#define DS18B20_CMD_RECALL_EE          0xB8
#define DS18B20_CMD_READ_POWER_SUPPLY  0xB4

#define DS18B20_RESOLUTION_9BIT        0x1F
#define DS18B20_RESOLUTION_10BIT       0x3F
#define DS18B20_RESOLUTION_11BIT       0x5F
#define DS18B20_RESOLUTION_12BIT       0x7F

//////////////////////////////////// Types ////////////////////////////////////

#ifdef __PCD__
typedef struct __attribute__((packed))
#else
typedef struct
#endif
{
   uint8_t FamilyCode;
   uint8_t SerialNumber[6];
   uint8_t CRC;
} DS18B20_ROM_CODE;

typedef enum
{
   SEARCH_TYPE_ROM,
   SEARCH_TYPE_ALARM,
} SEARCH_TYPE;

typedef struct
{
   int8_t TH;
   int8_t TL;
   uint8_t Resolution;
} DS18B20_CONFIG;

////////////////////////////////// Prototypes /////////////////////////////////

void ds18b20_init(void);
uint8_t ds18b20_search(DS18B20_ROM_CODE *Serial, uint8_t Max, SEARCH_TYPE Type);
uint8_t ds18b20_search_rom(DS18B20_ROM_CODE *Serial, uint8_t Max = MAX_DS18B20_SENSORS);
uint8_t ds18b20_search_alarm(DS18B20_ROM_CODE *Serial, uint8_t Max = MAX_DS18B20_SENSORS);
int1 ds18b20_start_conversion(DS18B20_ROM_CODE *Serial = NULL);
int1 ds18d20_done(void);
int1 ds18b20_get_conversion(int16_t *pTemp, DS18B20_ROM_CODE *Serial);
int1 ds18b20_read(int16_t *pTemp, DS18B20_ROM_CODE *Serial);
int1 ds18b20_read(int16_t *pTemp, DS18B20_ROM_CODE *ROMCode, uint8_t Count);
float ds18b20_convert_temp(int16_t Reading);
int1 ds18b20_read_temp(float *pTemp, DS18B20_ROM_CODE *Serial);
int1 ds18b20_read_temp(float *pTemp,  DS18B20_ROM_CODE *Serial, uint8_t Count);
int1 ds18b20_read_config(DS18B20_CONFIG *Config, DS18B20_ROM_CODE *Serial);
int1 ds18b20_write_config(DS18B20_CONFIG Config, DS18B20_ROM_CODE *Serial = NULL);
int1 ds18b20_read_scratchpad(uint8_t *Data, DS18B20_ROM_CODE *ROMCode, int8 Count);
int1 ds18b20_write_scratchpad(uint8_t *Data, DS18B20_ROM_CODE *ROMCode);
int1 ds18b20_parasite_powered(void);

int1 ds18b20_start(void);
void ds18b20_write_bit(int1 value);
void ds18b20_write_byte(uint8_t value);
int1 ds18b20_read_bit(void);
uint8_t ds18b20_read_byte(void);

///////////////////////////////////// API /////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// ds18b20_init()
//
// Initializes the data pin used for communicating with DS18B20 sensors.  Mush
// be called before any other DS18B20 function are used.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void ds18b20_init(void)
{
   output_float(PIN_DS18B20_DATA);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_search()
//
// Searches for up to Max number of DS18B20 sensors and returns their unique 64
// bit ROM codes.
//
// Parameters:
//    ROMCode - pointer to DS18B20_ROM_CODE struct to return found ROM codes
//              to.
//
//    Max - Max number of ROM codes to return.
//
//    Type - Search type to perform, can be either SEARCH_TYPE_ROM which
//           returns all DS18B20 ROM codes found, or SEARCH_TYPE_ALARM which
//           returns only ROM codes of DS18B20 sensors that are in alarm.
//
// Returns - number of DS18B20 sensors found.
///////////////////////////////////////////////////////////////////////////////
uint8_t ds18b20_search(DS18B20_ROM_CODE *ROMCode, uint8_t Max, SEARCH_TYPE Type)
{
   uint8_t Count = 0;
   uint8_t i;
   
   int1 id_bit;
   int1 cmp_id_bit;
   int1 search_direction;
   int1 LastDeviceFlag = FALSE;
   uint8_t id_bit_number;
   uint8_t LastDiscrepancy = 0;
   uint8_t last_zero;
   uint8_t ROM_NO[8];
   
   do
   {
      // search rom command
      if(ds18b20_start() != TRUE)
         break;
      
      if(Type == SEARCH_TYPE_ALARM)
         ds18b20_write_byte(DS18B20_CMD_ALARM_SEARCH);
      else
         ds18b20_write_byte(DS18B20_CMD_SEARCH_ROM);

      // read in one entire rom
      for(i=0,last_zero=0,id_bit_number=1;i<64;i++,id_bit_number++)
      {
         id_bit = ds18b20_read_bit();     // read one bit from rom(s)
         cmp_id_bit = ds18b20_read_bit(); // read complement of previous bit from rom(s)

         if((!id_bit) && (!cmp_id_bit))
         {
            if(id_bit_number == LastDiscrepancy)
               search_direction = 1;
            else if(id_bit_number > LastDiscrepancy)
               search_direction = 0;
            else
               search_direction = bit_test(ROM_NO[i/8], i%8);
            
            if(search_direction == 0)
               last_zero = id_bit_number;
         }
         else
            search_direction = id_bit;
         
         if(search_direction)
            bit_set(ROM_NO[i/8], i%8);
         else
            bit_clear(ROM_NO[i/8], i%8);
         
         ds18b20_write_bit(search_direction);
      }
      
      LastDiscrepancy = last_zero;
      
      if(ROM_NO[0] == DS18B20_FAMILY_CODE)
         memcpy(&ROMCode[Count++], ROM_NO, 8);
         
      if((Count >= Max) || (LastDiscrepancy == 0))
         LastDeviceFlag = TRUE;
         
   }while(!LastDeviceFlag);
   
   return(Count);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_search_rom()
//
// Searches for up to Max number of DS18B20 sensors and returns their unique 64
// bit ROM codes.
//
// Parameters:
//    ROMCode - pointer to DS18B20_ROM_CODE struct to return found ROM codes
//              to.
//
//    Max - optional parameter specifying the max number of DS18B20 ROM codes
//          to return.  Default is MAX_DS18B20_SENSORS.
//
// Returns - number of DS18B20 sensors found.
///////////////////////////////////////////////////////////////////////////////
uint8_t ds18b20_search_rom(DS18B20_ROM_CODE *ROMCode, uint8_t Max = MAX_DS18B20_SENSORS)
{
   return(ds18b20_search(ROMCode, Max, SEARCH_TYPE_ROM));
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_search_alarm()
//
// Searches for up to Max number of DS18B20 sensor that are in alarm, and
// returns their unique 64 bit ROM codes.
//
// Parameters:
//    ROMCode - pointer to DS18B20_ROM_CODE struct to return found ROM codes
//              to.
//
//    Max - optional parameter specifying the max number of DS18B20 ROM codes
//          to return.  Default is MAX_DS18B20_SENSORS.
//
// Returns - number of DS18B20 sensors found that are in alarm.
///////////////////////////////////////////////////////////////////////////////
uint8_t ds18b20_search_alarm(DS18B20_ROM_CODE *ROMCode, uint8_t Max = MAX_DS18B20_SENSORS)
{
   return(ds18b20_search(ROMCode, Max, SEARCH_TYPE_ALARM));
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_start_conversion()
//
// Sends command to start temperature conversion.  Depending on resolution of 
// sensor can take up to 750ms to do.
//
// Parameters:
//    ROMCode - optional parameter, pointer to DS18B20_ROM struct containing
//              ROM code of sensor to start conversion.  If NULL is passed
//              as pointer function will use Skip ROM command which will cause
//              all sensors on buss to start a temperature conversion.  Default
//              is NULL.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_start_conversion(DS18B20_ROM_CODE *ROMCode = NULL)
{
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if (!ds18b20_start())
      return(FALSE);
   
   if(ROMCode == NULL)
      ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
   else
   {
      ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
      
      memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
      
      for(i=0;i<8;i++)
         ds18b20_write_byte(ROMCodeArray[i]);
   }
   
   ds18b20_write_byte(DS18B20_CMD_CONVERT_T);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_done()
//
// Determines if DS18B20 is done with previous task.  Can be used after 
// ds18b20_start_conversion() function to determine if temperature conversion 
// is complete.  This function can't be used after ds18b20_start_conversion()
// function call if powering device with parasite power.
//
// Parameters - None.
//
// Returns - TRUE is done, FALSE is busy.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_done(void)
{
   return(ds18b20_read_bit());
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_get_conversion()
//
// Reads the temperature conversion results from DS18B20 scratchpad.  
//
// Parameters:
//    pTemp - pointer to int16_t variable to return result to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct containing ROM code of
//              sensor to read.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_get_conversion(int16_t *pTemp, DS18B20_ROM_CODE *ROMCode)
{
   union 
   {
      uint8_t b[2];
      int16_t w;
   } ret;
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if (!ds18b20_start())
      return(FALSE);
   
   ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
   
   memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
   
   for(i=0;i<8;i++)
      ds18b20_write_byte(ROMCodeArray[i]);
   
   ds18b20_write_byte(DS18B20_CMD_READ_SCRATCHPAD);
   ret.b[0] = ds18b20_read_byte();
   ret.b[1] = ds18b20_read_byte();
   
   *pTemp = ret.w;
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read()
//
// Initiates a conversion and reads the result of a single DS18B20 sensor.
// This function can't be used if powering device with parasite power.
//
// Parameters:
//    pTemp - pointer to int16_t variable to return reading to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct variable containing ROM code
//              of sensor to read.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read(int16_t *pTemp, DS18B20_ROM_CODE *ROMCode)
{
   if (!ds18b20_start_conversion(ROMCode))
      return(FALSE);
      
   while(!ds18b20_done());
      
   return(ds18b20_get_conversion(pTemp, ROMCode));
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read()
//
// Overloaded version of ds18b20_read() function that initiates a conversion
// and reads the result from Count number of DS18B20 sensors.  This function
// can't be used if powering devices with parasite power.
//
// Parameters:
//    pTemp - pointer to an int16_t array to return readings to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct array containing ROM codes
//              of sensors to read.
//
//    Count - number of DS18B20 sensors to read.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read(int16_t *pTemp, DS18B20_ROM_CODE *ROMCode, uint8_t Count)
{
   uint8_t i;
   
   if(Count > 0)
   {
      if (!ds18b20_start_conversion())
         return(FALSE);
         
      while(!ds18b20_done());
      
      for(i=0;i<Count;i++)
      {
         if(!ds18b20_get_conversion(&pTemp[i], &ROMCode[i]))
            return(FALSE);
      }
   }
   else
      return(FALSE);
      
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_convert_temp()
//
// Converts int16_t reading from sensor to a float value in degrees Celsius.
//
// Parameters:
//    Reading - int16_t value read from senor.
//
// Returns - float value of reading in degrees Celsius.
///////////////////////////////////////////////////////////////////////////////
float ds18b20_convert_temp(int16_t Reading)
{
   return((float)Reading / 16);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_temp()
// 
// Initiates a conversion and reads the result of a single DS18B20 sensor, 
// similar to ds18b20_read() function except that result is returned as a float
// value with temperature as degrees Celsius.  This function can't be used if
// powering device with parasite power.
//
// Parameters:
//    pTemp - pointer to a float variable to return reading to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct variable containing ROM code
//              of sensor to read.
//
// 
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read_temp(float *pTemp, DS18B20_ROM_CODE *ROMCode)
{
   int16_t Reading;
   
   if(!ds18b20_read(&Reading, ROMCode))
      return(FALSE);
   
   *pTemp = ds18b20_convert_temp(Reading);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_temp()
//
// Overloaded version of ds18b20_read_temp() function that initiates a 
// conversion and reads the result from Count number of DS18B20 sensors.  This
// function can't be used if powering devices with parasite power.
//
// Parameters:
//    pTemp - pointer to a float array to return readings to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct array containing ROM codes
//              of sensors to read.
//
//    Count - number of DS18B20 sensors to read.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read_temp(float *pTemp, DS18B20_ROM_CODE *ROMCode, uint8_t Count)
{
   int16_t Reading;
   uint8_t i;
   
   if(Count > 0)
   {
      if(!ds18b20_start_conversion())
         return(FALSE);
      
      while(!ds18b20_done());
      
      for(i=0;i<Count;i++)
      {
         if(!ds18b20_get_conversion(&Reading, &ROMCode[i]))
            return(FALSE);
         
         pTemp[i] = ds18b20_convert_temp(Reading);
      }
   }
   else
      return(FALSE);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_config()
//
// Reads the alarm trigger values (TH and TL) and resolution data from EEPROM.
// First values are copied to bytes 2, 3 and 4, respectively, in devices's
// scratchpad and then they are read from scratchpad.
//
// Parameters:
//    Config - pointer to DS18B20_CONFIG struct variable to return the TH, TL
//             and Resolution read from sensor.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct variable containing ROM code
//              of sensor to read.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read_config(DS18B20_CONFIG *Config, DS18B20_ROM_CODE *ROMCode)
{
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if(!ds18b20_start())
      return(FALSE);
   
   ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
   
   memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
   
   for(i=0;i<8;i++)
      ds18b20_write_byte(ROMCodeArray[i]);
   
   ds18b20_write_byte(DS18B20_CMD_RECALL_EE);
   
   while(!ds18b20_done());
   
   if(!ds18b20_start())
      return(FALSE);
   
   ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
   
   memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
   
   for(i=0;i<8;i++)
      ds18b20_write_byte(ROMCodeArray[i]);
   
   ds18b20_write_byte(DS18B20_CMD_READ_SCRATCHPAD);
   ds18b20_read_byte();
   ds18b20_read_byte();
   Config->TH = ds18b20_read_byte();
   Config->TL = ds18b20_read_byte();
   Config->Resolution = ds18b20_read_byte();
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_write_config()
//
// Writes the alarm trigger values (TH and TL) and resolution to sensor's
// EEPROM.  First values are written to bytes 2, 3 and 4, respectively, of
// devices scratchpad and then they are written to EEPROM.  A 10 ms delay is
// required after this function is called to write data to EEPROM.  This 
// function doesn't do delay incase parasite power is used, in that case it
// is also required to do a strong pull-up on data line with in 10 us of
// command, wait 10ms and then release the strong pull-up.
// 
// Parameters:
//    Config - DS18B20_CONFIG variable containing TH, TL and resolution to 
//             write to sensor.
//
//    ROMCode - optional parameter, pointer to DS18B20_ROM_CODE struct
//              containing ROM code of sensor to write.  If NULL the Skip
//              ROM command will be used and value will be written to all
//              sensor on bus.  Default is NULL.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_write_config(DS18B20_CONFIG Config, DS18B20_ROM_CODE *ROMCode = NULL)
{
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if(!ds18b20_start())
      return(FALSE);
   
   if(ROMCode == NULL)
      ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
   else
   {
      ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
      
      memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
      
      for(i=0;i<8;i++)
         ds18b20_write_byte(ROMCodeArray[i]);
   }
   
   ds18b20_write_byte(DS18B20_CMD_WRITE_SCRATCHPAD);
   ds18b20_write_byte(Config.TH);
   ds18b20_write_byte(Config.TL);
   ds18b20_write_byte(Config.Resolution);
   
   if(!ds18b20_start())
      return(FALSE);
   
   if(ROMCode == NULL)
      ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
   else
   {
      ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
      
      memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
      
      for(i=0;i<8;i++)
         ds18b20_write_byte(ROMCodeArray[i]);
   }
   
   ds18b20_write_byte(DS18B20_CMD_COPY_SCRATCHPAD);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_scratchpad()
//
// Reads 1 to 9 bytes from sensors scratchpad.
//
// Parameters:
//    Data - pointer to uint8_t array to return read data to.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct variable containing ROM code
//              of sensor to read from.
//
//    Count - number of bytes to read (1-9).
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read_scratchpad(uint8_t *Data, DS18B20_ROM_CODE *ROMCode, int8 Count)
{
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if(!ds18b20_start())
      return(FALSE);
   
   if(Count > 0)
   {
      ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
         
      memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
      
      for(i=0;i<8;i++)
         ds18b20_write_byte(ROMCodeArray[i]);
         
      for(i=0;i<Count;i++)
         Data[i] = ds18b20_read_byte();
   }
   else
      return(FALSE);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_write_scratchpad()
//
// Write 3 bytes to sensors scratchpad.
//
// Parameters:
//    Data - pointer to uint8_t array containing 3 bytes to write.
//
//    ROMCode - pointer to DS18B20_ROM_CODE struct variable containing ROM code
//              of sensor to write to.
//
// Returns - TRUE if successful, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_write_scratchpad(uint8_t *Data, DS18B20_ROM_CODE *ROMCode)
{
   uint8_t i;
   uint8_t ROMCodeArray[8];
   
   if(!ds18b20_start())
      return(FALSE);
   
   ds18b20_write_byte(DS18B20_CMD_MATCH_ROM);
      
   memcpy(ROMCodeArray, ROMCode, sizeof(DS18B20_ROM_CODE));
   
   for(i=0;i<8;i++)
      ds18b20_write_byte(ROMCodeArray[i]);
      
   ds18b20_write_byte(DS18B20_CMD_WRITE_SCRATCHPAD);
   
   for(i=0;i<3;i++)
      ds18b20_write_byte(Data[i]);
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_parasite_powered()
//
// Used to determine if one or more of the DS18B20 devices on the bus is 
// parasite powered.
//
// Parameters - None.
//    
// Returns - TRUE if one or more DS18B20 devices are parasite powered, FALSE if 
//           no DS18B20 devices are parasite powered.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_parasite_powered(void)
{
   int1 Result;
   
   ds18b20_start();
   
   ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
   
   ds18b20_write_byte(DS18B20_CMD_READ_POWER_SUPPLY);
   
   Result = ds18b20_read_bit();
   
   return(Result == 0);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_start()
//
// Initializes the DS18B20 sensors for a transaction to occur.
//
// Parameters - None.
//
// Returns: TRUE if start pulse response was read, FALSE if an error occurred.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_start(void)
{
   int1 ret = FALSE;
  
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_ENTER();
  #endif
   
   output_low(PIN_DS18B20_DATA);
   delay_us(500);
   output_float(PIN_DS18B20_DATA);
   delay_us(70);//wait to read the SLAVES response
   if (!input(PIN_DS18B20_DATA))
   {
      ret = TRUE;
      delay_us(430);
   }
   
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_EXIT();
  #endif
   
   return(ret);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_write_bit()
//
// Initiates a write time slot and writes the bit to the DS18B20 sensors.
//
// Parameters:
//    value - int1 value to write.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void ds18b20_write_bit(int1 value)
{
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_ENTER();
  #endif
  
   output_low(PIN_DS18B20_DATA);
   delay_us(1);
   output_bit(PIN_DS18B20_DATA,value);
   delay_us(59);
   output_float(PIN_DS18B20_DATA);
   delay_us(1);
   
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_EXIT();
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_write_byte()
//
// Writes a byte to the DS18B20 sensors.
//
// Parameters:
//    value - uint8_t value to write.
//
// Return - Nothing.
///////////////////////////////////////////////////////////////////////////////
void ds18b20_write_byte(uint8_t value)
{
   uint8_t i;
   
   for(i=1;i<=8;++i)
   {
      ds18b20_write_bit(shift_right(&value,1,0));
   }
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_bit()
//
// Initiates a read time slot and write a bit to the DS18B20 sensors.
//
// Parameters - None.
//
// Returns - int1 value read.
///////////////////////////////////////////////////////////////////////////////
int1 ds18b20_read_bit(void)
{
   int1 value;
   
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_ENTER();
  #endif
   
   output_low(PIN_DS18B20_DATA);
   delay_us(1);
   output_float(PIN_DS18B20_DATA);
   delay_us(11);
   value=input(PIN_DS18B20_DATA);
   delay_us(49);
   
  #ifdef DS18B20_DISABLE_INTS
   CRITICAL_SECTION_EXIT();
  #endif
   
   return(value);
}

///////////////////////////////////////////////////////////////////////////////
// ds18b20_read_byte()
//
// Read a byte from the DS18B20 sensors.
//
// Parameters - None.
//
// Return - uint8_t value read.
///////////////////////////////////////////////////////////////////////////////
uint8_t ds18b20_read_byte(void)
{
   uint8_t i, value;
   
   for(i=0; i<8; i++)
   {
      shift_right(&value,1,ds18b20_read_bit());
   }
   
   return(value);
}
