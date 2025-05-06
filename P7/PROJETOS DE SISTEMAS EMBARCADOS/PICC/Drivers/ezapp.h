/**
 * \file EZApp.h
 *
 * Public prototypes and definitions for the EZ App Lynx library.
 *
 * The EZ App Lynx library allows the user to quickly and easily create a
 * GUI on Android and iOS smartphone/tablets.  The Android/iOS app acts
 * as an empty slate, and at run time this library tells the Android/iOS
 * app what to look like over Bluetooth.  This also speeds up deployment, as 
 * the associated smartphone/tablet app is already on the appropiate app store.
 *
 * Use EZAppAddField***() functions to add elements onto the GUI.  Elements
 * are added to the GUI in the order that EZAppAddField***() are called.
 * EZAppAddField***() returns an index that can be used on associated get
 * (EZAppGet***()) and set (EZAppSet***() functions.
 *
 * Use EZAppSet***() functions to have the PICmicro set any dynamic
 * values associated with the GUI element.
 *
 * Use EZAppGetKbhit() and EZAppGet***() to detect value changes from
 * smartphone and read the new value.
 *
 *
 * BLUETOOTH
 * -----------------------------------------------------------------------
 * A Bluetooth SPP connection is required between the PIC and the smartphone.
 * CCS provides a driver for RN42 Bluetooth SPP module that is compatible
 * with the EZ APP Lynx library, see RN42_Bluetooth.c.  If you want to support
 * different Bluetooth modules then you must develop your own library that
 * is compatible with the RN42_Bluetooth.c file.
 *
 *
 * CONFIG
 * -----------------------------------------------------------------------
 * There are a few config values that can be #define'd to configure the
 * library.  They are summarized here, for more information about each
 * look at the comments above their definition.
 *
 * EZAPP_MAX_STRING_SIZE - The maximum string size that can be sent by
 *    the smartphone/tablet host.  This includes strings used by the
 *    authentication/password via EZAPP_CHECK_AUTHENTICATION() or strings that 
 *    are editable via EZAppAddFieldText().
 *
 * EZAPP_STATIC_MEMORY_ALLOCATION - If this isn't defined, all the
 *    GUI elements handled by the PICmicro is done dynamically with malloc().
 *    If you know exactly how many GUI elements you are using, you can
 *    #define EZAPP_STATIC_MEMORY_ALLOCATION to this value and the library
 *    will not use malloc(), and instead allocate the needed memory at
 *    compile time.  Doing it statically will reduce ROM usage.  You will
 *    need to define this as one larger than the number of elements you are
 *    using, because the title of the app uses one element.  Each time you
 *    call EZAppAddField***() is one element.
 *
 * EZAPP_TICK_TYPE, EZAPP_TICKGET(), EZAPP_TICKS_PER_SECOND - define all of 
 *    these of these to enable an internal timeout mechanism inside EZAppTask() 
 *    to reset the Bluetooth module if it's connected to a host but hasn't 
 *    received a packet in over 8 seconds.  EZAPP_TICKGET() returns a time 
 *    that increments at a EZAPP_TICKS_PER_SECOND rate, and EZAPP_TICKGET()
 *    returns a data value of type EZAPP_TICK_TYPE.  This is optional.
 *
 * EZAPP_EEPROM_SIZE - if you are using any data stored in EE, this
 *    macro should return the size of the EEPROM in bytes.
 *    If not provided, the EZ App Lynx library assumes you are using the
 *    PIC's internal EEPROM and will automatically define this value based
 *    on the size of the PIC's internal EEPROM.  An example:
 *       #define EZAPP_EEPROM_SIZE 1024
 *
 * EZAPP_EEPROM_ADDRESS - if you are using any data stored in EE, this
 *    macro should return the integer type for holding an EEPROM address.
 *    If not provided, the EZ App Lynx library assumes you are using the
 *    PIC's internal EEPROM and will automatically define this value based
 *    on the size of the PIC's internal EEPROM.  An example:
 *       #define EZAPP_EEPROM_ADDRESS unsigned int8
 *
 *
 * CALLBACKS
 * -----------------------------------------------------------------------
 * There are some optional callback macros the user can define to extend
 * the operation of the EZ App Lynx library.  These callbacks will be
 * summarized here, but for more detail look below at their definition.
 *
 * EZAPP_IS_CONNECTED() - Called by the library each time a packet is received
 *    from a smartphone host.  
 *
 * int1 EZAPP_CHECK_AUTHENTICATION(char * password) - Called by the library when
 *    the smartphone host is trying to login with a password.  This callback
 *    should return TRUE if 'password' is valid, and then call 
 *    EZAppSetAuthLevel() to change the auth level of the host.
 *
 * EZAPP_EEPROM_WRITE(address, byte) - If EZ App is using memory stored in EE,
 *    this macro must be provided by the user to write one byte to the EE.
 *    If this isn't provided, the EZ App Lynx will use the CCS C Compiler's
 *    EEPROM library for writing.  Example:
 *       #define EZAPP_EEPROM_WRITE(a, b) write_eeprom(a, b)
 *
 * byte = EZAPP_EEPROM_READ(address) - If EZ App is using memory stored in EE,
 *    this macro must be provided by the user to read one byte from the EE.
 *    If this isn't provided, the EZ App Lynx will use the CCS C Compiler's
 *    EEPROM library for writing.  Example:
 *       #define EZAPP_EEPROM_READ(a) read_eeprom(a)
 *
 *
 * EXAMPLES
 * -----------------------------------------------------------------------
 * Several examples are provided for using this library:
 *
 * ex_ezapp_pot.c - A simple demo that has one GUI element displayed on the
 *    screen, a gas gauge showing an ADC value.
 *
 * ex_ezapp_gui.c - A complicated demo that shows many different GUI elements
 *    that can be used by the EZ App Lynx library.
 *
 * ex_ezapp_security.c - Shows how to use password protection.
 *
 * ex_ezapp_dual.c - Shows how to clear the screen and change the GUI
 *    elements on the screen for developing multiple screen applications.
 *
 * ex_ezapp_strings.c - Shows how to use text fields, including text
 *    fields that can be edited by the host smartphone/tablet.
 *
 * ex_ezapp_buttons.c - An example showing how to use buttons.
 *
 * ex_ezapp_style.c - An example showing how to apply styles
 *    to alter how contents in the app look.
 *
 * ex_ezapp_graphing.c - Shows how to add a line and bar graph with one or
 *    more graph data series.
 *
 * ex_ezapp_images.c - An example showing how to add images from the web
 *    or app bundle, as well as use them as buttons.
 *
 * ex_ezapp_colorpicker.c - An example showing how to add a color picker
 *    and how to get/set the color.
 *
 * ex_ezapp_backgrounds.c - An example showing how to add an app background
 *    image and button background images.
 *
 * ex_ezapp_joystick.c - An example showing how to add a joystick and how
 *    to get the Angle/Strength values.
 *
 * VERSION HISTORY
 * -----------------------------------------------------------------------
 * 
 * V4.00 - September 1st, 2017
 *    Added three new Style types.
 *       EZAPP_STYLE_ROW_COLORS_EX_CREATE() allows for setting 32-bit ARGB
 *          colors using EZAppCreateARGB(), where the 4th byte is an alpha
 *          value for adding transparency.
 *       EZAPP_STYLE_BUTTON_EXTRA_CREATE() allows for setting extra button
 *          style configurations, such as text position, max height, and images
 *       EZAPP_STYLE_BACKGROUND_CREATE() allows for setting a background image
 *          on the app.
 *    Added EZAppAddFieldColorPicker() for adding a color picker field/dialog
 *    Added EZAppAddFieldJoystick() for adding a joystick
 *    Added new examples: ex_ezapp_colorpicker.c, ex_ezapp_backgrounds.c,
 *                        ex_ezapp_joystick.c
 *
 * V3.00 - March 14th, 2017
 *    Added EZAppAddFieldGraph() and EZAppAddFieldGraphSeries() for
 *       adding line and bar graphs. Contains options for line style, color,
 *       axis labels and titles, scrolling/scaling, and multiple data series.
 *    Added EZAppAddFieldImage() for adding images from URLs or using some
 *       application images. Images can also be used as buttons.
 *    Added new examples: ex_ezapp_graphing.c, and ex_ezapp_images.c
 *
 * V2.10 - June 29th, 2015
 *    Added Microchip XC8, XC16 and XC32 compiler support.
 *
 * V2.00 - January 22nd 2015
 *    EZApp.c library can use the RN4020_MLDP_Bluetooth.c driver
 *       provided by CCS, so an RN4020 BLE Bluetooth module can
 *       be used to communicate with the smartphone/tablet host
 *       using the MLDP protocol.
 *    Added new examples: ex_ezapp_strings.c, ex_ezapp_style.c
 *       and ex_ezapp_buttons.c
 *    Added EZAppAddStylesROM() for applying styles to the content.
 *       Styles contain things like color, text size, spacing, etc.
 *       See the comments above EZAppAddStylesROM() in EZApp.h for
 *       for more information.
 *    Added EZAppAddFieldButtons(), and EZAppAddFieldButtonsROM().
 *       Unlike the older, original EZAppAddFieldButtonTwoState(), the new 
 *       functions have more settings, such as: number of buttons on the row, 
 *       one-state or two-state buttons, status LED inside the button.  If the 
 *       button has inset status LEDs, EZAppSetButtonLED() can be used to 
 *       control their states.
 *       See the comments above EZAppAddFieldButtons() in EZApp.h for
 *       for more information.
 *    Added EZAppAddFieldText(), which allows for a text field with
 *       a left header colum on the row or a text field that is editable.
 *       EZAppGetValueString() can be used to read a new string sent
 *       by the smartphone/tablet host.
 *       See the comments above EZAppFieldText() in EZApp.h for
 *       for more information.
 *    Added EZAppSetValueStringEE(), to add strings that are held
 *       in the EEPROM.  Use needs to provide EZAPP_EEPROM_ADDRESS,
 *       EZAPP_EEPROM_WRITE() and EZAPP_EEPROM_READ() macros - if not
 *       provided the library will use the CCS C Compiler's default eeprom
 *       routines.  Due to these user defined macros that are called
 *       by EZAppSetValueStringEE(), it is possible to have the strings
 *       held in any memory location.  If the string is editable, it will
 *       write the string back to EE.  EZAppGetValueStringEE() added
 *       to get any changes made by the host smartphone/tablet.
 *    Added EZAppAddFieldAnalogValueScaled().  It is similar to 
 *       EZAppAddFieldAnalogValue() but has a means of controlling the way
 *       data is scaled before being displayed on the host smartphone/tablet.
 *    EZAPP_MAX_PASSWORD_SIZE changed to EZAPP_MAX_STRING_SIZE, to denote that
 *       this field is used for both password and user editable strings.  If
 *       you have old code using EZAPP_MAX_PASSWORD_SIZE, then it will still
 *       use that value.
 *
 * V1.00 - September 5th 2014
 *    Initial release
 */
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
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

