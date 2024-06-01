#ifndef INDEX_H
#define INDEX_H
#include <stdio.h>
#include "./abstract.h"
#define indexTitle "index.txt"
typedef struct Index
{
    int id;
    int bloc;
    int position;
} Index;


typedef struct indexNode * list ;
typedef struct indexNode {
    Index indexes[10] ;
    int nb ;
    list next ;
}indexNode ;



list loadIndexToRam(File file);
Index search(int id, File file);


#endif // INDEX_H