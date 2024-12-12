
#ifndef ILI9341_H
#define ILI9341_H
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <data/sprite.h>
#include "linkedlist.h"

#define SPI_DDR DDRB
#define CS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5
#define DC PINB1
#define RST PINB0

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

#define CS_LOW() (PORTB &= ~(1 << CS)) // CS active (low)
#define CS_HIGH() (PORTB |= (1 << CS)) // CS inactive (high)

#define DC_LOW() (PORTB &= ~(1 << DC)) // Command mode
#define DC_HIGH() (PORTB |= (1 << DC)) // Data mode

#define RST_LOW() (PORTB &= ~(1 << RST)) // Reset active (low)
#define RST_HIGH() (PORTB |= (1 << RST)) // Reset inactive (high)

class Command
{
public:
    int commandId;
    LinkedList<char> data;
    Command(int commandId) : commandId(commandId) {}
    Command *writeInt(uint32_t val)
    {
        this->data.push(val >> 24);
        this->data.push(val >> 16);
        this->data.push(val >> 8);
        this->data.push(val & 0xFF);
        return this;
    }
    Command *writeShort(uint16_t val)
    {
        this->data.push(val >> 8);
        this->data.push(val & 0xFF);
        return this;
    }
    Command *writeByte(uint8_t val)
    {
        this->data.push(val);
        return this;
    }
};

