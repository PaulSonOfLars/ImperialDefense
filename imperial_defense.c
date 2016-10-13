#include "backbone.h"
#include "images.h"
#include "imperial_defense.h"

int32_t initialized = 0;
int32_t oldTime;
int32_t prevSpawn;
int32_t timeWhenLastSpawned;
pixelPoint_t mousePos;
int32_t spawnedCounter;
int32_t waveSize;
int32_t timerVal;
int32_t allTimeWaves;
int32_t waveStartTime;
int32_t timerCounter;

extern image_t *totalSetOfImages[IMAGE_NUM];


int32_t FrameBuffer_Init(void);
void SetFrameBuffer(int32_t address);

// called once at start, set the game assets here
void game_initialize(void) {
    int32_t fb = FrameBuffer_Init();
    if (fb == 0) {
      DebugLED_ERROR();
    }

    init_images();
    SetFrameBuffer(fb);

    ASM_ClearScreen();
    DrawText("Initializing the best game ever made", ERROR_POS, ERROR_POS);

    EnableMMU(); // framerate beautifier
    InitializeMouse();

    gameStatus = GAME_INMENU;

    // clear all the projectiles
    for(int i = 0; i < MAX_PROJECTILES; i++)
      g_sProjectiles[i].active = 0;

    for(int i = 0; i < MAX_BLOOD_ANIMS; i++)
      g_sBloodAnims[i].active = 0;
}

// do all logical updates here, scale the velocity by the time delta
// formula for smooth movements:
// (1) given timeDelta is in seconds, and we know that velocity is change
//     in position per time, hence scale all the velocities by this timeDelta
//   aka: position += velocity * timeDelta;
// (2) when using constant velocities, you can also average previous frame
//     velocity with the new one to get even smoother experience
void game_update(float timeDelta) {

  // animate the regular images
  for(int i = 0; i < IMAGE_NUM; i++) {
    image_t *pCurImage = totalSetOfImages[i];
    if(!pCurImage)
      continue;

    if(!pCurImage->animated)
      continue;

    pCurImage->info.timeElapsed += timeDelta;
    if(pCurImage->info.timeElapsed > pCurImage->timePerFrame) {
      pCurImage->info.currentFrame++;
      if(pCurImage->info.currentFrame >= pCurImage->maxFrames)
        pCurImage->info.currentFrame = 0;

      pCurImage->info.timeElapsed -= pCurImage->timePerFrame;
    }
  }

  // animate blood sprites
  image_t *pBlood = totalSetOfImages[IMAGE_BLOOD];
  for(int i = 0; i < MAX_BLOOD_ANIMS; i++) {
    bloodanim_t *pCur = &g_sBloodAnims[i];
    if(pCur->active == 0)
      continue;

    pCur->info.timeElapsed += timeDelta;
    if(pCur->info.timeElapsed > pBlood->timePerFrame) {
      pCur->info.currentFrame++;
      if(pCur->info.currentFrame >= pBlood->maxFrames) {
        pCur->active = 0;
        continue;
      }

      pCur->info.timeElapsed -= pBlood->timePerFrame;
    }
  }

  // animate the projectiles
  for(int i = 0; i < MAX_PROJECTILES; i++) {
    projectile_t *pCur = &g_sProjectiles[i];
    if(!pCur)
      break;

    if(pCur->active == 0)
      continue;

    // delete the projectile if we visally touched a enemy
    int32_t shouldRemove = 0;
    enemNode_t *enemNode = enemHead;
    while (enemNode != NULL) {
      float distanceX = pCur->curPos.x - enemNode->enemy->stats->location.x;
      float distanceY = pCur->curPos.y - enemNode->enemy->stats->location.y;

      if( ((distanceX*distanceX) + (distanceY*distanceY)) <= (TILE_PIXEL_SIZE*TILE_PIXEL_SIZE) )
      {
        shouldRemove = 1;
        break;
      }
      enemNode = enemNode->next;
    }

    if(pCur->curPos.x < 9 ||
      pCur->curPos.y < 9 ||
      pCur->curPos.x >= SCREEN_SIZE_X - 9 ||
      pCur->curPos.y >= SCREEN_SIZE_Y - 9)
        shouldRemove = 1;

    if(shouldRemove == 1) {
      pCur->active = 0;
      continue;
    }

    pCur->curPos.x += pCur->directionX * ((float)PROJECTILE_SPEED * timeDelta);
    pCur->curPos.y += pCur->directionY * ((float)PROJECTILE_SPEED * timeDelta);
  }

  enemNode_t *enemNode = enemHead;
  while (enemNode != NULL) {
    enemy_move(enemNode->enemy, timeDelta);
    enemNode = enemNode->next;
  }

  if (prevSpawn +   (SEC_BETWEEN_SPAWNS * SEC_CONST) < GetSystemTime() && gameStatus == GAME_INGAME) {
    prevSpawn = GetSystemTime();
    spawnEnemy();
    if(waveSize == 0){//new wave
      waveStartTime = prevSpawn;// get current moment
      // timerVal = time length of current wave + time between wave in secs
      timerVal = waveSizeCalc(spawnedCounter) * SEC_BETWEEN_SPAWNS + SPAWN_DELAY_TIMER;
    }
    spawnedCounter ++;
    waveSize ++;
    if(waveSize >= waveSizeCalc(spawnedCounter)) {
      waveSize = 0;
      prevSpawn += SPAWN_DELAY_TIMER * SEC_CONST;
      allTimeWaves++;

    }
  }

  turrNode_t *turrNode = turrHead;
  while (turrNode != NULL) {
    if (turrNode->turret->stats->moreStats->type != NONE) {

      if ((turrNode->turret->cooldown + sizeCooldown(turrNode->turret->stats->moreStats->speed)) < GetSystemTime()) {
        turretAttack(turrNode->turret);
      }
    }
    turrNode = turrNode->next;
  }
}