#ifndef __EZAPP_H__
#define __EZAPP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//////////////////////////////////////////////////
/// See the comments above.  Defines maximum string size that can be
/// received.
//////////////////////////////////////////////////
#ifndef EZAPP_MAX_STRING_SIZE
   #if defined(EZAPP_MAX_PASSWORD_SIZE)
      // this was originally defined  as EZAPP_MAX_PASSWORD_SIZE, if that's
      // defined by the user then use it.
      #define EZAPP_MAX_STRING_SIZE EZAPP_MAX_PASSWORD_SIZE
   #else
      #define EZAPP_MAX_STRING_SIZE  32
   #endif
#endif

#if (EZAPP_MAX_STRING_SIZE<16)
   #error make the buffer at least 16 bytes large
#endif

#if defined(__PCD__) || defined(__XC16__) || defined(__XC32__)
   #define __EZAPP_PACKED  __attribute__((packed))
#else
   #define __EZAPP_PACKED
#endif

#ifndef EZAPP_EEPROM_SIZE
   #if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
      // CCS C Compiler
      #define EZAPP_EEPROM_SIZE getenv("DATA_EEPROM")
   #elif defined(__XC8)
      // XC8 C Compiler
      #define EZAPP_EEPROM_SIZE _EEPROMSIZE
   #else
      #define EZAPP_EEPROM_SIZE 0
   #endif
#endif

#ifndef EZAPP_EEPROM_ADDRESS
   #if (EZAPP_EEPROM_SIZE >= 0x100)
      #define EZAPP_EEPROM_ADDRESS  uint16_t
   #else
      #define EZAPP_EEPROM_ADDRESS  uint8_t
   #endif
#endif

//////////////////////////////////////////////////
/// After calling a EZAppAdd***() function (like EZAppAddFieldString()),
/// this type will be returned.  This type can then be passed to 
/// EZAppGetValue(), EZAppGetKbhit(), EZAppSetValueString(), EZAppSetValue()
/// or any other function requiring an index for which GUI element to control.
/// If there wasn't enough memory to add this GUI element the EZAppAdd***() 
/// function instead will return EZAPP_FIELD_INDEX_INVALID.
//////////////////////////////////////////////////
typedef uint8_t ezapp_field_index_t;

//////////////////////////////////////////////////
/// If there wasn't enough memory to add a new GUI element, the 
/// EZAppAdd***() function (like EZAppAddFieldString()) will
/// return this value to signify the error.
//////////////////////////////////////////////////
#define EZAPP_FIELD_INDEX_INVALID  (-1)

//////////////////////////////////////////////////
/// Initializes the bluetooth module and library for use.
///
/// If you are using dynamic memory (EZAPP_STATIC_MEMORY_ALLOCATION
/// isn't defined), if you call this after you use any EZAppAdd***() functions
/// (like EZAppAddFieldString()) you will cause a memory leak.  If you
/// already called an EZAppAdd***() then call EZAppFieldsClearAll() before
/// EZAppInit() to free() all the allocated memory.
//////////////////////////////////////////////////
void EZAppInit(void);

//////////////////////////////////////////////////
/// Checks if there have been any requests from  
/// the smartphone device and runs the methods that 
/// corespond to that request.                   
//////////////////////////////////////////////////
void EZAppTask(void);

//////////////////////////////////////////////////
/// Returns TRUE if a smartphone/tablet host is communicating correctly with
/// the library.  This is different than BtSppIsConnected(), as 
/// BtSppIsConnected() returns TRUE if any device is connected wheras
/// EZAppIsConnected() returns TRUE if the host app is connected and running
/// correctly.  If EZAPP_TICK_TYPE, EZAPP_TICKGET() and 
/// EZAPP_TICKS_PER_SECOND are all not defined this will stay TRUE once
/// a connection has been heard but it won't switch to FALSE if the app
/// has left/stopped communicating (the EZAPP_TICK_TYPE adds the timeout
/// mechanism needed to tell if the host has disappeared/lost connection).
//////////////////////////////////////////////////
bool EZAppIsConnected(void);

