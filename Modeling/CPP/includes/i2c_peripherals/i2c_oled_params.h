#ifndef __OLED_PARAMETERS_H__
#define __OLED_PARAMETERS_H__
/// The following "raw" color names are kept for backwards client compatability
/// They can be disabled by predefining this macro before including the Adafruit header
/// client code will then need to be modified to use the scoped enum values directly
/// fit into the SSD1306_ naming scheme
// #define SSD1306_BLACK               0    ///< Draw 'off' pixels
// #define SSD1306_WHITE               1    ///< Draw 'on' pixels
// #define SSD1306_INVERSE             2    ///< Invert pixels

// #define SSD1306_COMMAND             0x00
// #define SSD1306_MEMORYMODE          0x20 ///< See datasheet
// #define SSD1306_COLUMNADDR          0x21 ///< See datasheet
// #define SSD1306_PAGEADDR            0x22 ///< See datasheet
// #define SSD1306_SETCONTRAST         0x81 ///< See datasheet
// #define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
// #define SSD1306_SEGREMAP            0xA0 ///< See datasheet
// #define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
// #define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
// #define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
// #define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
// #define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
// #define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
// #define SSD1306_DISPLAYON           0xAF ///< See datasheet
// #define SSD1306_COMSCANINC          0xC0 ///< Not currently used
// #define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
// #define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
// #define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
// #define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
// #define SSD1306_SETCOMPINS          0xDA ///< See datasheet
// #define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

// #define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
// #define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
// #define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

// #define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
// #define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

// #define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
// #define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
// #define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
// #define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
// #define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
// #define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
// #define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

#define SSD1306_ADDRESS		0x3C

#define SSD1306_COMMAND			0x00
#define SSD1306_DATA			0xC0
#define SSD1306_DATA_CONTINUE	0x40

#define SSD1306_SET_CONTRAST_CONTROL					0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON							0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF
#define SSD1306_NOP										0xE3

#define SSD1306_HORIZONTAL_SCROLL_RIGHT					0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT					0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3

#define SSD1306_SET_LOWER_COLUMN						0x00
#define SSD1306_SET_HIGHER_COLUMN						0x10
#define SSD1306_MEMORY_ADDR_MODE						0x20
#define SSD1306_SET_COLUMN_ADDR							0x21
#define SSD1306_SET_PAGE_ADDR							0x22

#define SSD1306_SET_START_LINE							0x40
#define SSD1306_SET_SEGMENT_REMAP						0xA0
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8
#define SSD1306_COM_SCAN_DIR_INC						0xC0
#define SSD1306_COM_SCAN_DIR_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3
#define SSD1306_SET_COM_PINS							0xDA
#define SSD1306_CHARGE_PUMP								0x8D

#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO				0xD5
#define SSD1306_SET_PRECHARGE_PERIOD					0xD9
#define SSD1306_SET_VCOM_DESELECT						0xDB

#endif //__OLED_PARAMETERS_H__