void prettyMenuButtons(void) {
  if (isPlayButton(mousePos)) {
    DrawImage(PRETTY_PLAY, START_BUTTON_X, START_BUTTON_Y);
  } else if (isExitButton(mousePos)) {
    DrawImage(PRETTY_EXIT, EXIT_BUTTON_X, EXIT_BUTTON_Y);
  }
}

void prettyGameButtons(void) {
  if (sidePaneText == IMAGE_TEXT_DEFAULT) {
    if (isMenuButton(mousePos)) {
      DrawImage(PRETTY_MENU, MENU_BUTTON_X, MENU_BUTTON_Y);
    }
  } else if (sidePaneText == IMAGE_TEXT_BUILD) {
    if (isMenuButton(mousePos)) {
      DrawImage(PRETTY_MENU, MENU_BUTTON_X, MENU_BUTTON_Y);
    } else if (isOption1(mousePos)) {
      DrawImage(PRETTY_ARCHER, FIRST_BUTTON_X, FIRST_BUTTON_Y);
      DrawImageTr(IMAGE_HOVER_ARCHER, FIRST_BUTTON_X - 262, FIRST_BUTTON_Y);
    } else if (isOption2(mousePos)) {
      DrawImage(PRETTY_CANON, SECOND_BUTTON_X, SECOND_BUTTON_Y);
      DrawImageTr(IMAGE_HOVER_CANON, SECOND_BUTTON_X - 262, SECOND_BUTTON_Y);
    } else if (isOption3(mousePos)) {
      DrawImage(PRETTY_MAGE, THIRD_BUTTON_X, THIRD_BUTTON_Y);
      DrawImageTr(IMAGE_HOVER_MAGE, THIRD_BUTTON_X - 262, THIRD_BUTTON_Y);
    } else if (isCancel(mousePos)) {
      DrawImage(PRETTY_CANCEL, CANCEL_X, CANCEL_Y);
    }
  } else if (sidePaneText == IMAGE_TEXT_UPGRADE) {
    if (isMenuButton(mousePos)) {
      DrawImage(PRETTY_MENU, MENU_BUTTON_X, MENU_BUTTON_Y);
    } else if (isOption1(mousePos)) {
      DrawImage(PRETTY_SPEED, FIRST_BUTTON_X, FIRST_BUTTON_Y);
    } else if (isOption2(mousePos)) {
      DrawImage(PRETTY_RANGE, SECOND_BUTTON_X, SECOND_BUTTON_Y);
    } else if (isOption3(mousePos)) {
      DrawImage(PRETTY_DAMAGE, THIRD_BUTTON_X, THIRD_BUTTON_Y);
    } else if (isMove(mousePos)) {
      DrawImage(PRETTY_MOV, MOVE_X,MOVE_Y);
    } else if (isDelete(mousePos)) {
      DrawImage(PRETTY_DEL, DELETE_X, DELETE_Y);
    } else if (isCancel(mousePos)) {
      DrawImage(PRETTY_CANCEL, CANCEL_X, CANCEL_Y);
    }
  }
}