//////////////////////////////////////////////////
/// Clears the screen of all GUI elements.  You can add then add new GUI
/// elements with the EZAppAdd***() functions.  This is useful for devloping
/// multi-screen GUIs.
//////////////////////////////////////////////////
void EZAppFieldsClearAll(void);

//////////////////////////////////////////////////
/// The authorization levels of the PICmicro, to prevent
/// unauthorized access from a smartphone app.
//////////////////////////////////////////////////
typedef enum
{
   /// guest can read/write
   EZAPP_AUTH_OPEN = 0,
   
   /// guest can read
   EZAPP_AUTH_READ_ONLY = 1, 
   
   /// guest cannot do anything
   EZAPP_AUTH_LOCKED = 2      
} ezapp_authlevel_t;

//////////////////////////////////////////////////
/// Callback: Authorization check
///
/// If you want to support authentication/password,
/// then you must define this macro to point to a function
/// that handles the check.
///
/// The format for the macro:
///   EZAPP_CHECK_AUTHENTICATION(str)
///
/// The format of the function that accepts this macro should be:
///   int1 function_name(char *str);
///
///   'str' is the new authorization string sent by the smartphone app.
///
///   Function should return TRUE if the password was a known password,
///   FALSE if it is not a valid passowrd.
///
/// In your macro/function, call EZAppSetAuthLevel() to set the new
/// auth level based on this check.  Your software can also call
/// EZAppSetAuthLevel() after a set time as a timeout mechanism (timeout
/// is not handled automatically by the library).
////////////////////////////////////////////////////
#ifndef EZAPP_CHECK_AUTHENTICATION
#define EZAPP_CHECK_AUTHENTICATION(str)   (true)
#endif

//////////////////////////////////////////////////
/// Set the authorization level of the PICmicro, to prevent unauthorized
/// access from the smartphone.  See ezapp_authlevel_t for valid values.
//////////////////////////////////////////////////
void EZAppSetAuthLevel(ezapp_authlevel_t level);

//////////////////////////////////////////////////
/// Sets the application title String to the     
/// value of 'prTitle'. The application title is 
/// always stored at index 0.                    
//////////////////////////////////////////////////
void EZAppSetTitle(char * prTitle);
void EZAppSetTitleROM(rom char * prTitle);

//////////////////////////////////////////////////
/// Returns TRUE if the host smartphone app changed
/// the value of this dynamic field.  Calling EZAppGetValue() will clear this 
/// flag until the next time smartphone writes to the field.
//////////////////////////////////////////////////
bool EZAppGetKbhit(ezapp_field_index_t index);

//////////////////////////////////////////////////
/// Creates a string. 'rgb' sets the color of the text (you can use 
/// EZAppCreateRGB() to create value).  'size' increases or decreases the
/// size of the text.  'size' is a signed value, a negative value is used
/// to decrease the size of the text.  Using a value of '0' for size will
/// use the default font size.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldStringDynamic(uint32_t rgb, int8_t size);

//////////////////////////////////////////////////
/// Similar to EZAppAddFieldStringDynamic(), but uses the default
/// font text and color.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldString(void);

//////////////////////////////////////////////////
/// This is very similar to EZAppAddFieldString(), but this allows
/// the user to add a header column string that goes to the left of the
/// string or to make an editable text field.
/// EZAppAddFieldString() is essentially EZAppAddFieldText(NULL, 0)
/// 'pHeader' is the string to go in the left column, set to NULL if you 
/// don't want a left column header.  
/// 'editSize' is the maximum size of characters the PIC can receive from
/// an tablet/smartphone host, leave it to 0 if the string is not editable.
/// If the text is editable, 'editSize' should be the size of memory allocated
/// to the string passed to this field with EZAppSetValueString().
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldText(rom char * pHeader, uint8_t editSize);

///////////////////////////////////////////////////
/// After a string/text field has been created with EZAppAddFieldString()
/// or EZAppAddFieldText(), this function can be used to alter the 
/// style of this string.  This must be called before EZAppTask() is called.
/// 'rgb' sets the color of the text (you can use 
/// EZAppCreateRGB() to create value).  'size' increases or decreases the
/// size of the text.  'size' is a signed value, a negative value is used
/// to decrease the size of the text.  Using a value of '0' for size will
/// use the default font size.
/// EZAppAddStylesROM() is also available for applying styles.
/////////////////////////////////////////////////////
void EZAppSetStringStyle(ezapp_field_index_t idx, uint32_t rgb, int8_t size);

//////////////////////////////////////////////////
/// Sets the String value of String field at     
/// 'index' to the value of '*pString'. This version uses a RAM pointer for
/// the string, for a ROM version see EZAppSetValueStringROM().  For an EE
/// version, see EZAppSetValueStringEE().  The library will not make a copy of 
/// the contents of 'pString', so the user needs to make sure the contents 
/// always stays allocated (perhaps by placing the string in a globally or 
/// statically allocated string).  If the text is editable (smartphone/host 
/// can send the PIC new strings), make sure the 'editSize' parameter of 
/// EZAppAddFieldText() is the total size allocated for pString.
/// After this is called, the smartphone host/table will download the new
/// string.
//////////////////////////////////////////////////
void EZAppSetValueString(ezapp_field_index_t index, char *pString);

//////////////////////////////////////////////////
/// Sets the String value of String field at     
/// 'index' to the value of '*prString'.  This version uses a ROM pointer,
/// for a RAM version see EZAppSetValueString().  For an EE version, 
/// see EZAppSetValueStringEE().
/// After this is called, the smartphone host/table will download the new
/// string.
//////////////////////////////////////////////////
void EZAppSetValueStringROM(ezapp_field_index_t index, rom char *prString);

//////////////////////////////////////////////////
/// Sets the string value of string filed at 'index' to use the contents
/// of the data in EEPROM address 'address'.  For a RAM version see 
/// EZAppSetValueString().  For a ROM version, EZAppSetValueStringROM().
/// Will use the macros EZAPP_EEPROM_ADDRESS, EZAPP_EEPROM_WRITE() and 
/// EZAPP_EEPROM_READ().
/// After this is called, the smartphone host/table will download the new
/// string.
//////////////////////////////////////////////////
#if (EZAPP_EEPROM_SIZE > 0)
void EZAppSetValueStringEE(ezapp_field_index_t index, EZAPP_EEPROM_ADDRESS address);
#endif

//////////////////////////////////////////////////
/// Returns a pointer to the string added by EZAppSetValueString().
/// This will primarily be used if the string is editable and 
/// you want to read the string after the host smartphone/tablet
/// changed the value of the string.  Calling this function
/// will also clear the EZAppGetKbhit() status for this index
/// in the event that EZAppGetKbhit() is returning true because
/// the host changed the string.
/// Will return NULL if there is a problem with the field (invalid
/// index, or string is read-only in ROM, etc).
//////////////////////////////////////////////////
char * EZAppGetValueString(ezapp_field_index_t index);

//////////////////////////////////////////////////
/// Returns a EEPROM address to the string added by EZAppGetValueStringEE().
/// This will primarily be used if the string is editable and 
/// you want to read the string after the host smartphone/tablet
/// changed the value of the string.  Calling this function
/// will also clear the EZAppGetKbhit() status for this index
/// in the event that EZAppGetKbhit() is returning true because
/// the host changed the string.
/// Will return -1 if there is a problem with the field (invalid
/// index, or string is read-only in ROM, etc).
//////////////////////////////////////////////////
#if (EZAPP_EEPROM_SIZE > 0)
EZAPP_EEPROM_ADDRESS EZAppGetValueStringEE(ezapp_field_index_t index);
#endif

