#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <stdio.h>
#define MAX_SIZE 100
#define FILE_NAME "file.bin"
#define APPEND 'A'
#define READ 'R'


typedef struct Bloc
{
    char charArray[MAX_SIZE];
    int next;
} Buffer;

extern Buffer buffer;

typedef struct student
{
    char *name;
    int id;
    float average;
    int LogicallyDeleted;
} student;

typedef struct header {
    int firstBloc ; 
    int lastBloc ;
    int FistFreePosition ;
}header ;

typedef struct File {
    header Header ;
    FILE *file ;
}File ;

// FILE RELATED METHODS
void initFile(File *file);
void openFile(File *file, char  mode);
void closeFile(File *file);
void writeHeader(File *file);
header readHeader(File *file);
void writeBloc(File *file, int position);
void readBloc(File *file, int position);
void allocateBloc(File *file);


// STUDENT RELATED METHODS
char *studentToChar(student s);
student charToStudent(char *s);
void insert(student s, File *file);
int delete(int key, File *file);
// BLOC RELATED METHODS
void initBloc();

void getLastInsertedStudent(File *file, student *student);
void getWholeLastBloc(File *file, char *string);

void printAllBlocs(File *file);


char *getStudentFromLinkedList(int key, int *blocPosition, int *charPosition, File *file);
void extractAllStudentsToFile(const char *outputFileName, File *file , int isExtracting);
#endif // ABSTRACT_H