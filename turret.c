#include "imperial_defense.h"
#include "backbone.h"

//TODO: set this :D
int32_t initRangeArr[MAX_TURR_TYPE] = {150, 100, 999}; // range in pixels
int32_t initSpeedArr[MAX_TURR_TYPE] = {14, 8, 4};
int32_t initDamageArr[MAX_TURR_TYPE] = {7, 11, 3};

int32_t isWithinRange(pixelPoint_t location1, pixelPoint_t location2, int32_t range);
void turretAttack(turret_t *attacker_turret);

void upgradeTurret(gridPoint_t pos, int32_t upgradeType);
void buildTurret(gridPoint_t pos, int32_t turretType);
void moveTurret(gridPoint_t from, gridPoint_t to);
void demolishTurret(gridPoint_t pos);

void mageAttack(turret_t *turret);
void singleTargetTurretAttack(turret_t *turret);
void doAttack(turret_t *turret);

// projectile stuff
projectile_t g_sProjectiles[MAX_PROJECTILES];

void projectileLaunch(pixelPoint_t *pStartPos, pixelPoint_t *pEndPos, int32_t type) {
  for(int i = 0; i < MAX_PROJECTILES; i++) {
    projectile_t *pCur = &g_sProjectiles[i];
    if(!pCur)
      break;

    if(pCur->active == 1)
      continue;

    pCur->curPos.x = pStartPos->x;
    pCur->curPos.y = pStartPos->y;
    pCur->endPos.x = pEndPos->x;
    pCur->endPos.y = pEndPos->y;
    pCur->active = 1;
    pCur->type = type;
    pCur->directionX = pEndPos->x - pStartPos->x;
    pCur->directionY = pEndPos->y - pStartPos->y;
    float temp = Math_SQRT(pCur->directionX * pCur->directionX +
      pCur->directionY * pCur->directionY);
    pCur->directionX /= temp;
    pCur->directionY /= temp;
    return;
  }
}

// -----------------IMPLEMENTATIONS-------------------------

// Helper function to check if enemy is withing given range
int32_t isWithinRange(pixelPoint_t location1, pixelPoint_t location2, int32_t range) {
  return ((range*range) > (((location1.y - location2.y)*(location1.y - location2.y)) + ((location1.x - location2.x)*(location1.x - location2.x)))); // euclidean distance allows for circular range
}

enemy_t *findTarget(turret_t *turret) {
  enemNode_t *enemNode = enemHead;
  while (enemNode != NULL) {
    if (isWithinRange(enemNode->enemy->stats->location,
        turret->stats->pixelLocation,
        turret->stats->moreStats->range)) {
      return enemNode->enemy;
    }
    enemNode = enemNode->next;
  }
  return NULL;
}

int32_t sizeCooldown(int32_t turretSpeedLevel) {
  if(MAX_TURRET_SPEED < turretSpeedLevel){
    return SEC_HALF_CONST;
  }
  return ((int32_t)( (float) MAX_TURRET_SPEED / (float) turretSpeedLevel * (float) SEC_HALF_CONST) );
}

// turret is looping and finding the enemy who has been spawned earliest
// and if the enemy is still alive and is within range of turret, then it
// gets shot, if he's dead then we iterate to the next enemy until there
// are no enemies left. Returns 1 if hit was successful and 0 if not

void turretAttack(turret_t *turret) {
  if(turret->stats->moreStats->type == MAGE) {
    mageAttack(turret);

  } else if (turret->stats->moreStats->type == ARCHER) {
    singleTargetTurretAttack(turret);
  } else if (turret->stats->moreStats->type == CANON) {
    singleTargetTurretAttack(turret);
  }
}


void upgradeTurret(gridPoint_t position, int32_t upgradeType) {
  //pre: pos is in mapGrid and upgradeType is 1-3
  //  NOTE: might have added a bracket or not :D
  // turret_t *myTurret = mapGrid[position.x][position.y].turret;
  //  if (upgradeType == RANGE_MASK && (gold >= myTurret->stats->moreStats->range*UPGRADE_COST) ) {
  //    gold -= (myTurret->stats->moreStats->range * UPGRADE_COST;
  //    myTurret->stats->moreStats->range += UPGRADE_RANGE_AMOUNT;
  //
  // } else if (upgradeType == DAMAGE_MASK && (gold >= myTurret->stats->moreStats->damage*UPGRADE_COST) ) {
  //    gold -= (myTurret->stats->moreStats->damage * UPGRADE_COST;
  //    myTurret->stats->moreStats->damage += UPGRADE_DAMAGE_AMOUNT;
  //
  // } else if (upgradeType == SPEED_MASK && (gold >= myTurret->stats->moreStats->speed*UPGRADE_COST) ) {
  //    gold -= myTurret->stats->moreStats->speed * UPGRADE_COST;
  //    myTurret->stats->moreStats->speed += UPGRADE_SPEED_AMOUNT;
  //
  // }
 // NOTE: Tuesday
  advancedTurretStats_t *stats = mapGrid[position.x][position.y].turret->stats->moreStats;
   if (upgradeType == RANGE_MASK
            && (gold >= (stats->range - initRangeArr[stats->type] + UPGRADE_RANGE_AMOUNT) / PIXELS_PER_UPGRADE * UPGRADE_COST) ) {
     gold -= (stats->range - initRangeArr[stats->type] + UPGRADE_RANGE_AMOUNT) / PIXELS_PER_UPGRADE * UPGRADE_COST;
     stats->range += UPGRADE_RANGE_AMOUNT;

  } else if (upgradeType == DAMAGE_MASK
            && (gold >= (stats->damage - initDamageArr[stats->type] + UPGRADE_DAMAGE_AMOUNT) * UPGRADE_COST) ) {
     gold -= (stats->damage - initDamageArr[stats->type] + UPGRADE_DAMAGE_AMOUNT) * UPGRADE_COST;
     stats->damage += UPGRADE_DAMAGE_AMOUNT;

  } else if (upgradeType == SPEED_MASK
            && (gold >= (stats->speed - initSpeedArr[stats->type] + UPGRADE_SPEED_AMOUNT) * UPGRADE_COST) ) {
     gold -= (stats->speed - initSpeedArr[stats->type] + UPGRADE_SPEED_AMOUNT) * UPGRADE_COST;
     stats->speed += UPGRADE_SPEED_AMOUNT;

  }

  return;
}