typedef enum
{
   EZAPP_ANALOG_TYPE_SLIDER = 0,
      ///< Read only analog signal, Slider representation
      
   EZAPP_ANALOG_TYPE_TEXT_VALUE = 1,
      ///< Read only analog signal, String representation
      
   EZAPP_ANALOG_TYPE_SLIDER_RW = 2,
      ///< R/W analog signal,  Slider representation
      
   EZAPP_ANALOG_TYPE_RW_TEXT_VALUE = 3,
      ///< R/W analog signal, String representation
      
   EZAPP_ANALOG_TYPE_GAUGE = 4,                  
      ///< Read only analog signal, Gauge representation
      
   EZAPP_ANALOG_TYPE_SPINNER = 5
      ///< Spinner (pull-down) Representation.  You have more options using EZAppAddFieldPulldownValue() for this type.} ezapp_analog_type_t;
} ezapp_analog_type_t;

//////////////////////////////////////////////////
/// Creates a field of type Analog.  'prLabel' is the text label for this
/// element.  'analogType' is the display/edit type of this analog value, see
/// ezapp_analog_type_t for list of valid types and descriptions.  'maxValue'
/// is the max value to/from the PICmicro.  'scaling' is a scaling factor
/// used on the smartphone app and represents what is displayed to the user
/// if the value is 'maxValue'.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldAnalogValue(rom char * prLabel,
   ezapp_analog_type_t analogType, 
   uint16_t maxValue);

//////////////////////////////////////////////////
/// This is very similar to EZAppAddFieldAnalogValue(), so for the
/// 'prLablel', 'analogType' and 'maxValue' fields refer to the documentation
/// of EZAppAddFieldAnalogValue() for those values.
/// The difference of this function to EZAppAddFieldAnalogValue() is that
/// this function configures the analog value to be scaled when received
/// by the smartphone/tablet host.
///
/// Scaling will be performed where the host will display 'scaling' when
/// the value is 'maxValue'.  
///
/// 'decimalPlaces' configures 'scaling':
///    0 - 'scaling' is 1:1, but host will display 1 decimal point maximum
///    1 - 'scaling' is in tenths, host will display 1 decimal point maximum
///    2 - 'scaling' is in hundredths, host will display 2 decimal points max
///    3 - 'scaling' is in thousandths, host will display 3 decimal points max
///
/// Set scaling to '0' to disable all scaling and prevent smartphone/tablet host
///   from displaying decimal points.
///
/// An example:
///   'maxValue' is 1024, 'scaling' is 330, 'decimalPlaces' is 2, 
///   and EZAppSetValue() was set with 512.  What would be displayed on the
///   screen for this value would be "1.65"
////////////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldAnalogValueScaled(rom char * prLabel,
   ezapp_analog_type_t analogType, 
   uint16_t minValue,
   uint16_t maxValue,
   uint16_t scaling,
   uint8_t decimalPlaces
   );


//////////////////////////////////////////////////
/// Sets the value of an analog field at 'index' to 'newVal'.  This controls
/// the value going from the PIC to the smartphone.
//////////////////////////////////////////////////
void EZAppSetValue(ezapp_field_index_t index, uint16_t newVal);

//////////////////////////////////////////////////
/// Sets the value of a field at 'index' and 'index+1' to 'newVal'. The field
/// at 'index' contains the lower 16 bits while the field at 'index+1' contains
/// the upper 16 bits of the value. This controls the value going from the PIC
/// to the smartphone.
/// This should only be used on fields that use 32bit values.
//////////////////////////////////////////////////
void EZAppSetValue32(ezapp_field_index_t index, uint32_t newVal);

//////////////////////////////////////////////////
/// Gets the value of the GUI element at 'index'.  This reads the value written
/// to the PIC by the smartphone.
//////////////////////////////////////////////////
uint16_t EZAppGetValue(ezapp_field_index_t index);

//////////////////////////////////////////////////
/// Gets the value of the GUI element at 'index' and 'index+1'.  The field
/// at 'index' contains the lower 16 bits while the field at 'index+1' contains
/// the upper 16 bits of the value. This reads the value written to the PIC by
/// the smartphone.
/// This should only be used on fields that use 32bit values.
//////////////////////////////////////////////////
uint32_t EZAppGetValue32(ezapp_field_index_t index);

//////////////////////////////////////////////////
/// Creates a two-state button field, with a     
/// String for the button label, OFF state and   
/// ON state. 
/// 'prHeader' is a text header on the left side/column next to the buttons,
///   is optional.
/// 'prLabels' points to two strings representing the state of the
/// button, with each string seperated by a tab (\t, or 0x09) character.
/// The first string in 'prStates' is when the value of the element it 0,
/// the second string when the value of the element is 1.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldButtonTwoState(rom char * prHeader, rom char * prLabels);

/// values that can be passed to 'ledColor0' and 'ledColor1' of
/// 'ezapp_buttons_t'.  these define the color of the LED for that
/// state.
typedef enum 
{
   EZAPP_LED_GRAY = 0,
   EZAPP_LED_RED = 1,
   EZAPP_LED_GREEN = 2,
   EZAPP_LED_ORANGE = 3
} ezapp_button_led_t;

/// configuration information that can be passed to 
/// EZAppAddFieldButtons() to configure how buttons operate on
/// this row.
/// 'numButtons' is the number of buttons to display on this row.
///   If the buttons have inset status LEDs, the max number is 8.
///   If the buttons do not have inset status LEDs, the max number is 16.
/// 'fillRow' defines if the buttons should attempt to use the total
///   length of the row, or the minimum size needed for the contents
///   of the button.  if 'fillRow' is true and 'numButtons' isn't 1,
///   then all buttons will use equal width to fill the row.
/// 'oneState' defines if the button is one-state or two-state.
///   TRUE defines one-state, which means when the button is held
///      the value read for this button is set and when the button
///      is released the value read for this button is clear.
///   FALSE defines two-state, which means a press and release 
///      toggles the value read for this button.
/// 'ledColor0' and 'ledColor1' define the colors used by
///      inset status LEDs.  Set both to 0 to not use 
///      inset status LEDs.
typedef struct __EZAPP_PACKED
{
   struct __EZAPP_PACKED
   {
      unsigned int numButtons:5;
      unsigned int fillRow: 1;
      unsigned int oneState: 1;
      unsigned int reserved: 1;
   };
   struct __EZAPP_PACKED
   {
      unsigned int ledColor0: 4; // see ezapp_button_led_t for valid values
      unsigned int ledColor1: 4; // see ezapp_button_led_t for valid values
   };
} ezapp_buttons_t;

