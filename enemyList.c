#include "imperial_defense.h"
#include <uspios.h>

int32_t enemy_list_length = 0;
enemNode_t *enemHead = NULL;
int32_t uniqueEnemIDCounter;

int32_t getEnemyListLength(void) {
  return enemy_list_length;
}

// adds a new node at the tail of the list
enemNode_t *addEnem(void) {

  enemNode_t *temp = (enemNode_t *) malloc(sizeof(enemNode_t));
  enemy_t *enemy = (enemy_t *) malloc(sizeof(enemy_t));
  enemyStats_t *stats = (enemyStats_t *) malloc(sizeof(enemyStats_t));
  temp->enemy = enemy;
  temp->enemy->stats = stats;
  temp->next = NULL;
  temp->enemy->uniqueID = uniqueEnemIDCounter;
  uniqueEnemIDCounter++;

  if (enemHead == NULL) {
    enemHead = temp;
    return enemHead;
  }

  enemNode_t *iterator = enemHead;
  while (iterator->next != NULL) {
    iterator = iterator->next;
  }
  iterator->next = temp;

  return iterator->next;
}

void removeEnem(enemy_t *enemy) {
  enemNode_t *current = enemHead;
  enemNode_t *previous = NULL;

  if (enemHead == NULL) {
    return;
  }

  while (current->enemy->uniqueID != enemy->uniqueID) {
    if (current->next == NULL) {
      return;
    } else {
      previous = current;
      current = current->next;
    }
  }

  if (current == enemHead) {
    enemHead = enemHead->next;
  } else {
    previous->next = current->next;
  }
  free(current->enemy->stats);
  current->enemy->stats = NULL;
  free(current->enemy);
  current->enemy = NULL;

  free(current);
  current = NULL;

}
