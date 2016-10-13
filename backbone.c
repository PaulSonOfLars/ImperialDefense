#include "backbone.h"

#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>

#define MOUSE_ACCELERATION 1.8
#define CURSOR_CENTER_X 8
#define CURSOR_CENTER_Y 13

int32_t g_iMousePosX = CURSOR_CENTER_X;
int32_t g_iMousePosY = CURSOR_CENTER_Y;

int32_t g_iMousePosX_prev = CURSOR_CENTER_X;
int32_t g_iMousePosY_prev = CURSOR_CENTER_Y;

int32_t g_iSeed = 39;

int32_t rand()
{
  g_iSeed = (1103515245 * g_iSeed + 12345) % 2^32;
  return g_iSeed;
}

void EnableMMU(void)
{
  static volatile __attribute__ ((aligned (0x4000))) unsigned PageTable[4096];

  unsigned base;
  for (base = 0; base < 512; base++)
  {
    // outer and inner write back, write allocate, shareable
    PageTable[base] = base << 20 | 0x1140E;
  }
  for (; base < 4096; base++)
  {
    // shared device, never execute
    PageTable[base] = base << 20 | 0x10416;
  }

  // restrict cache size to 16K (no page coloring)
  unsigned auxctrl;
  asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
  auxctrl |= 1 << 6;
  asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));

  // set domain 0 to client
  asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (1));

  // always use TTBR0
  asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));

  // set TTBR0 (page table walk inner cacheable, outer non-cacheable, shareable memory)
  asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (3 | (unsigned) &PageTable));

  // invalidate data cache and flush prefetch buffer
  asm volatile ("mcr p15, 0, %0, c7, c5,  4" :: "r" (0) : "memory");
  asm volatile ("mcr p15, 0, %0, c7, c6,  0" :: "r" (0) : "memory");

  // enable MMU, L1 cache and instruction cache, L2 cache, write buffer,
  //   branch prediction and extended page table on
  unsigned mode;
  asm volatile ("mrc p15,0,%0,c1,c0,0" : "=r" (mode));
  mode |= 0x0480180D;
  asm volatile ("mcr p15,0,%0,c1,c0,0" :: "r" (mode) : "memory");
}

void onClick(void);
static void mouse_callback (unsigned nButtons,
				      int nDisplacementX,		// -127..127
				      int nDisplacementY) {

      int posX = g_iMousePosX + ((int32_t)(nDisplacementX*MOUSE_ACCELERATION));
      int posY = g_iMousePosY + ((int32_t)(nDisplacementY*MOUSE_ACCELERATION));

      if(posX <= CURSOR_CENTER_X)
        g_iMousePosX_prev = posX = CURSOR_CENTER_X;

      if(posY <= CURSOR_CENTER_Y)
        g_iMousePosY_prev = posY = CURSOR_CENTER_Y;

      if(posX + CURSOR_CENTER_X >= SCREEN_SIZE_X)
        g_iMousePosX_prev = posX = SCREEN_SIZE_X - CURSOR_CENTER_X;

      if(posY + CURSOR_CENTER_Y >= SCREEN_SIZE_Y)
        g_iMousePosY_prev = posY = SCREEN_SIZE_Y - CURSOR_CENTER_Y;

      // set the new pos
      g_iMousePosX = posX + g_iMousePosX_prev;
      g_iMousePosY = posY + g_iMousePosY_prev;

      g_iMousePosX = g_iMousePosX/2;
      g_iMousePosY = g_iMousePosY/2;

      g_iMousePosX_prev = g_iMousePosX;
      g_iMousePosY_prev = g_iMousePosY;

      if(nButtons & MOUSE_BUTTON1) {
        onClick();
      }
}

void InitializeMouse(void) {
  if(USPiEnvInitialize() == 0) {
    ASM_ClearScreen();
    DrawText("Could not initialize USPi Env", ERROR_POS, ERROR_POS);
    DebugLED_ERROR();
  }

  if(USPiInitialize() == 0) {
    ASM_ClearScreen();
    DrawText("Could not initialize USPi Lib", ERROR_POS, ERROR_POS);
    DebugLED_ERROR();
  }

  if(USPiMouseAvailable() == 0) {
    ASM_ClearScreen();
    DrawText("Cannot continue without the mouse", ERROR_POS, ERROR_POS);
    DebugLED_ERROR();
  }

  USPiMouseRegisterStatusHandler(&mouse_callback);
}