void prettyLevelPickButtons(void) {
  if(isLevel1(mousePos)) {
    DrawImage(PRETTY_L1, BUTTON_L1_X, BUTTON_L1_Y);
  } else if(isLevel2(mousePos)) {
    DrawImage(PRETTY_L2, BUTTON_L2_X, BUTTON_L2_Y);
  } else if(isLevel3(mousePos)) {
    DrawImage(PRETTY_L3, BUTTON_L3_X, BUTTON_L3_Y);
  } else if(isBack(mousePos)) {
    DrawImage(PRETTY_BACK, BUTTON_BACK_X, BUTTON_BACK_Y);
  }
}


void printCosts(void) {
  if (sidePaneText == IMAGE_TEXT_DEFAULT) {
  } else if (sidePaneText == IMAGE_TEXT_BUILD) {
    DrawText(itoa(BUILD_COST * (ARCHER + 1), 10), HEALTH_TEXT_X, FIRST_BUTTON_Y);
    DrawText(itoa(BUILD_COST * (CANON + 1), 10), HEALTH_TEXT_X, SECOND_BUTTON_Y);
    // NOTE:Hardcoded Mage to cost 500 since it is too OP
    DrawText(itoa(500, 10), HEALTH_TEXT_X, THIRD_BUTTON_Y);
  } else if (sidePaneText == IMAGE_TEXT_UPGRADE) {
    printUpgrades();
  }
}


