/* graphics */

void settiles(unsigned int b, unsigned char *p1, unsigned int size);
void setmap(unsigned int b, unsigned char *p1);
void setpalette(unsigned char *pal);
void enablescreen();
void screenmode(unsigned char m);
void setsprite(unsigned int s, unsigned char x, unsigned char y, unsigned char t, unsigned char p);
void waitforvsync();
void delay(unsigned int d);
void resettimer();
void sync(unsigned int d);

