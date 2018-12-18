#pragma once

typedef unsigned char u8;
typedef unsigned short u16;

inline
void poke(u16 address, u8 value)
{
  *(u8*)address = value;
}

static inline
void poke16(u16 address, u16 value)
{
  *(u16*)address = value;
}

inline
u8 peek(u16 address)
{
  return *((u8*)0x4212);
}