const uint8_t font5x7[][5] PROGMEM = {
    // Space ' ' (ASCII 32)
    {0x00, 0x00, 0x00, 0x00, 0x00},
    // '!' (ASCII 33)
    {0x00, 0x00, 0x5F, 0x00, 0x00},
    // '"' (ASCII 34)
    {0x00, 0x07, 0x00, 0x07, 0x00},
    // '#' (ASCII 35)
    {0x14, 0x7F, 0x14, 0x7F, 0x14},
    // '$' (ASCII 36)
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},
    // '%' (ASCII 37)
    {0x23, 0x13, 0x08, 0x64, 0x62},
    // '&' (ASCII 38)
    {0x36, 0x49, 0x55, 0x22, 0x50},
    // ''' (ASCII 39)
    {0x00, 0x05, 0x03, 0x00, 0x00},
    // '(' (ASCII 40)
    {0x00, 0x1C, 0x22, 0x41, 0x00},
    // ')' (ASCII 41)
    {0x00, 0x41, 0x22, 0x1C, 0x00},
    // '*' (ASCII 42)
    {0x14, 0x08, 0x3E, 0x08, 0x14},
    // '+' (ASCII 43)
    {0x08, 0x08, 0x3E, 0x08, 0x08},
    // ',' (ASCII 44)
    {0x00, 0x50, 0x30, 0x00, 0x00},
    // '-' (ASCII 45)
    {0x08, 0x08, 0x08, 0x08, 0x08},
    // '.' (ASCII 46)
    {0x00, 0x60, 0x60, 0x00, 0x00},
    // '/' (ASCII 47)
    {0x20, 0x10, 0x08, 0x04, 0x02},
    // '0' (ASCII 48)
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    // '1' (ASCII 49)
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    // '2' (ASCII 50)
    {0x42, 0x61, 0x51, 0x49, 0x46},
    // '3' (ASCII 51)
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    // '4' (ASCII 52)
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    // '5' (ASCII 53)
    {0x27, 0x45, 0x45, 0x45, 0x39},
    // '6' (ASCII 54)
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    // '7' (ASCII 55)
    {0x01, 0x71, 0x09, 0x05, 0x03},
    // '8' (ASCII 56)
    {0x36, 0x49, 0x49, 0x49, 0x36},
    // '9' (ASCII 57)
    {0x06, 0x49, 0x49, 0x29, 0x1E},
    // ':' (ASCII 58)
    {0x00, 0x36, 0x36, 0x00, 0x00},
    // ';' (ASCII 59)
    {0x00, 0x56, 0x36, 0x00, 0x00},
    // '<' (ASCII 60)
    {0x08, 0x14, 0x22, 0x41, 0x00},
    // '=' (ASCII 61)
    {0x14, 0x14, 0x14, 0x14, 0x14},
    // '>' (ASCII 62)
    {0x00, 0x41, 0x22, 0x14, 0x08},
    // '?' (ASCII 63)
    {0x02, 0x01, 0x51, 0x09, 0x06},
    // '@' (ASCII 64)
    {0x32, 0x49, 0x79, 0x41, 0x3E},
    // 'A' (ASCII 65)
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    // 'B' (ASCII 66)
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    // 'C' (ASCII 67)
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    // 'D' (ASCII 68)
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    // 'E' (ASCII 69)
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    // 'F' (ASCII 70)
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    // 'G' (ASCII 71)
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    // 'H' (ASCII 72)
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    // 'I' (ASCII 73)
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    // 'J' (ASCII 74)
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    // 'K' (ASCII 75)
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    // 'L' (ASCII 76)
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    // 'M' (ASCII 77)
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    // 'N' (ASCII 78)
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    // 'O' (ASCII 79)
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    // 'P' (ASCII 80)
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    // 'Q' (ASCII 81)
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    // 'R' (ASCII 82)
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    // 'S' (ASCII 83)
    {0x46, 0x49, 0x49, 0x49, 0x31},
    // 'T' (ASCII 84)
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    // 'U' (ASCII 85)
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    // 'V' (ASCII 86)
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    // 'W' (ASCII 87)
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    // 'X' (ASCII 88)
    {0x63, 0x14, 0x08, 0x14, 0x63},
    // 'Y' (ASCII 89)
    {0x07, 0x08, 0x70, 0x08, 0x07},
    // 'Z' (ASCII 90)
    {0x61, 0x51, 0x49, 0x45, 0x43},
    // '[' (ASCII 91)
    {0x00, 0x7F, 0x41, 0x41, 0x00},
    // '\' (ASCII 92)
    {0x02, 0x04, 0x08, 0x10, 0x20},
    // ']' (ASCII 93)
    {0x00, 0x41, 0x41, 0x7F, 0x00},
    // '^' (ASCII 94)
    {0x04, 0x02, 0x01, 0x02, 0x04},
    // '_' (ASCII 95)
    {0x40, 0x40, 0x40, 0x40, 0x40},
    // '`' (ASCII 96)
    {0x00, 0x01, 0x02, 0x04, 0x00},
    // 'a' (ASCII 97)
    {0x20, 0x54, 0x54, 0x54, 0x78},
    // 'b' (ASCII 98)
    {0x7F, 0x48, 0x44, 0x44, 0x38},
    // 'c' (ASCII 99)
    {0x38, 0x44, 0x44, 0x44, 0x20},
    // 'd' (ASCII 100)
    {0x38, 0x44, 0x44, 0x48, 0x7F},
    // 'e' (ASCII 101)
    {0x38, 0x54, 0x54, 0x54, 0x18},
    // 'f' (ASCII 102)
    {0x08, 0x7E, 0x09, 0x01, 0x02},
    // 'g' (ASCII 103)
    {0x0C, 0x52, 0x52, 0x52, 0x3E},
    // 'h' (ASCII 104)
    {0x7F, 0x08, 0x04, 0x04, 0x78},
    // 'i' (ASCII 105)
    {0x00, 0x44, 0x7D, 0x40, 0x00},
    // 'j' (ASCII 106)
    {0x20, 0x40, 0x44, 0x3D, 0x00},
    // 'k' (ASCII 107)
    {0x7F, 0x10, 0x28, 0x44, 0x00},
    // 'l' (ASCII 108)
    {0x00, 0x41, 0x7F, 0x40, 0x00},
    // 'm' (ASCII 109)
    {0x7C, 0x04, 0x18, 0x04, 0x78},
    // 'n' (ASCII 110)
    {0x7C, 0x08, 0x04, 0x04, 0x78},
    // 'o' (ASCII 111)
    {0x38, 0x44, 0x44, 0x44, 0x38},
    // 'p' (ASCII 112)
    {0x7C, 0x14, 0x14, 0x14, 0x08},
    // 'q' (ASCII 113)
    {0x08, 0x14, 0x14, 0x18, 0x7C},
    // 'r' (ASCII 114)
    {0x7C, 0x08, 0x04, 0x04, 0x08},
    // 's' (ASCII 115)
    {0x48, 0x54, 0x54, 0x54, 0x20},
    // 't' (ASCII 116)
    {0x04, 0x3F, 0x44, 0x40, 0x20},
    // 'u' (ASCII 117)
    {0x3C, 0x40, 0x40, 0x20, 0x7C},
    // 'v' (ASCII 118)
    {0x1C, 0x20, 0x40, 0x20, 0x1C},
    // 'w' (ASCII 119)
    {0x3C, 0x40, 0x30, 0x40, 0x3C},
    // 'x' (ASCII 120)
    {0x44, 0x28, 0x10, 0x28, 0x44},
    // 'y' (ASCII 121)
    {0x0C, 0x50, 0x50, 0x50, 0x3C},
    // 'z' (ASCII 122)
    {0x44, 0x64, 0x54, 0x4C, 0x44},
    // '{' (ASCII 123)
    {0x00, 0x08, 0x36, 0x41, 0x00},
    // '|' (ASCII 124)
    {0x00, 0x00, 0x7F, 0x00, 0x00},
    // '}' (ASCII 125)
    {0x00, 0x41, 0x36, 0x08, 0x00},
    // '~' (ASCII 126)
    {0x08, 0x04, 0x08, 0x10, 0x08}};

