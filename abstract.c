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


