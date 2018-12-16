#pragma once

inline
void poke(unsigned short address, unsigned char value)
{
  *(unsigned char*)address = value;
}

inline
unsigned char peek(unsigned short address)
{
  return *((unsigned char*)0x4212);
}

