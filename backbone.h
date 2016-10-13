#ifndef __BACKBONE_H__
#define __BACKBONE_H__

#include <stdint.h>
#include "images.h"

#define SCREEN_SIZE_X       (int32_t)1024
#define SCREEN_SIZE_Y       (int32_t)768
#define FRAMES_PER_SECOND   (int32_t)16666 // 1/60 * 1000000 = 60FPS

#define ERROR_POS 50

extern int32_t rand();

// pseudo transparency applied, skips every pure blue colour (0,0,255)
extern void ASM_DrawImage(void *pImage, void *pSize, int32_t posX, int32_t posY);
extern void ASM_DrawImageTr(void *pImage, void *pSize, int32_t posX, int32_t posY);

extern void ASM_ClearScreen(void);
extern void ASM_SwapBuffers(void);

extern void EnableMMU(void);
extern void InitializeMouse(void);

inline void DrawImage(int32_t imageID, int32_t posX, int32_t posY) {
  ASM_DrawImage((void*)totalSetOfImages[imageID]->pBinaryAddress,
    &totalSetOfImages[imageID]->info, posX, posY);
}


inline void DrawImageTr(int32_t imageID, int32_t posX, int32_t posY) {
  ASM_DrawImageTr((void*)totalSetOfImages[imageID]->pBinaryAddress,
    &totalSetOfImages[imageID]->info, posX, posY);
}

inline void DrawText(const char *pText, int32_t posX, int32_t posY) {
  char *pTemp = (char*)pText;
  int32_t curPosX = posX;
  while(pTemp && *pTemp) {
    // set current offset according to ASCII
    totalSetOfImages[IMAGE_FONT]->info.currentFrame =
      (int32_t)(*(pTemp));

    // draw the character
    DrawImageTr(IMAGE_FONT, curPosX, posY);

    curPosX += totalSetOfImages[IMAGE_FONT]->info.widthReal;
    pTemp++;
  }
}

inline static char* itoa(int val, int base){
  static char buf[32] = {0};
  int i = 30;

  for(; val && i ; --i, val /= base) {
    buf[i] = "0123456789abcdef"[val % base];
  }
  return &buf[i+1];
}

extern void DebugLED_ON(void);
extern void DebugLED_OFF(void);
extern void DebugLED_ERROR(void);

// makes LED blink very fast indicating an error
extern void ErrorLoop(void);

extern int32_t g_iMousePosX;
extern int32_t g_iMousePosY;

// GetSystemTime gets the current system "time" (amount of milliseconds passed
// since the RPi was turned on
extern int32_t GetSystemTime(void);

extern float Math_SQRT(float value);

#endif
