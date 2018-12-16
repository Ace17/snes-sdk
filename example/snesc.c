#include <string.h>
#include "snesc.h"
#include "graph.h"
#include "input.h"

extern unsigned char tiles1[];
extern unsigned char tiles2[];
extern unsigned char bg1map[];
extern unsigned char bg2map[];
extern unsigned char palette[];
extern unsigned char backpal[];

unsigned int blockmap[0x400];
unsigned int backmap[0x400];
unsigned int pal[0x100];
unsigned char blocks[0x64];

unsigned char map[0x64] =
{
  7, 8, 8, 8, 8, 8, 8, 8, 8, 7,
  8, 7, 8, 7, 8, 8, 7, 8, 7, 8,
  8, 8, 7, 8, 7, 7, 8, 7, 8, 8,
  8, 8, 8, 1, 3, 3, 1, 8, 8, 8,
  8, 0, 4, 8, 8, 8, 8, 4, 0, 8,
  8, 0, 8, 8, 5, 5, 8, 8, 0, 8,
  8, 0, 4, 8, 8, 8, 8, 4, 0, 8,
  8, 8, 8, 1, 3, 3, 1, 8, 8, 8,
  8, 8, 6, 8, 6, 6, 8, 6, 8, 8,
  7, 7, 7, 7, 8, 8, 7, 7, 7, 7,
};

static const char* ST_READY = "PLAYER 1\n\n READY";
static const char* ST_GAMEOVER = "GAME OVER";
static const char* ST_PAUSED = "PAUSE";
static const char* ST_BLANK = "        ";

int px = 80;
unsigned int i, j, b = 0, c, obx, oby, bx = 5, by = 11, py = 0;

typedef struct
{
  int x, y;
}Vec2;

Vec2 vel = { 2, 1 };
Vec2 pos = { 94, 109 };

static const Vec2 dir[4] =
{
  { -2, -1 }, { -1, -2 }, { 1, -2 }, { 2, -1 }
};

unsigned int blockcount = 0;
unsigned long long score = 0, hiscore = 50000;
unsigned int level2 = 1;
unsigned int color = 0, level = 0, lives = 4;

void draw_screen()
{
  // main sprites
  setsprite(0, pos.x, pos.y, 20, 0x31);
  setsprite(1, px, 200, 15, 0x31);
  setsprite(2, px + 8, 200, 16, 0x31);
  setsprite(3, px + 16, 200, 16, 0x31 + 64);
  setsprite(4, px + 24, 200, 17, 0x31);

  // shadow sprites
  setsprite(5, pos.x + 3, pos.y + 3, 21, 0x11);
  setsprite(6, px + 4, 204, 18, 0x11);
  setsprite(7, px + 12, 204, 19, 0x11);
  setsprite(8, px + 20, 204, 19, 0x11 + 64);
  setsprite(9, px + 28, 204, 18, 0x11 + 64);
}

void die()
{
  if(lives == 0)
  {
    writestring(ST_GAMEOVER, blockmap, 0x267, 0x3F6);
    setmap(0, (unsigned char*)blockmap);
    lose: goto lose;
  }

  lives--;
  pos.x = 94;
  pos.y = 109;
  px = 80;
  writenum(lives, 8, blockmap, 0x136, 0x426);
  writestring(ST_READY, blockmap, 0x248, 0x3F6);
  setmap(0, (unsigned char*)blockmap);

  draw_screen();
  delay(50);

  while(getjoystatus(0) == 0)
    continue;

  writestring(ST_BLANK, blockmap, 0x248, 0x3F6);
  writestring(ST_BLANK, blockmap, 0x289, 0x3F6);
  setmap(0, (unsigned char*)blockmap);
}

void handle_pause()
{
  if((getjoystatus(0) & START_BUTTON) != 0)
  {
    writestring(ST_PAUSED, blockmap, 0x269, 0x3F6);
    setmap(0, (unsigned char*)blockmap);

    while(getjoystatus(0) != 0)
    {
      clearjoy(0);
      delay(5);
    }

    while((getjoystatus(0) & START_BUTTON) == 0)
      continue;

    while(getjoystatus(0) != 0)
    {
      clearjoy(0);
      delay(5);
    }

    writestring(ST_BLANK, blockmap, 0x269, 0x3F6);
    setmap(0, (unsigned char*)blockmap);
  }
}

int clamp(int val, int min, int max)
{
  if(val < min)
    val = min;

  if(val > max)
    val = max;

  return val;
}