//////////////////////////////////////////////////
/// Create one or more buttons on this row.
/// 'cfg' is information about the setup of this row, 
///   see the comments above ezapp_buttons_t for documentation
///   of these options.
/// 'prHeader' is an optional pointer to a string that is the
///   header string to go in the left column.  If this is NULL
///   there will be no left header column string in the row.
/// 'pLabels' is the strings that go in each button.  A \t
///   character to divide several strings in each button.
///
/// If the buttons are one state, pLabels must contain one string
///   per button (with \t divider).  An example for 3 buttons:
///   char labels[] = "Button 1\tButton 2\tButton 3";
/// If the buttons are two state, pLabels must contain two strings
///   per button (with \t divider). An example for 2 buttons:
///   char labels[] = "#1 Off\t#1 On\t#2 Off\t#2 On";
///
/// EZAppGetKbhit() will return TRUE if the value of any buttons
///   on the row have changed.  EZAppGetValue() will return a
///   bitmap of values for this row, where bit 0 of the returned
///   value is the button on the left most column, bit 0 is the
///   value of the second button, etc.
///
/// ex_ezapp_buttons.c is an example that shows how to use
/// buttons in your project.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldButtonsROM(ezapp_buttons_t cfg, rom char * prHeader, rom char * pLabels);
ezapp_field_index_t EZAppAddFieldButtons(ezapp_buttons_t cfg, rom char * prHeader, char * pLabels);

//////////////////////////////////////////////////
/// If a button has inset leds ('ledColor0' and 'ledColor1' passed to
/// EZAppAddFieldButtons() 'cfg' wasn't 0), then this function
/// can be used to set the value of the LED.
/// 'idx' is the row index for the EZ App Lynx, use the value returned
///   by EZAppAddFieldButtons().
/// 'column' is the column number for the button on the row, where
///   the leftmost button is 0.
/// 'value' is the new value for the LED.
//////////////////////////////////////////////////
void EZAppSetButtonLED(ezapp_field_index_t idx, uint8_t column, bool value);

//////////////////////////////////////////////////
/// Creates a Digital type field with 'prLabel' string for Label.
/// A digital representation looks like a series of LEDs.
/// 'num' is the number of LEDs to display on the host smartphone app, as one
/// GUI element field can display 16 values (one for each bit passed to it
/// with EZAppSetValue()/EZAppGetValue()).
/// Bit0 of value (EZAppSetValue()/EZAppGetValue()) is the
/// first LED on the GUI element, Bit1 of value
/// (EZAppSetValue()/EZAppGetValue()) is the second LED
/// of the GUI element, etc.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldDigitalValue(rom char * prLabel, uint8_t num);

//////////////////////////////////////////////////
/// Add a pull-down GUI element.  'prLabel' is the description/label of the
/// element/value.  'numChoices' is the number of choices to have on the 
/// pull-down.  'prChoices' is the string value of each choice, each string
/// choice should be seperated with a tab character (/t).  If NULL is
/// passed to 'prChoices' then it won't use strings to populate the pull-down,
/// will use the integer value 0..numChoices-1.
///
/// Ex:
///  index = EZAppAddFieldPulldownValue((rom char *)"FAN", 2, (rom char *)"Off\tOn");
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldPulldownValue(rom char * prLabel,
   uint8_t numChoices, 
   rom char * prChoices);

//////////////////////////////////////////////////
/// Creates a horizontal spacer to help seperate fields on the smartphone GUI.
/// See EZAppAddFieldSpacerDynamic(), which has more options.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldSpacer();

//////////////////////////////////////////////////
/// Creates a horizontal spacer to help seperate fields on the smartphone GUI.
/// 'rgb' is the color of the spacer.  'size' is the size of the spacer, 0
/// for default size.
/// 'prString' can be a string that goes above the spacer.  The string will
/// use the same 'rgb' and 'size' as the spacer.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldSpacerDynamic(uint32_t rgb, uint8_t size, rom char *prString);

typedef enum
{
   EZAPP_IMAGE_SCALE_NONE = 0, // No Scaling is applied
   EZAPP_IMAGE_SCALE_HALF = 1, // Image will scale to half of the screen width
   EZAPP_IMAGE_SCALE_FULL = 2, // Image will scale to match screen width
   EZAPP_IMAGE_SCALE_PIXEL = 3 // Image will scale such that a 1000px image takes up full screen width
}ezapp_image_scale_t;

//////////////////////////////////////////////////
/// holds configuration information for an image field
///
///'scaleType': The scale mode to use for this image
///
///'preloaded': If true, the app will look for the
/// image in the app bundle instead of the web. Currently,
/// the only preloaded images available for use are the CCS
/// logo ("logo"), a round red LED ("led_round_red"), and a
/// round green LED ("led_round_green"). To use these images,
/// set 'preloaded' to TRUE and set the 'urls' parameter of
/// EZAppAddFieldImage() to one of the noted strings.
///
/// 'useLogo': If true, the app will use the image
/// on the connect screen instead of in the GUI. The image
/// should be a 200px X 100px image for the best results
///
/// 'button': If true, this image behaves as a button, 
/// with the GUI sending the state to the PIC
///
/// 'oneState': If true, and if button is true, this image
/// will behave as a push button, changing state only
/// while held
///
/// 'reserved': currently unused bits
//////////////////////////////////////////////////
typedef struct __EZAPP_PACKED 
{
   uint8_t scaleType:2; // this is of type ezapp_image_scale_t
   uint8_t preloaded:1;
   uint8_t useLogo:1;
   uint8_t button:1;
   uint8_t oneState:1;
   uint8_t reserved:2;  
} ezapp_image_cfg_t;

//////////////////////////////////////////////////
/// Creates an image to be displayed on the GUI. The
/// image can be loaded from the web or from the app
/// bundle. More information on images loaded from the
/// app bundle can be found in the EZ App Lynx documentation
///
/// 'cfg': a struct that holds configuration information
/// for this image field
///
/// 'urls' one or two urls separated by a tab. the
/// urls can be for images on the web, or they can
/// be files in the app bundle. If using the iOS App,
/// the URLs may need to be HTTPS, not HTTP.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldImage(ezapp_image_cfg_t cfg, rom char *urls);

typedef enum 
{
   EZAPP_GRAPH_BAR = 0, // The graph will be drawn as a bar graph
   EZAPP_GRAPH_LINE = 1 // The graph will be drawn as a line graph
}ezapp_graph_type_t;

typedef enum
{
   EZAPP_GRAPH_LINE_SOLID = 0,
   EZAPP_GRAPH_LINE_DASHED = 1,
   EZAPP_GRAPH_LINE_SOLID_SQUARE = 2,
   EZAPP_GRAPH_LINE_SOLID_CIRCLE = 3,
   EZAPP_GRAPH_LINE_SOLID_TRIANGLE = 4,
   EZAPP_GRAPH_LINE_SOLID_DIAMOND = 5,
   EZAPP_GRAPH_LINE_SOLID_HOURGLASS = 6,
   EZAPP_GRAPH_LINE_DASHED_SQUARE = 7,
   EZAPP_GRAPH_LINE_DASHED_CIRCLE = 8,
   EZAPP_GRAPH_LINE_DASHED_TRIANGLE = 9,
   EZAPP_GRAPH_LINE_DASHED_DIAMOND = 10,
   EZAPP_GRAPH_LINE_DASHED_HOURGLASS = 11
   
}ezapp_graph_line_type_t;

