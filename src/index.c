#include "../include/index.h"
#include "../include/abstract.h"
#include <stdio.h>
#include <stdlib.h>

list loadIndexToRam(File file) {

  list head = malloc(sizeof(indexNode));
  head->next = NULL;
  head->nb = 0;
  list current = head;

  int blocCount = 0;
  readBloc(&file, blocCount);
  int pos = 0;
  int bloc = 0;
  int i = 0;
  while (blocCount <= (&file)->Header.lastBloc) {

    while (blocCount <= (&file)->Header.lastBloc && i < MAX_SIZE &&
           buffer.charArray[i] != '|') {
      if (buffer.charArray[i] == '#') {
        pos = i == MAX_SIZE - 1 ? 0 : i + 1;
        bloc = i == MAX_SIZE - 1 ? blocCount + 1 : blocCount;
      }
      i++;
      if (i == MAX_SIZE) {
        readBloc((&file), ++blocCount);
        i = 0;
      }
    }
    if (blocCount > (&file)->Header.lastBloc)
      break;
    int k = 0;
    i++;
    if (i == MAX_SIZE) {
      readBloc((&file), ++blocCount);
      i = 0;
    }

    char *studentID = malloc(100);
    pos = i;
    bloc = blocCount;
    while (i < MAX_SIZE && buffer.charArray[i] != '$') {
      studentID[k++] = buffer.charArray[i++];
      if (i == MAX_SIZE) {
        readBloc((&file), ++blocCount);
        i = 0;
      }
    }
    studentID[k] = '\0';

    if (i < MAX_SIZE - 1 && buffer.charArray[i + 1] == '1' ||
        i == MAX_SIZE - 1 && buffer.charArray[0] == '1') {
      free(studentID);
      continue;
    }

    Index index = {atoi(studentID), bloc, pos};
    if (current->nb == 10) {
      current->next = malloc(sizeof(indexNode));
      current = current->next;
      current->next = NULL;
      current->nb = 0;
    }

    current->indexes[current->nb++] = index;

    free(studentID);
    if (i == MAX_SIZE)
      i = 0;
  }

  return head;
}

Index search(int id, File file) {
  int bloc = -1;
  int pos = -1;
  list head = loadIndexToRam(file);
  list current = head;
  while (current != NULL) {
    for (int i = 0; i < current->nb; i++) {
      if (current->indexes[i].id == id) {
        bloc = current->indexes[i].bloc;
        pos = current->indexes[i].position;
        break;
      }
    }
    current = current->next;
  }
  if (bloc == -1) {
    printf("THIS USER DOESN'T EXIST\n");
    return (Index){-1, -1, -1};
  }
  return (Index){id, bloc, pos};
}