uint16_t width = 240;
uint16_t height = 320;

void SPI_init()
{
    // Set MOSI, SCK, CS, DC, and RST as outputs
    SPI_DDR |= (1 << MOSI) | (1 << SCK) | (1 << CS) | (1 << DC) | (1 << RST);

    // Set MISO as input
    SPI_DDR &= ~(1 << MISO);

    // Set CS, DC, and RST high to deselect and deactivate reset
    PORTB |= (1 << CS) | (1 << DC) | (1 << RST);

    // Enable SPI, set as Master, set clock rate fosc/2 (maximum speed for ATmega328P)
    SPCR = (1 << SPE) | (1 << MSTR); // SPI enabled, Master mode
    SPSR = (1 << SPI2X);             // Double SPI speed (fosc/2)
}

void SPI_write(uint8_t data)
{
    SPDR = data; // Start transmission
    while (!(SPSR & (1 << SPIF)))
        ; // Wait for transmission complete
}

void ILI9341_reset()
{
    RST_LOW(); // Reset the display
    _delay_ms(20);
    RST_HIGH(); // Bring display out of reset
    _delay_ms(150);
}

void ILI9341_sendData(uint8_t data)
{
    DC_HIGH(); // Data mode
    CS_LOW();  // Select the display
    SPI_write(data);
    CS_HIGH(); // Deselect the display
}

void ILI9341_sendData16(uint16_t data)
{
    DC_HIGH();              // Data mode
    CS_LOW();               // Select the display
    SPI_write(data >> 8);   // High byte
    SPI_write(data & 0xFF); // Low byte
    CS_HIGH();              // Deselect the display
}

void ILI9341_sendCommand(uint8_t cmd)
{
    DC_LOW(); // Command mode
    CS_LOW(); // Select the display
    SPI_write(cmd);
    CS_HIGH(); // Deselect the display
}

void ILI9341_sendCommand(Command *command)
{
    ILI9341_sendCommand(command->commandId);
    LinkedNode<char> *dummy = command->data.head;
    while (dummy != nullptr)
    {
        ILI9341_sendData(dummy->dataByte);
        dummy = dummy->next;
    }
}