//////////////////////////////////////////////////
/// holds configuration information for a graph
///
/// 'type': The type of graph to draw
///
/// 'scrollX': If true, the x axis will scroll to 
/// show new data. If false, the graph will be 
/// bounded by minX and maxX
///
/// 'scrollY': If true, the y axis will scroll to 
/// show new data. If false, the graph will be
/// bounded by minY and maxY
///
/// 'showLegend': If true, the legend will be drawn
/// under the graph. This will slightly reduce the 
/// overall space for drawing the data
///
/// 'showXGrid': If true, gridlines will be drawn in
/// the data area at regular intervals on the x axis
///
/// 'showYGrid': If true, gridlines will be drawn in
/// the data area at regular intervals on the y axis
///
/// 'xScale': Scale factor used on the X-Axis (as
/// interger). Scale can be adjusted to a decimal
/// factor by setting 'decimalX'
///
/// 'decimalX': Adjusts 'xScale' by this number of decimal
/// places. For example, if 'xScale' is 25 and 'decimalX'
/// is 1, then the resulting scale factor is *2.5
///
/// 'yScale': Same as 'xScale' but for the Y-Axis
///
/// 'decimalY': Same as 'decimalX' but for the Y-Axis
///
/// 'height': This is the height of the graph in 
/// relation to the size of other fields. 1 would 
/// be the same height as a regular field, 5 would
/// be 5 times the height of a regular field. If
/// a 0 is used for the height and the rest of the
/// GUI does not extend below the bottom of the
/// screen, the graph will take up the remaining space
///
/// 'numPoints': This is the number of points to keep
/// on screen when using scrollX
///
/// 'minX': If not using scrollX, this is the
/// lowest x value to display
///
/// 'maxX': If not using scrollx, this is the highest
/// x value to display
///
/// 'ticksX': This is the number of labels to print
/// on the x axis
///
/// 'minY': If not using scrollY, this is the
/// lowest y value to display
///
/// 'maxY': If not using scrollY, this is the highest
/// y value to display
///
/// 'ticksY': This is the number of labels to print
/// on the y axis
//////////////////////////////////////////////////
typedef struct __EZAPP_PACKED
{
   struct __EZAPP_PACKED
   {
      uint8_t scrollX:1;
      uint8_t scrollY:1;
      uint8_t showLegend:1;
      uint8_t showXGrid:1;
      uint8_t showYGrid:1;
      uint8_t type:3;   // this is of type ezapp_graph_type_t
   };
   
   struct __EZAPP_PACKED
   {
      uint8_t decimalX: 4;
      uint8_t decimalY: 4;
   };
   
   uint16_t scaleX;
   uint16_t scaleY;
   uint8_t height;
   uint8_t numPoints;
   uint16_t minX;
   uint16_t maxX; 
   uint16_t ticksX;
   uint16_t minY;
   uint16_t maxY;
   uint16_t ticksY;
   
   rom char* labels;
   rom char* title;
} ezapp_graph_cfg_t;

//////////////////////////////////////////////////
/// Creates a graph for displaying dynamic and 
///historic data on the GUI
///
/// 'cfg': A struct pointer that holds configuration
/// information for this graph
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldGraph(ezapp_graph_cfg_t *cfg);

//////////////////////////////////////////////////
/// Creates a graph series to be displayed on a
/// graph. This field must immediately follow a 
/// graph field or another graph series field
///
/// 'type': The style of line to draw, solid or 
/// dashed, and the shape for each datapoint
///
/// 'color' The color for this series
///
/// 'increment' The amount by which each subsequent 
/// point is ahead of the last on the x axis
/// 
/// 'name' The name of the series
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldGraphSeries( ezapp_graph_line_type_t type, 
   uint32_t color, uint16_t increment, rom char *name);

//////////////////////////////////////////////////
/// Creates a Color Picker field. Since a field's value is only
/// 16 bits, this field requires two field indexes to store the
/// full 24-bit color. The index returned will be the first of
/// the two indexes used and is the index that should be passed
/// to EZAppSetValue32() and EZAppGetValue32(). EZAppSetValue()
/// and EZAppGetValue() should NOT be used with this field type!
///
/// 'prHeader' is an optional pointer to a string that is the
///   header string to go in the left column.  If this is NULL
///   there will be no left header column string in the row.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldColorPicker(rom char * prHeader);

//////////////////////////////////////////////////
/// Holds configuration information for a joystick.
/// You can use EZAppCreateRGB() for the colors.
///
/// 'stickColor': Color of the movable "stick"
///
/// 'borderColor': Color of the border around the base
///
/// 'backgroundColor': Color of the base circle
/// 
/// 'borderWidth': Width of the border around the base
///
/// 'maxHeight': Max number of lines of text tall this
///  field will be. This is based on the current row
///  style's right view text height.
///
/// 'autoRecenter': If set to true, the joystick will
///  snap back to 0 degrees/0 strength when letting go
///  of the stick. If false, it will remain in the last
///  position touched.
//////////////////////////////////////////////////
typedef struct __EZAPP_PACKED
{
   uint32_t stickColor;
   uint32_t borderColor;
   uint32_t backgroundColor;
   uint8_t borderWidth;
   uint8_t maxHeight;
   
   struct __EZAPP_PACKED
   {
      uint8_t autoReCenter:1;
      uint8_t reserved:7;
   };
 
} ezapp_joystick_cfg_t;

//////////////////////////////////////////////////
/// Creates a Joystick field. 
///
/// 'cfg' is a pointer to the joystick configuration. See the
///  ezapp_joystick_cfg_t structure for more info.
///
/// 'prHeader' is an optional pointer to a string that is the
///   header string to go in the left column.  If this is NULL
///   there will be no left header column string in the row.
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldJoystick(ezapp_joystick_cfg_t *cfg, rom char * prHeader);

//////////////////////////////////////////////////
/// This extracts the Angle value from the joystick field
/// value returned by EZAppGetValue(<Joystick field index>).
/// Angle is always 0-359 degrees.
//////////////////////////////////////////////////
#define EZAppGetJoystickAngle(val) (val &= 0x01FF)

//////////////////////////////////////////////////
/// This extracts the Strength value from the joystick field
/// value returned by EZAppGetValue(<Joystick field index>).
/// Strength is always 0-100%
//////////////////////////////////////////////////
#define EZAppGetJoystickStrength(val) (val >> 9)

//////////////////////////////////////////////////
/// This creates the 24bit RGB value used by the library from individual
/// 8bit values for red, green and blue.  This can be used when passing
/// values to functions like EZAppAddFieldStringDynamic() and
/// EZAppSetFGColor().
//////////////////////////////////////////////////
//#define EZAppCreateRGB(r,g,b) make32(0, (uint8_t)r, (uint8_t)g, (uint8_t)b)
#define EZAppCreateRGB(r,g,b) ((uint32_t)b + ((uint32_t)g<<8) + ((uint32_t)r<<16))

//////////////////////////////////////////////////
/// This creates the 32bit ARGB value used by the library from individual
/// 8bit values for red, green, blue, and alpha.  This can be used in the
/// extended versions of styles. Alpha is the color's transparency, where
/// 0xFF is completely opaque and 0x00 is completely transparent
//////////////////////////////////////////////////
#define EZAppCreateARGB(a,r,g,b) ((uint32_t)b + ((uint32_t)g<<8) + ((uint32_t)r<<16) + ((uint32_t)a<<24))

//////////////////////////////////////////////////
/// Set the forground color of the app, which is used by the text/fonts.
/// You can use EZAppCreateRGB() to create value.
//////////////////////////////////////////////////
void EZAppSetFGColor(uint32_t rgb);

//////////////////////////////////////////////////
/// Set the background color of the app, which is the color
/// usder under text/fonts.
/// You can use EZAppCreateRGB() to create value.
//////////////////////////////////////////////////
void EZAppSetBGColor(uint32_t rgb);

//////////////////////////////////////////////////
/// Set the application background color, which is used in between
/// dynamic elements.
/// You can use EZAppCreateRGB() to create value.
//////////////////////////////////////////////////
void EZAppSetAppColor(uint32_t rgb);

