/* init */
void snesc_init(void);

/* string */
void writenum(unsigned long long num, unsigned char len, unsigned int* map, unsigned int p, unsigned int offset);
void writestring(const char* st, unsigned int* map, unsigned int p, unsigned int offset);

extern unsigned char snesc_timer_enabled;
extern unsigned int snesc_timers[16];
extern unsigned char snesc_do_copy;

struct oam_table1
{
  unsigned char x;
  unsigned char y;
  unsigned char t;
  unsigned char p;
};

extern struct oam_table1 snesc_oam_table1[128];
extern unsigned int snesc_oam_table2[32];
extern unsigned char snesc_palette[512];

struct dma_transfer
{
  union
  {
    struct
    {
      unsigned short addr; /* 0 + 1 */
      unsigned char bank; /* 2 */
      unsigned char type;
    } c;
    unsigned char* ptr;
  } src;
  unsigned short dest; /* 3 + 4 */
  unsigned short size; /* 5 + 6 */
};

extern struct dma_transfer snesc_dma_transfers[64];