void new_level()
{
  level++;
  level2++;
  pos.x = 94;
  pos.y = 109;
  px = 80;
  writenum(level2, 8, blockmap, 0x2D6, 0x426);
  writestring(ST_READY, blockmap, 0x248, 0x3F6);
  memcpy(backmap, bg2map + 0x800 * (level & 3), 0x800);
  memcpy(blocks, map, 0x64);

  if(color < 6)
  {
    color++;
  }
  else
    color = 0;

  memcpy(pal + 16, backpal + color * 16, 0x10);
  b = 0;

  for(j = 0; j < 10; j++)
  {
    for(i = 0; i < 20; i++, i++)
    {
      int a = blocks[b];

      if(a < 8)
      {
        c = (j << 5) + i;
        blockcount++;
        blockmap[0x62 + c] = 13 + (a << 10);
        blockmap[0x63 + c] = 14 + (a << 10);
        backmap[0x83 + c] += 0x400;
        backmap[0x84 + c] += 0x400;
      }

      b++;
    }
  }

  setpalette((unsigned char*)pal);
  setmap(0, (unsigned char*)blockmap);
  setmap(1, (unsigned char*)backmap);

  draw_screen();
  delay(50);

  while(getjoystatus(0) == 0)
    continue;

  writestring(ST_BLANK, blockmap, 0x248, 0x3F6);
  writestring(ST_BLANK, blockmap, 0x289, 0x3F6);
  setmap(0, (unsigned char*)blockmap);
}

void run_frame()
{
  resettimer();

  handle_pause();

  if(getjoystatus(0) & A_BUTTON != 0)
  {
    if(getjoystatus(0) & RIGHT_BUTTON)
      px += 4;

    if(getjoystatus(0) & LEFT_BUTTON)
      px -= 4;
  }
  else
  {
    if(getjoystatus(0) & RIGHT_BUTTON)
      px += 2;

    if(getjoystatus(0) & LEFT_BUTTON)
      px -= 2;
  }

  clearjoy(0);

  px = clamp(px, 16, 144);

  pos.x += vel.x;
  pos.y += vel.y;

  if(pos.x > 171)
  {
    vel.x = -vel.x;
    pos.x = 171;
  }
  else if(pos.x < 16)
  {
    vel.x = -vel.x;
    pos.x = 16;
  }

  if(pos.y < 15)
  {
    vel.y = -vel.y;
  }
  else if(pos.y > 195)
  {
    if(pos.y < 203)
    {
      if((pos.x >= px) && (pos.x <= px + 27))
      {
        int a = (pos.x - px) / 7;
        vel = dir[a];
      }
    }
    else if(pos.y > 224)
    {
      die();
    }
  }
  else if(pos.y < 112)
  {
    obx = bx;
    oby = by;
    bx = (pos.x - 14) >> 4;
    by = (pos.y - 14) >> 3;
    b = bx + (by << 3) + (by << 1) - 10;

    if((b >= 0) && (b < 100))
    {
      if(blocks[b] != 8)
      {
        blockcount--;

        for(i = 0; i <= level; i++)
          score += blocks[b] + 1;

        if(oby != by)
          vel.y = -vel.y;

        if(obx != bx)
          vel.x = -vel.x;

        blocks[b] = 8;
        b = (by << 5) + (bx << 1);
        blockmap[0x42 + b] = 0;
        blockmap[0x43 + b] = 0;
        backmap[0x63 + b] -= 0x400;
        backmap[0x64 + b] -= 0x400;
        writenum(score, 8, blockmap, 0xF5, 0x426);

        if(score > hiscore)
        {
          hiscore = score;
          writenum(score, 8, blockmap, 0x95, 0x426);
        }

        setmap(0, (unsigned char*)blockmap);
        setmap(1, (unsigned char*)backmap);

        if(blockcount == 0)
        {
          new_level();
        }
      }
    }
  }

  draw_screen();

  sync(1);
}

int main()
{
  snesc_init();

  settiles(0, tiles1, 0xF00);
  settiles(1, tiles2, 0x250);

  memcpy(blockmap, bg1map, 0x800);
  memcpy(backmap, bg2map, 0x800);
  memcpy(blocks, map, 0x64);
  memcpy(pal, palette, 0x200);

  for(j = 0; j < 10; j++)
  {
    for(i = 0; i < 20; i += 2)
    {
      int a = blocks[b];

      if(a < 8)
      {
        c = (j << 5) + i;
        blockcount++;
        blockmap[0x62 + c] = 13 + (a << 10);
        blockmap[0x63 + c] = 14 + (a << 10);
        backmap[0x83 + c] += 0x400;
        backmap[0x84 + c] += 0x400;
      }

      b++;
    }
  }

  writenum(lives, 8, blockmap, 0x136, 0x426);
  writestring(ST_READY, blockmap, 0x248, 0x3F6);

  setmap(0, (unsigned char*)blockmap);
  setmap(1, (unsigned char*)backmap);
  setpalette((unsigned char*)pal);

  enablescreen();

  draw_screen();

  delay(50);

  while(getjoystatus(0) == 0)
    continue;

  writestring(ST_BLANK, blockmap, 0x248, 0x3F6);
  writestring(ST_BLANK, blockmap, 0x289, 0x3F6);
  setmap(0, (unsigned char*)blockmap);

  while(1)
    run_frame();
}