/// Style info that can be sent to EZAppAddStylesROM().
/// This will reset all style information to defaults.
#define EZAPP_STYLE_DEFAULT_ALL()  0x00

/// Style info that can be sent to EZAppAddStylesROM().
/// This will configure the style of all the text in the app
/// (except for text inside buttons).
/// 'boldLeft' and 'boldRight' is a boolean that defines
///   whether the text in that column should be bold.
/// 'sizeDeltaLeft' and 'sizeDeltaRight' is a signed int8 that
///   defines if the text on the left column or right column 
///   should be bigger, or smaller.  setting this value to 0 will 
///   leave the text at the default size.
/// 'colorRgbLeft' and 'colorRgbRight' defined the text color
///   of the left or right column.  EZAppCreateRGB() can be used
///   to define this value.
/// 'colorRgbEdited' is the text color used in the right column when
///   a value has been edited but not sent back to the PIC.
#define EZAPP_STYLE_TEXT_CREATE(boldLeft, boldRight, sizeDeltaLeft, colorRgbLeft,  sizeDeltaRight, colorRgbRight, colorRgbEdited)   \
      0x01, \
      (boldLeft << 0) | (boldRight << 1),  \
      sizeDeltaLeft, \
      (colorRgbLeft & 0xFF),  \
      ((colorRgbLeft >> 8) & 0xFF), \
      ((colorRgbLeft >> 16) & 0xFF),   \
      sizeDeltaRight,   \
      (colorRgbRight & 0xFF),  \
      ((colorRgbRight >> 8) & 0xFF), \
      ((colorRgbRight >> 16) & 0xFF),  \
      (colorRgbEdited & 0xFF),  \
      ((colorRgbEdited >> 8) & 0xFF), \
      ((colorRgbEdited >> 16) & 0xFF)
      
/// Style info that can be sent to EZAppAddStylesROM().
/// This will undo styles applied by EZAPP_STYLE_TEXT_CREATE()
#define EZAPP_STYLE_TEXT_DEFAULT() 0x81

/// Style info that can be sent to EZAppAddStylesROM().
/// This will set styles for all buttons displayed in the app,
/// including colors, text size, padding, etc.
/// 'borderRadius' is the rounding pitch of the corners of the
///   button.
/// 'borderWidth' is the width of the border for each button.  This
///   value does not necessarily mean pixels, as the smartphone/tablets
///   deal with scaled pixels in order to maintain unity with 
///   different screen sizes.  A value of 0 is no border, 1 is the
///   thinnest border, etc.
/// 'borderColor' is the color of the border, if 'borderWidth' isn't
///   0.  EZAppCreateRGB() can be used to create this value.
/// 'textSizeDelta' is a signed int8 that defines if the text size
///   in the button should be bigger, or smaller.  setting this value 
///   to 0 will  leave the text at the default size.
/// 'textColor' is the color of the text in the button. 
///   EZAppCreateRGB() can be used to create this value.
/// 'bottomColor' and 'topColor' define the colors used in the
///   button.  The application will use a gradient of these
///   two colors.  EZAppCreateRGB() can be used to create this
///   value. 
/// 'innerPadding' is the padding between the border of the button
///   and the inner contents of the button.  This doesn't explicitly 
///   use pixels, instead they used a scaled pixels to make the UI 
///   uniform between  smartphone/tablets that have different screen 
///   sizes.  A value of 0 would be no padding, 1 the least amount of
///   padding, etc.  See ezapplynx_help.pdf for a graphic representation 
///   of all the paddings.
#define EZAPP_STYLE_BUTTON_CREATE(borderWidth, borderRadius, borderColor, textSizeDelta, textColor, topColor, bottomColor, innerPadding) \
   0x02, \
   borderRadius,  \
   borderWidth,   \
   textSizeDelta, \
   (borderColor & 0xFF),  \
   ((borderColor >> 8) & 0xFF), \
   ((borderColor >> 16) & 0xFF),   \
   (topColor & 0xFF),  \
   ((topColor >> 8) & 0xFF), \
   ((topColor >> 16) & 0xFF),   \
   (bottomColor & 0xFF),  \
   ((bottomColor >> 8) & 0xFF), \
   ((bottomColor >> 16) & 0xFF),   \
   (textColor & 0xFF),  \
   ((textColor >> 8) & 0xFF), \
   ((textColor >> 16) & 0xFF),   \
   innerPadding
   
/// Style info that can be sent to EZAppAddStylesROM().
/// This will undo styles applied by EZAPP_STYLE_BUTTON_CREATE()
#define EZAPP_STYLE_BUTTON_DEFAULT() 0x82

/// Style info that can be sent to EZAppAddStylesROM().
/// Sets color style for each row/field that is added from the PIC to the
/// application, including the background of each row or the border
/// of each row.
/// 'borderRadius' is the rounding pitch of the corners of the
///   row.
/// 'borderWidth' is the width of the border for each row.  This
///   value does not necessarily mean pixels, as the smartphone/tablets
///   deal with scaled pixels in order to maintain unity with 
///   different screen sizes.  A value of 0 is no border, 1 is the
///   thinnest border, etc.
/// 'borderColor' is the color of the row, if 'borderWidth' isn't
///   0.  EZAppCreateRGB() can be used to create this value.
/// 'bottomColor' and 'topColor' define the colors used in the
///   row.  The application will use a gradient of these
///   two colors.  EZAppCreateRGB() can be used to create this
///   value. 
#define EZAPP_STYLE_ROW_COLORS_CREATE(borderWidth, borderRadius, borderColor, topColor, bottomColor) \
   0x03, \
   borderWidth,   \
   borderRadius,  \
   (borderColor & 0xFF),  \
   ((borderColor >> 8) & 0xFF), \
   ((borderColor >> 16) & 0xFF),   \
   (topColor & 0xFF),  \
   ((topColor >> 8) & 0xFF), \
   ((topColor >> 16) & 0xFF),   \
   (bottomColor & 0xFF),  \
   ((bottomColor >> 8) & 0xFF), \
   ((bottomColor >> 16) & 0xFF)
   
/// Style info that can be sent to EZAppAddStylesROM().
/// This will undo styles applied by EZAPP_STYLE_ROW_COLORS_CREATE()
#define EZAPP_STYLE_ROW_COLORS_DEFAULT() 0x83

/// These values can be passed to the 'leftAlignment' and
/// 'rightAlignment' fields of EZAPP_STYLE_ROW_SPACINGS_CREATE().
/// A HOR value can be OR'd with a VER value.
typedef enum
{
   EZAPP_ALIGN_HOR_LEFT = 0x0,
   EZAPP_ALIGN_HOR_CENTER = 0x1,
   EZAPP_ALIGN_HOR_RIGHT = 0x2,
   EZAPP_ALIGN_VER_TOP = 0x00,
   EZAPP_ALIGN_VER_CENTER = 0x10,
   EZAPP_ALIGN_VER_BOTTOM = 0x20,
} ezapp_alignment_t;