// render game here -> drawing etc
void game_render(void) {
  mousePos = (pixelPoint_t) {g_iMousePosX, g_iMousePosY};

  if(gameStatus == GAME_INMENU) {
    DrawImage(IMAGE_LOADING, 512, 384);
    prettyMenuButtons();

  } else if(gameStatus == GAME_LEVELPICK) {
    DrawImage(IMAGE_LEVELPICK, 512, 384);
    prettyLevelPickButtons();
  } else if(gameStatus == GAME_INGAME) {

    switch(selectedLevel) {
      case 0:
        DrawImage(IMAGE_MAP_ONE, 384, 384);
        break;

      case 1:
        DrawImage(IMAGE_MAP_TWO, 384, 384);
        break;

      case 2:
        DrawImage(IMAGE_MAP_THREE, 384, 384);
        break;
    }

    // draw the tiles
    for (int i = 0 ; i < MAX_TILES; i++){
      for (int j = 0; j < MAX_TILES; j++){
        int x = i * TILE_PIXEL_SIZE + TILE_PIXEL_SIZE/2;
        int y = j * TILE_PIXEL_SIZE + TILE_PIXEL_SIZE/2;

        // NOTE: no longer required as of full map render
        // DrawImage  - BASE GRID
        // DrawImage(mapGrid[j][i].spriteLocation,x,y);

        // DrawOverlay turret
        // (here so the whole looping vars dont have to be computed it again)
        if(mapGrid[j][i].state == TURRET_OCCUPIED){
          // the first parts computes which sprite it needs
          DrawImage(T_TURRET_1 + mapGrid[j][i].turret->stats->moreStats->type, x, y);
        }
      }
    }
    // Draws Side PANEL
    if (sidePaneText == IMAGE_TEXT_DEFAULT) {
      DrawImage(IMAGE_TEXT_DEFAULT, SIDE_PANEL_X, SIDE_PANEL_Y);
    } else if (sidePaneText == IMAGE_TEXT_BUILD) {
      DrawImage(IMAGE_TEXT_BUILD, SIDE_PANEL_X, SIDE_PANEL_Y);
    } else if (sidePaneText == IMAGE_TEXT_UPGRADE) {
      DrawImage(IMAGE_TEXT_UPGRADE, SIDE_PANEL_X, SIDE_PANEL_Y);
    }

    // render blood sprites
    image_t *pBlood = totalSetOfImages[IMAGE_BLOOD];
    for(int i = 0; i < MAX_BLOOD_ANIMS; i++) {
      bloodanim_t *pCur = &g_sBloodAnims[i];
      if(pCur->active == 0)
        continue;

      ASM_DrawImageTr((void*)pBlood->pBinaryAddress,
          &pCur->info, pCur->posX, pCur->posY);
    }

    enemNode_t *enemNode = enemHead;
    while (enemNode != NULL) {
      DrawImageTr(enemNode->enemy->spriteLocation, enemNode->enemy->stats->location.x, enemNode->enemy->stats->location.y);
      enemNode = enemNode->next;
    }

    // render the projectiles
    for(int i = 0; i < MAX_PROJECTILES; i++) {
      projectile_t *pCur = &g_sProjectiles[i];
      if(!pCur)
        break;

      if(pCur->active == 0)
        continue;

      DrawImageTr(pCur->type, pCur->curPos.x, pCur->curPos.y);
    }

    prettyGameButtons();

    // fill in the side panel with needed text
    // timerVal = (int32_t)(float)( (prevSpawn - GetSystemTime())/ SEC_CONST
    //       + SEC_BETWEEN_SPAWNS * (waveSizeCalc(spawnedCounter) - waveSize));
    timerCounter = (waveStartTime + timerVal*SEC_CONST - GetSystemTime());// gets timer Val in mSec
    timerCounter = (int32_t)(float)(timerCounter / SEC_CONST); // gets into actual seconds
    if(timerCounter >= 0){
      DrawText(itoa(timerCounter, 10),TIME_COUNTER_X,TIME_COUNTER_Y);
    }
    else {
      DrawText(itoa(0,10),TIME_COUNTER_X,TIME_COUNTER_Y);
    }

    DrawText(itoa(gold, 10), GOLD_TEXT_X, GOLD_TEXT_Y);
    DrawText(itoa(lifePoints, 10), HEALTH_TEXT_X, HEALTH_TEXT_Y);

    printCosts();

  } else if(gameStatus == GAME_CREDITS) {
    DrawImage(IMAGE_CREDITS, 512, 384);
  }

  // MOUSE DRAW OVER EVERYTHING
  DrawImageTr(IMAGE_CURSOR,g_iMousePosX,g_iMousePosY);
}



// this is the main game loop
void game_loop(void) {
  if(initialized == 0) {
    DebugLED_ON();
    game_initialize();
    initialized = 1;
    DebugLED_OFF();
  }

  while(1) {
    // get the time in which we start this frame
	  int32_t curTime = GetSystemTime();
    int32_t timeDelta = curTime - oldTime;
    oldTime = curTime;

    game_update(inSeconds(timeDelta));
    game_render();

    // swap front and back buffers
    ASM_SwapBuffers();
  }
}
