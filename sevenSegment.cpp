#include "sevenSegment.h"

static byte CLK_PIN, DATA_PIN;
const byte segmentFigure[] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
#define CLOCK_DELAY 5

#define PIN_LOW(pin)     digitalWrite(pin, LOW);
#define PIN_HIGH(pin)    digitalWrite(pin, HIGH);
#define PIN_INPUT(pin)   pinMode(pin, INPUT_PULLUP);
#define PIN_OUTPUT(pin)  pinMode(pin, OUTPUT);
#define DELAY(delay)     delayMicroseconds(delay);

#define START() do{ PIN_HIGH(DATA_PIN);         \
                    PIN_HIGH(CLK_PIN);          \
                    DELAY(CLOCK_DELAY);         \
                    PIN_LOW(DATA_PIN);          \
                }while(0)

#define STOP() do{  PIN_LOW(CLK_PIN);           \
                    DELAY(CLOCK_DELAY);         \
                    PIN_LOW(DATA_PIN);          \
                    DELAY(CLOCK_DELAY);         \
                    PIN_HIGH(CLK_PIN);          \
                    DELAY(CLOCK_DELAY);         \
                    PIN_HIGH(DATA_PIN);         \
                }while(0)


void sevenSegmentInit(byte clkPin, byte dataPin)
{
    CLK_PIN = clkPin;
    DATA_PIN = dataPin;

    PIN_OUTPUT(CLK_PIN);
    PIN_OUTPUT(DATA_PIN);

    PIN_LOW(CLK_PIN);
    PIN_LOW(DATA_PIN);
}



static byte getAck(void)
{
    byte Ack;

    PIN_LOW(CLK_PIN);
    PIN_INPUT(DATA_PIN);
    DELAY(CLOCK_DELAY);

    Ack = digitalRead(DATA_PIN);
    PIN_HIGH(CLK_PIN);
    DELAY(CLOCK_DELAY);
    PIN_LOW(CLK_PIN);

    PIN_OUTPUT(DATA_PIN);

return Ack;
}

static void sevenSegmentWriteByte(byte data)
{
    for(byte i=0; i<8; i++)
    {
        PIN_LOW(CLK_PIN);
        if(data & 1) {PIN_HIGH(DATA_PIN);}
        else         {PIN_LOW(DATA_PIN); }
        
        DELAY(CLOCK_DELAY);
        PIN_HIGH(CLK_PIN);
        DELAY(CLOCK_DELAY);
        data >>= 1;
    }
}

static void seventSegmentWrite(byte *pData, byte Lenght)
{
    byte Ack;
    Ack = 1;

    START();
    for(byte i=0; i<Lenght; i++){
        sevenSegmentWriteByte(pData[i]);
        Ack &= getAck();
    }
    STOP();
}

void seventSegmentShowDigit(char *pString)
{
    byte j=0, tempByte, sendData[16];

    sendData[0] = 0x40;                              /* Memory Write Command */
    seventSegmentWrite(sendData,1);

    sendData[j++] = 0xC0;                            /* Set display address to first digit command */
    
    for(byte i=0; i<5; i++)
    {
        if(i != 2){
            tempByte = 0;
            if(pString[i] >= '0' && pString[i] <= '9')
            {
                tempByte = segmentFigure[pString[i] & 0xF];
            }
            sendData[j++] = tempByte;
        }
        else
        {
            if(pString[i] == ':')   {   sendData[2] |= 0x80;    }
        }
    }
    
    seventSegmentWrite(sendData, j);
}

void seventSegmentSetBrightness(byte value)
{
    byte brightnessControl;

    if(value == 0)
    {
        brightnessControl = 0x80;
    }
    else
    {
        if(value > 8) { value = 8; }
        brightnessControl = (0x88) | (value - 1);
    }
    seventSegmentWrite(&brightnessControl, 1);
}