/// Style info that can be sent to EZAppAddStylesROM().
/// 'leftWidth' is the size, in %, of the left column (if the 
///   row has a left column).  Set this to -1 or greater than
///   100 and the application will use the minimum size required
///   for the left column to fit the left column header text.
/// 'leftAlignment' and 'rightAlignment' define how the contents
///   of the left or right column should be aligned.  The values
///   of ezapp_alignment_t can be used here.
/// 'paddingInner' defines the padding between the border of the 
///   row and the inner contents of the row.
/// 'paddingPerColumn' is the padding between each column
///   of the row.
/// 'paddingBelow' is the padding below a row before the next
///   row starts.
///
/// The padding values don't explicitly use pixels, instead they
///   used a scaled pixels to make the UI uniform between 
///   smartphone/tablets that have different screen sizes.
///   A value of 0 would be no padding, 1 the least amount of
///   padding, etc.
/// See ezapplynx_help.pdf for a graphic representation of
///   all the paddings.
#define EZAPP_STYLE_ROW_SPACINGS_CREATE(leftWidth, leftAlignment, rightAlignment, paddingInner, paddingPerColumn, paddingBelow) \
   0x04, \
   leftWidth,  \
   paddingInner,  \
   paddingBelow,  \
   paddingPerColumn, \
   leftAlignment, \
   rightAlignment
   
/// Style info that can be sent to EZAppAddStylesROM().
/// This will undo styles applied by EZAPP_STYLE_ROW_SPACINGS_CREATE()
#define EZAPP_STYLE_ROW_SPACINGS_DEFAULT() 0x84

/// Style info that can be sent to EZAppAddStylesROM().
/// Sets color style for each row/field that is added from the PIC to the
/// application, including the background of each row or the border
/// of each row. This is an extended version of EZAPP_STYLE_ROW_COLORS_CREATE()
/// where colors include the Alpha byte used for levels of transparency.
/// 'borderRadius' is the rounding pitch of the corners of the
///   row.
/// 'borderWidth' is the width of the border for each row.  This
///   value does not necessarily mean pixels, as the smartphone/tablets
///   deal with scaled pixels in order to maintain unity with 
///   different screen sizes.  A value of 0 is no border, 1 is the
///   thinnest border, etc.
/// 'borderColor' is the color of the row, if 'borderWidth' isn't
///   0.  EZAppCreateARGB() can be used to create this value.
/// 'bottomColor' and 'topColor' define the colors used in the
///   row.  The application will use a gradient of these
///   two colors.  EZAppCreateARGB() can be used to create this
///   value. 
#define EZAPP_STYLE_ROW_COLORS_EX_CREATE(borderWidth, borderRadius, borderColor, topColor, bottomColor) \
   0x05, \
   borderWidth,   \
   borderRadius,  \
   (borderColor & 0xFF),  \
   ((borderColor >> 8) & 0xFF), \
   ((borderColor >> 16) & 0xFF),   \
   ((borderColor >> 24) & 0xFF),   \
   (topColor & 0xFF),  \
   ((topColor >> 8) & 0xFF), \
   ((topColor >> 16) & 0xFF),   \
   ((topColor >> 24) & 0xFF),   \
   (bottomColor & 0xFF),  \
   ((bottomColor >> 8) & 0xFF), \
   ((bottomColor >> 16) & 0xFF), \
   ((bottomColor >> 24) & 0xFF)

/// Style info that can be sent to EZAppAddStylesROM().
/// Sets the background image for the application. If multiple background
/// styles are sent, only the last one will be used. If using this style, it is
/// best to add it before any other field as it overwrites the main app color
/// to being transparent, and other fields may use the old color they are added
/// first.
/// 'preloaded' is a boolean telling the app if 'imageURL' points to preloaded
///   image in the application bundle or an image on the web.
/// 'imageURL' is a string containing the name of a preloaded image or a
///   URL to an image on the web to download. Downloaded images are stored in
///   the application's cache memory so they do not need to be downloaded again
///   unless the cache becomes full.
#define EZAPP_STYLE_BACKGROUND_CREATE(preloaded, imageURL) \
   0x06, \
   (preloaded << 0),   \
   0x00, \
   0x00, \
   imageURL

/// Style info that can be sent to EZAppAddStylesROM().
/// This will set styles for all buttons displayed in the app,
/// including text alignment and background images.
/// 'textAlignment' defines how the button text should be aligned.  The values
///   of ezapp_alignment_t can be used here.
/// 'maxHeightLines' is the maximum number of lines of text (height) the image
///   will be scaled to. If the height is unobtainable (because the button is
///   not wide enough to achieve it), the image will be scaled to a maximum of
///   the button's width, maintaining aspect ratio.
/// 'preloaded' is a boolean telling the app if 'imageURLs' points to preloaded
///   images in the application bundle or an image on the web.
/// 'imageURLs' is a string containing the name of a preloaded image or a
///   URL to an image on the web to download. This string can contain up to two
///   images separated by '\t', one for each button state. If only one image is
///   specified, then it is used for both states. Setting this value to 0 (NULL)
///   indicates no images are to be used. Downloaded images are stored in
///   the application's cache memory so they do not need to be downloaded again
///   unless the cache becomes full.
#define EZAPP_STYLE_BUTTON_EXTRA_CREATE(textAlignment, maxHeightLines, preloaded, imageURLs) \
   0x07, \
   textAlignment, \
   maxHeightLines, \
   (preloaded << 0), \
   0x00, \
   imageURLs

// same as above, but you can specify each imageUrl seperately
#define EZAPP_STYLE_BUTTON_EXTRA_CREATE2(textAlignment, maxHeightLines, preloaded, imageURL1, imageURL2) \
   0x07, \
   textAlignment, \
   maxHeightLines, \
   (preloaded << 0), \
   0x00, \
   imageURL1 "\t" imageURL2

/// Style info that can be sent to EZAppAddStylesROM().
/// This will undo styles applied by EZAPP_STYLE_BUTTON_EXTRA_CREATE()
#define EZAPP_STYLE_BUTTON_EXTRA_DEFAULT() 0x87

//////////////////////////////////////////////////
/// Add a style to the application.  Once a style has been added,
/// all following fields will use that style.  Styles can accumulate,
/// so adding a text style at one point of the code and then adding
/// a button style later in the code will still keep the text style
/// applied.
///
/// 'pStyles' is a pointer to style data.  Style data can be
/// initialized using the EZAPP_STYLE macros provided above,
/// like EZAPP_STYLE_TEXT_CREATE which creates a text style.
/// 'bytes' is the size of 'pStyles'.
/// 
/// An example:
///   rom char* style1[] = {EZAPP_STYLE_TEXT_CREATE(your settings};
///   EZAppAddStylesROM(style1, sizeof(style1));
///   EZAppSetValueString(EZAppAddFieldString(), "line1");
///   EZAppSetValueString(EZAppAddFieldString(), "line2");
///   rom char* style2[] = {EZAPP_STYLE_ROW_COLORS_CREATE(your settings};
///   EZAppAddStylesROM(style2, sizeof(style2));
///   EZAppSetValueString(EZAppAddFieldString(), "line3");
///
///   In the example above, "line1" and "line2" will use the
///   text styles provided in style1[] and then "line3" will
///   use the row colors provided in style2[] AND the the
///   text styles provided in style1[].
///
/// ex_ezapp_style.c example is provided that is a more comprehensive
/// example of using styles in your project.
//////////////////////////////////////////////////
void EZAppAddStylesROM(rom void* pStyles, size_t bytes);

#ifndef EZAPP_IS_CONNECTED
//////////////////////////////////////////////////
/// Callback: Connection sense
///
/// This macro can be defined/created by the user, and will be called
/// each time a succesful message was heard from the smartphone.  The
/// user can then use this for a connection sense routine, handling their
/// own timeout.
//////////////////////////////////////////////////
#define EZAPP_IS_CONNECTED()
#endif

#endif
