#include "DFPlayerMini.h"
#include "uart.h";

void DFPlayerMini::findChecksum()
{
  uint16_t checksum = (~(VER + LEN + commandValue + FEEDBACK + paramMSB + paramLSB)) + 1;

  checksumMSB = checksum >> 8;
  checksumLSB = checksum & 0xFF;
  
  return;
}

void DFPlayerMini::sendCommand(uint8_t command, uint16_t parameter = 0)
{
  commandValue = command;
  paramMSB = (uint8_t) (parameter >> 8);
  paramLSB = (uint8_t) (parameter & 0xFF);
  
  findChecksum();
  sendData();
  
  return;
}

void DFPlayerMini::sendData()
{
  uart_putc(SB);
  uart_putc(VER);
  uart_putc(LEN);
  uart_putc(commandValue);
  uart_putc(FEEDBACK);
  uart_putc(paramMSB);
  uart_putc(paramLSB);
  uart_putc(checksumMSB);
  uart_putc(checksumLSB);
  uart_putc(EB);
  
  return;
}