void buildTurret(gridPoint_t position, int32_t turretType) {
  //pre: proper type,checks if the block is buildable
  //post: allocate and init a new turret
  int32_t cost = BUILD_COST + BUILD_COST * turretType;

  //NOTE: hardcoded MAGE to cost 500
  if( turretType == MAGE) {
    cost = 500;
  }
  if(gold < cost){
    return;
  }

  turrNode_t *turretNode = addTurr();
  mapGrid[position.x][position.y].state = TURRET_OCCUPIED;
  mapGrid[position.x][position.y].turret = turretNode->turret;
  turretNode->turret->stats->moreStats->range = initRangeArr[turretType];
  turretNode->turret->stats->moreStats->speed = initSpeedArr[turretType];
  turretNode->turret->stats->moreStats->damage = initDamageArr[turretType];
  turretNode->turret->stats->moreStats->type = turretType;
  turretNode->turret->stats->gridLocation = position;
  turretNode->turret->stats->pixelLocation = convertGridtoGridPixelCenter(position);

  // turretNode->turret->target = NULL;
  turretNode->turret->cooldown = GetSystemTime();

  gold -= cost;

  return;
}



void moveTurret(gridPoint_t from, gridPoint_t to) {
  tile_t *dest = &mapGrid[to.x][to.y];
  tile_t *src = &mapGrid[from.x][from.y];
  if (dest->state == BUILD_ZONE) {

    dest->state = TURRET_OCCUPIED;
    dest->turret = src->turret;
    dest->turret->stats->gridLocation = to;
    dest->turret->stats->pixelLocation = convertGridtoGridPixelCenter(to);

    //delete
    //NOTE : no need to free_t() since it still exist but on another tile
    src->state = BUILD_ZONE;
    src->turret = NULL;

  } else {
    // cancel move
    sidePaneText = IMAGE_TEXT_DEFAULT;
  }

  return;
}



void demolishTurret(gridPoint_t pos) {
  gold += DEMOLISH_GOLD;
  mapGrid[pos.x][pos.y].state = BUILD_ZONE;
  turret_t *myTurret = mapGrid[pos.x][pos.y].turret;
  removeTurr(myTurret);
  mapGrid[pos.x][pos.y].turret->stats->moreStats->type = NONE;
  mapGrid[pos.x][pos.y].turret = NULL;
  sidePaneText = IMAGE_TEXT_DEFAULT;

  return;
}


// TURRET OF GLOBAL DAMAGE AND RANGE that HIT ALL OF enemies
// good analogy is the Armaggeddon in Heroes 3
// should deal low damage
// really slow

void mageAttack(turret_t *turret){
  enemNode_t *enemNode = enemHead;
  while (enemNode != NULL) {

    // launch the projectile
    projectileLaunch(&turret->stats->pixelLocation,
      &enemNode->enemy->stats->location, PROJ_MAGIC);

    enemy_hit(enemNode->enemy, turret->stats->moreStats->damage);
    enemNode = enemNode->next;
  }
   turret->cooldown = GetSystemTime();

}



// SINGLE TARGET TURRET WITH PROJECTILE AND MID-RANGE
// mid damage
// speed above avarage
// AOE TURRET WITH SMALL AOE FIELD(FIXED)  AND SHORT TO MID RANGE
// high damage
// speed low to mid

void singleTargetTurretAttack(turret_t *turret) {
  // updates life and frees enemy memory if dead
  //TODO: projectile handler;
  enemy_t *enemy = findTarget(turret);
  if (enemy == NULL) {
    return;
  }
  // launch the projectile
  projectileLaunch(&turret->stats->pixelLocation, &enemy->stats->location,
    (turret->stats->moreStats->type==ARCHER)?PROJ_ARROW:PROJ_CANNON);

  enemy_hit(enemy,turret->stats->moreStats->damage);

  turret->cooldown = GetSystemTime();
}
