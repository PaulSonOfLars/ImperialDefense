#include "imperial_defense.h"
#include "backbone.h"
#include "images.h"

bloodanim_t g_sBloodAnims[MAX_BLOOD_ANIMS];
gridPoint_t endOfMap[MAX_LEVELS] = {
  {10,10},{7,5},{5,5}
};

void addSomeGore(int32_t posX, int32_t posY) {
  for(int i = 0; i < MAX_BLOOD_ANIMS; i++) {
    bloodanim_t *pCur = &g_sBloodAnims[i];
    if(pCur->active == 1)
      continue;

    imageInfo_t *pOrigInfo = &totalSetOfImages[IMAGE_BLOOD]->info;

    pCur->posX = posX;
    pCur->posY = posY;
    pCur->info.width = pOrigInfo->width;
    pCur->info.height = pOrigInfo->height;
    pCur->info.widthReal = pOrigInfo->widthReal;
    pCur->info.currentFrame = 0;
    pCur->info.timeElapsed = 0.0f;
    pCur->active = 1;

    return;
  }
}

// need to change this entirely
pixelPoint_t convertGridtoGridPixelCenter(gridPoint_t gridLocation){
  int32_t y = gridLocation.x * TILE_PIXEL_SIZE + TILE_PIXEL_SIZE/2;
  int32_t x = gridLocation.y * TILE_PIXEL_SIZE + TILE_PIXEL_SIZE/2;
  return (pixelPoint_t) {x,y};
}

// return 1 if a<b --- -1 if a>b ---- 0 if equal
//so you know which direction you should scale your move vector
int32_t signOfDirectionVector(int a, int b){
   if ( a == b ) {
     return 0;
   } else if (a > b) {
     return -1;
   } else {
     return 1;
   }
}

// function which moves an enemy minion in the right direction
// with speed pixels

void enemy_move(enemy_t *enemy, float timeDelta) {

  pixelPoint_t currentLocation = enemy->stats->location;

  if(enemyRoute[enemy->currentRoute -1].x == endOfMap[selectedLevel].x  && enemyRoute[enemy->currentRoute -1].y == endOfMap[selectedLevel].y){
          // despawn and hurt you
          lifePoints--;

          removeEnem(enemy);

          if(lifePoints == 0){
            gameStatus = GAME_CREDITS;
          }
  } else {
    pixelPoint_t destination = convertGridtoGridPixelCenter(enemyRoute[enemy->currentRoute]);

    // the enemy did reach the desired point and therefore continues to another one
    if (currentLocation.x == destination.x && currentLocation.y == destination.y) {
      (enemy->currentRoute)++;
    }ta
    else if(currentLocation.x == destination.x) { //means it should move a straight line on Yaxis
      enemy->stats->location.y += signOfDirectionVector(enemy->stats->location.y,destination.y) * (enemy->stats->speed * timeDelta);
    } else if (currentLocation.y == destination.y) {
      enemy->stats->location.x += signOfDirectionVector(enemy->stats->location.x, destination.x) * (enemy->stats->speed * timeDelta);
    }
    // this is the end since the next route where the enemy can go is NULL
  }
  return;
}



// 1 if enemy is dead
int32_t enemy_dead(enemy_t *enemy) {
  return enemy->stats->health <= 0;
}


void spawnEnemy(void) {
  enemNode_t *newEnemy = addEnem();
// NOTE: Tuesday
  newEnemy->enemy->stats->health = ENEMY_INIT_HEALTH + allTimeWaves * ENEMY_HEALTH_INCREASE ;
  newEnemy->enemy->stats->speed = ENEMY_INIT_SPEED;
  newEnemy->enemy->currentRoute = 0;
  newEnemy->enemy->stats->location = convertGridtoGridPixelCenter(enemyRoute[0]);
  newEnemy->enemy->spriteLocation = (rand() % 3) ? T_ENEMY_2 : T_ENEMY_1;

}


// 1 is killed 0 still lives.. used to tell whether the turret needs to find new
// front target
int32_t enemy_hit(enemy_t *enemy, int32_t damage) {
  enemy->stats->health -= damage;
  if (enemy_dead(enemy)) {
    gold += KILL_LOOT;
    addSomeGore(enemy->stats->location.x, enemy->stats->location.y);
    removeEnem(enemy);
    return 1;
  }
  return 0;
}

int32_t waveSizeCalc(int32_t totalSpawned) {
    if( totalSpawned < 20){
      return 5;
    } else if( totalSpawned < 50){
      return 9;
    }else if( totalSpawned < 100){
      return 13;
    }else if( totalSpawned < 170){
      return 17;
    }else {
      return 25;
    }
}
