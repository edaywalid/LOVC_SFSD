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

void openFile(File *file, char mode)
{
    if (mode == APPEND || mode == READ)
    {
        file->file = fopen(FILE_NAME, (mode == APPEND) ? "wb+" : "rb+");
        if (file->file == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
    }
}

void closeFile(File *file)
{
    fseek(file->file, 0, SEEK_SET);
    fwrite(&file->Header, sizeof(header), 1, file->file);
    fclose(file->file);
}

void readBloc(File *file, int position)
{

    fseek(file->file, sizeof(header) + position * sizeof(Buffer), SEEK_SET);
    fread(&buffer, sizeof(Buffer), 1, file->file);
}

void initBloc()
{
    buffer.next = -1;
    memset(buffer.charArray, '\0', MAX_SIZE);
}

void allocateBloc(File *file)
{

    initBloc();
    file->Header.FistFreePosition = 0;
}

void writeBloc(File *file, int position)
{

    fseek(file->file, sizeof(header) + position * sizeof(Buffer), SEEK_SET);
    fwrite(&buffer, sizeof(Buffer), 1, file->file);
}

char *studentToChar(student s)
{
    // Calculate the size needed for the string, including space for the null terminator
    size_t size = snprintf(NULL, 0, "%d$%s$%f$%d#", s.id, s.name, s.average, s.LogicallyDeleted) + 1;

    // // Allocate memory for the string
    size = size + countDigits(size) + 1;
    char *result = malloc(size);

    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Format the string
    snprintf(result, size, "%ld|%d$%d$%s$%f#", size, s.id, s.LogicallyDeleted, s.name, s.average);

    return result;
}




void insert(student s, File *file)
{
    int bloc, charPos;
    if (strcmp(getStudentFromLinkedList(s.id, &bloc, &charPos, file), "NOT FOUND") != 0)
    {
        printf("This student already exists\n");
        return;
    }
    char *studentChar = studentToChar(s);

    int currentBloc = bloc;
    char *tmpChar = malloc(MAX_SIZE * (file->Header.lastBloc + 1));

    int j = 0;
    int k = charPos;
    while (currentBloc <= file->Header.lastBloc)
    {
        readBloc(file, currentBloc);
        for (int l = k; l < MAX_SIZE; l++)
        {
            tmpChar[j++] = buffer.charArray[l];
        }
        currentBloc++;
        k = 0;
    }

    tmpChar[j] = '\0';
    readBloc(file, bloc);
    j = 0;
    while (studentChar[j] != '\0')
    {
        if (charPos == MAX_SIZE)
        {
            if (file->Header.lastBloc == bloc)
            {
               
                writeBloc(file, file->Header.lastBloc++);
                allocateBloc(file);
                bloc = file->Header.lastBloc;
                
            }
            else
            {
               
                writeBloc(file, bloc);
                readBloc(file, ++bloc);
            }
            charPos = 0;
        }
        buffer.charArray[charPos++] = studentChar[j++];
    }
    
    k = 0;
    

    while (tmpChar[k] != '\0')
    {
        
        if (charPos == MAX_SIZE)
        {
            if (file->Header.lastBloc == bloc)
            {
                writeBloc(file, file->Header.lastBloc++);
                allocateBloc(file);
                bloc = file->Header.lastBloc;
            }
            else
            {
                writeBloc(file, bloc);
                readBloc(file, ++bloc);
            }
            charPos = 0;
        }
        buffer.charArray[charPos++] = tmpChar[k++];
    }
    writeBloc(file, bloc );
    file->Header.FistFreePosition = charPos;
    free(tmpChar);
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
     else if (atoi(studentData) > key)
         {
             // here the insertion will be done before this student
             // the charPosition is already set to the beginning of the student
             // the blocPosition is already set to the bloc of the student
             return "NOT FOUND";
         }

         if (blocCount <= file->Header.lastBloc)
         {

             goto search;
         }
         else
         {
             *blocPosition = file->Header.lastBloc;
             *charPosition = file->Header.FistFreePosition; // indicate that the student should be inserted at the end of the file
             return "NOT FOUND";
         }
}
