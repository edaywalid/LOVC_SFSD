#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract.h"

Buffer buffer;

void initFile(File *file)
{
    File newFile;
    openFile(&newFile, APPEND);
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


void delete(int key, File *file)
{

    printf("see if it enters here 1\n");
    int blocPosition, charPosition;
    char *str = getStudentFromLinkedList(key, &blocPosition, &charPosition, file);
    if (strcmp(str, "NOT FOUND") != 0)
    {

        printf("see if it enters here 2\n");
        readBloc(file, blocPosition);
        int k = charPosition;

        while (buffer.charArray[k] != '|')
        {

            printf("see if it enters here 3\n");
            k++;
            if (k == MAX_SIZE)
            {
                k = 0;
                readBloc(file, ++blocPosition);
            }
        }

        while (buffer.charArray[k] != '$')
        {
            k++;
            if (k == MAX_SIZE)
            {
                k = 0;
                readBloc(file, ++blocPosition);
            }
        }
        buffer.charArray[++k] = '1';
        writeBloc(file, blocPosition);
        printf("Student %d Deleted!\n", key);
    }

    else
    {
        printf("This Id Don't Exicte\n");
    }
}