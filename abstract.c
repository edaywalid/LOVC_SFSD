#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract.h"

Buffer buffer;

void initFile(File *file)
{
    File newFile;
    openFile(&newFile, 'A');
    newFile.Header.firstBloc = 0;
    newFile.Header.lastBloc = 0;
    newFile.Header.FistFreePosition = 0;
    *file = newFile;
}

























char *getStudentFromLinkedList(int key, int *blocPosition, int *charPosition, File *file)
{

    *blocPosition = 0;
    *charPosition = 0; // indicate that the student should be inserted at the beginning of the file
    int i = 0;
    int blocCount = 0;
    char *studentData = malloc(100);

    readBloc(file, blocCount);
    if (file->Header.lastBloc == 0 && file->Header.FistFreePosition == 0)
    {
        return "NOT FOUND";
    }
search:

    while (buffer.charArray[i] != '|')
    {

        if (buffer.charArray[i] == '#')
        {

            *charPosition = i == MAX_SIZE - 1 ? 0 : i + 1; // always set the charPosition the beginning of any inserted student
        }

        i++;

        if (i == MAX_SIZE)
        {

            readBloc(file, ++blocCount);
            i = 0;
        }
    }

    int k = 0;
    i++;

    if (i == MAX_SIZE)
    {
        readBloc(file, ++blocCount);
        i = 0;
    }

    while (buffer.charArray[i] != '$')
    {

        studentData[k++] = buffer.charArray[i++];
        if (i == MAX_SIZE)
        {
            readBloc(file, ++blocCount);
            i = 0;
        }
    }

    studentData[k] = '\0';
    *blocPosition = blocCount; // indicate that this is the bloc to insert ;
    if (atoi(studentData) == key)
    {
        if (i < MAX_SIZE - 1 && buffer.charArray[i + 1] == '1' || i == MAX_SIZE - 1 && buffer.charArray[0] == '1')
        {
            return "NOT FOUND";
        }
        int tmp = blocCount;
        while (buffer.charArray[i] != '#')
        {
            if (i == MAX_SIZE)
            {
                readBloc(file, ++tmp);
                i = 0;
            }
            studentData[k++] = buffer.charArray[i++];
        }
        studentData[k] = '\0';
        return studentData;
    }
}