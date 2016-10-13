#include "imperial_defense.h"
#include <uspios.h>

int32_t turret_list_length = 0;
turrNode_t *turrHead = NULL;
int32_t uniqueTurrIDCounter;


int32_t getTurretListLength(void) {
  return turret_list_length;
}

// adds a new node at the head of the list
turrNode_t *addTurr(void) {
  turrNode_t *temp = (turrNode_t *) malloc(sizeof(turrNode_t));
  turret_t *turret = (turret_t *) malloc(sizeof(turret_t));
  turretStats_t *stats = (turretStats_t *) malloc(sizeof(turretStats_t));
  advancedTurretStats_t *moreStats = (advancedTurretStats_t *)malloc(sizeof(advancedTurretStats_t));
  temp->turret = turret;
  temp->turret->stats = stats;
  temp->turret->stats->moreStats = moreStats;
  temp->next = turrHead;
  temp->turret->uniqueID = uniqueTurrIDCounter;
  uniqueTurrIDCounter++;
  turrHead = temp;

  return turrHead;
}

void removeTurr(turret_t *turret) {
  turrNode_t *current = turrHead;
  turrNode_t *previous = NULL;

  if (turrHead == NULL) {
    return;
  }

  while (current->turret->uniqueID != turret->uniqueID) {
    previous = current;
    current = current->next;
  }

  if (current == turrHead) {
    turrHead = turrHead->next;
  } else {
    previous->next = current->next;
  }
  free(current->turret->stats->moreStats);
  free(current->turret->stats);
  free(current->turret);
  free(current);
}