void ILI9341_init()
{
    // https://github.com/adafruit/Adafruit_ILI9341/blob/master/Adafruit_ILI9341.cpp
    ILI9341_reset();

    // power control A
    ILI9341_sendCommand(Command(0xCB).writeByte(0x39)->writeByte(0x2C)->writeByte(0x00)->writeByte(0x34)->writeByte(0x02));

    // power control B
    ILI9341_sendCommand(Command(0xCF).writeByte(0x00)->writeByte(0xC1)->writeByte(0x30));

    // driver timing control A
    ILI9341_sendCommand(Command(0xE8).writeByte(0x85)->writeByte(0x00)->writeByte(0x78));

    // driver timing control B
    ILI9341_sendCommand(Command(0xEA).writeByte(0x00)->writeByte(0x00));

    // power on sequence control
    ILI9341_sendCommand(Command(0xED).writeByte(0x64)->writeByte(0x03)->writeByte(0x12)->writeByte(0x81));

    // pump ratio control
    ILI9341_sendCommand(Command(0xF7).writeByte(0x20));

    // power control, VRH[5:0]
    ILI9341_sendCommand(Command(0xC0).writeByte(0x23));

    // power control, SAP[2:0]; BT[3:0]
    ILI9341_sendCommand(Command(0xC1).writeByte(0x10));

    // vcm control
    ILI9341_sendCommand(Command(0xC5).writeByte(0x3E)->writeByte(0x28));

    // vcm control 2
    ILI9341_sendCommand(Command(0xC7).writeByte(0x86));

    // memory access control
    ILI9341_sendCommand(Command(0x36).writeByte(0x48));

    // pixel format (allows color & fills page)
    ILI9341_sendCommand(Command(0x3A).writeByte(0x55));

    // frameration control, normal mode full colours
    ILI9341_sendCommand(Command(0xB1).writeByte(0x00)->writeByte(0x18));

    // display function control
    ILI9341_sendCommand(Command(0xB6).writeByte(0x08)->writeByte(0x82)->writeByte(0x27));

    // 3gamma function disable
    ILI9341_sendCommand(Command(0xF2).writeByte(0x00));

    // gamma curve selected
    ILI9341_sendCommand(Command(0x26).writeByte(0x01));

    // set positive gamma correction
    ILI9341_sendCommand(Command(0xE0).writeByte(0x0F)->writeByte(0x31)->writeByte(0x2B)->writeByte(0x0C)->writeByte(0x0E)->writeByte(0x08)->writeByte(0x4E)->writeByte(0xF1)->writeByte(0x37)->writeByte(0x07)->writeByte(0x10)->writeByte(0x03)->writeByte(0x0E)->writeByte(0x09)->writeByte(0x00));

    // set negative gamma correction
    ILI9341_sendCommand(Command(0xE1).writeByte(0x00)->writeByte(0x0E)->writeByte(0x14)->writeByte(0x03)->writeByte(0x11)->writeByte(0x07)->writeByte(0x31)->writeByte(0xC1)->writeByte(0x48)->writeByte(0x08)->writeByte(0x0F)->writeByte(0x0C)->writeByte(0x31)->writeByte(0x36)->writeByte(0x0F));

    // exit sleep
    ILI9341_sendCommand(0x11);
    _delay_ms(120);

    // display on
    ILI9341_sendCommand(0x29);
}

void ILI9341_setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ILI9341_sendCommand(Command(0x2A).writeShort(x0)->writeShort(x1));
    ILI9341_sendCommand(Command(0x2B).writeShort(y0)->writeShort(y1));

    ILI9341_sendCommand(0x2C); // Memory Write
}

void ILI9341_fillScreen(uint16_t color)
{
    ILI9341_setAddressWindow(0, 0, width - 1, height - 1); // Full screen

    DC_HIGH(); // Data mode
    CS_LOW();
    CS_LOW(); // Select the display

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    for (uint32_t i = 0; i < 240UL * 320UL; i++)
    {
        SPI_write(hi);
        SPI_write(lo);
    }

    CS_HIGH(); // Deselect the display
}

void ILI9341_drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ILI9341_setAddressWindow(x, y, x + 1, y + 1);
    ILI9341_sendData16(color);
}

void ILI9341_fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    // Set the address window to the rectangle area
    ILI9341_setAddressWindow(x, y, x + w - 1, y + h - 1);

    uint32_t pixels = (uint32_t)w * h;
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    DC_HIGH(); // Data mode
    CS_LOW();  // Select the display

    // Send color data for all pixels
    for (uint32_t i = 0; i < pixels; i++)
    {
        SPI_write(hi);
        SPI_write(lo);
    }

    CS_HIGH(); // Deselect the display
}

void ILI9341_drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *bitmap)
{
    ILI9341_setAddressWindow(x, y, x + width - 1, y + height - 1);

    DC_HIGH();
    CS_LOW();
    // ILI9341_sendCommand(0x2C);
    uint16_t defaultColor = 0x11b4;
    for (int16_t i = 0; i < width * height; i++)
    {
        uint16_t color = pgm_read_word(&bitmap[i]);

        // ILI9341_sendData16(color);
        if (color == 0x0000)
        {
            SPI_write(defaultColor >> 8);
            SPI_write(defaultColor & 0xFF);
        }
        else
        {
            SPI_write(color >> 8);
            SPI_write(color & 0xFF);
        }
    }

    CS_HIGH();
}

