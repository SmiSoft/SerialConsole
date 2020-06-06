#include "Arduino.h"
#include "SerialConsole.hpp"
#include <avr/pgmspace.h>

void print_P(Stream& serial,const char *msg)
{
  static char c[11];
  unsigned char cnt = 0;
  while(1)
  {
    c[cnt] = pgm_read_byte_near(msg);
    if(c[cnt] == 0){
      serial.print(c);
      break;
    }
    ++msg;
    ++cnt;
    if(cnt >= sizeof(c)){
      c[cnt] = 0;
      serial.print(c);
      cnt = 0;
    }
  }
}

SerialConsole::SerialConsole(Stream&_serial, byte _bufferSize, const char*_promt):
  serial(_serial),bufferSize(_bufferSize),promt(_promt),requiredProcess(false),bufferPos(0)
{
  buffer1 = (char*)malloc(bufferSize); // unluckily, we may do nothing, if result is NULL, so won't even check
  buffer2 = (char*)malloc(bufferSize);
  if((buffer1 == NULL) || (buffer2 == NULL))
  {
    print_P(serial,"Shit happens: out of memory\n");
  }
  serial.print(promt);
}

SerialConsole::~SerialConsole()
{
  free(buffer1);
  buffer1 = NULL;
  free(buffer2);
  buffer2 = NULL;
}

boolean isPrintable(char ch)
{
  static const char*chok = " ()[]{}<>+-*\\|/^%=!?.,:;~`@#$&_\"'";
  if((ch >= '0') && (ch <= '9'))
    return true;
  if((ch >= 'A') && (ch <= 'Z'))
    return true;
  if((ch >= 'a') && (ch <= 'z'))
    return true;
  for(byte b = 0; chok[b] != 0; ++b)
    if(ch == chok[b])
      return true;
  return false;
}

void SerialConsole::nextChar()
{
  int inChar;
  while ((inChar = serial.read()) >= 0)
  {
    if((inChar==127) || (inChar == 8))
    {
      // by default, Putty sends 127 code for BS and HyperTerminal - 8. May be overridden by software...
      if(bufferPos > 0){
        buffer1[--bufferPos] = 0;
        serial.print("\b \b");
      }
    }
    else if((inChar == 13) || (inChar == 10))
    {
      // pressing Enter on remote machine results in capturing received command in buffer2 and raising requiredProcess flag
      serial.print("\n\r");
      if(buffer2 != NULL){
        for(byte b = 0; b < bufferPos; ++b)
        {
          buffer2[b]=buffer1[b];
        }
        buffer2[bufferPos]=0;
      }
      bufferPos = 0;
      requiredProcess = true;
    }
    else if((isPrintable((char)inChar)) && (bufferPos < bufferSize))
    {
      // received from serial port printable data captured into buffer1 (if it's large amount)
      if(buffer1 != NULL)
      {
        buffer1[bufferPos++] = (char)inChar;
      }
      // echoing
      serial.print((char)inChar);
    }
    else
    {
      // non-printable characters or buffer overflow results in "bell" character (\b was occupied, \a because "audible")
      serial.print('\a');
    }
  }
}

const char*SerialConsole::avail()
{
  if(!requiredProcess)
    return NULL;
  requiredProcess = false;
  return buffer2;
}

void SerialConsole::done()
{
  serial.print(promt);
}