void ILI9341_drawBitmap(uint16_t x, uint16_t y, Sprite *sprite)
{
    ILI9341_drawBitmap(x, y, sprite->width(), sprite->height(), sprite->bitmap());
}

void ILI9341_drawBitmapScaled(uint16_t x, uint16_t y, uint16_t scale, uint16_t width, uint16_t height, const uint16_t *bitmap)
{
    ILI9341_setAddressWindow(x, y, x + width * scale - 1, y + height * scale - 1);

    DC_HIGH();
    CS_LOW();
    uint16_t defaultColor = 0x11b4;

    for (int16_t j = 0; j < height; j++)
    {
        for (int16_t sj = 0; sj < scale; sj++)
        {
            for (int16_t i = 0; i < width; i++)
            {
                uint16_t index = j * width + i;
                uint16_t color = pgm_read_word(&bitmap[index]);
                for (int16_t si = 0; si < scale; si++)
                {

                    // Use defaultColor if the bitmap color is 0x0000
                    if (color == 0x0000)
                    {
                        SPI_write(defaultColor >> 8);
                        SPI_write(defaultColor & 0xFF);
                    }
                    else
                    {
                        SPI_write(color >> 8);
                        SPI_write(color & 0xFF);
                    }
                }
            }
        }
    }

    CS_HIGH();
}
void ILI9341_drawBitmapScaled(uint16_t x, uint16_t y, Sprite *sprite)
{
    ILI9341_drawBitmapScaled(x, y, sprite->scale(), sprite->width(), sprite->height(), sprite->bitmap());
}

void ILI9341_drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, bool transparent, uint8_t size)
{
    if (c < 32 || c > 126)
        c = '?'; // Replace unsupported characters with '?'

    for (uint8_t i = 0; i < FONT_WIDTH; i++)
    {
        uint8_t line = pgm_read_byte(&(font5x7[c - 32][i]));

        for (uint8_t j = 0; j < FONT_HEIGHT; j++)
        {
            if (line & 0x01)
            {
                if (size == 1)
                    ILI9341_drawPixel(x + i, y + j, color);
                else
                    ILI9341_fillRect(x + i * size, y + j * size, size, size, color);
            }
            else if (!transparent)
            {
                if (size == 1)
                    ILI9341_drawPixel(x + i, y + j, bg);
                else
                    ILI9341_fillRect(x + i * size, y + j * size, size, size, bg);
            }
            line >>= 1;
        }
    }
}

void ILI9341_drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size)
{
    ILI9341_drawChar(x, y, c, color, 0x0000, true, size);
}

void ILI9341_drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size)
{
    ILI9341_drawChar(x, y, c, color, bg, false, size);
}
// Implementation of ILI9341_drawString
void ILI9341_drawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, bool transparent, uint8_t size)
{
    while (*str)
    {
        if (*str == '\n')
        {
            y += FONT_HEIGHT * size + 2; // Move to next line
            x = 0;
        }
        else if (*str == '\r')
        {
            // Carriage return: do nothing or reset x position
            // x = 0;
        }
        else
        {
            ILI9341_drawChar(x, y, *str, color, bg, transparent, size);
            x += (FONT_WIDTH + 1) * size; // Advance x position (+1 for spacing)
        }
        str++;
    }
}

void ILI9341_drawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint8_t size)
{
    ILI9341_drawString(x, y, str, color, 0x0000, true, size);
}

void ILI9341_drawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size)
{
    ILI9341_drawString(x, y, str, color, bg, false, size);
}

void ILI9341_setRotation(uint8_t rotation)
{
    switch (rotation)
    {
    case 0:
        ILI9341_sendCommand(Command(0x36).writeByte(0x48));
        width = 240;
        height = 320;
        break;
    case 1:
        ILI9341_sendCommand(Command(0x36).writeByte(0x28));
        width = 320;
        height = 240;
        break;
    case 2:
        ILI9341_sendCommand(Command(0x36).writeByte(0x88));
        width = 240;
        height = 320;
        break;
    case 3:
        ILI9341_sendCommand(Command(0x36).writeByte(0xE8));
        width = 320;
        height = 240;
        break;
    default:
        ILI9341_sendCommand(Command(0x36).writeByte(0x48));
        width = 240;
        height = 320;
        break;
    }
}
#endif