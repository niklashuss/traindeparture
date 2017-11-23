#include "file.h"

File::File() : pFile(nullptr)
{
}

void File::close()
{
    if (pFile != nullptr)
    {
        fclose(pFile);
        pFile = nullptr;
    }
}

File::~File()
{
    close();
}

bool File::open(const char* path, const FileMode& mode)
{
    if (mode == FileMode::Read)
    {
        pFile = fopen(path, "r");
    }
    else if (mode == FileMode::Write)
    {
        pFile = fopen(path, "w");
    }
    else if (mode == FileMode::Append)
    {
        pFile = fopen(path, "a");
    }
    return pFile != nullptr;
}

int File::read(char* buffer, int maxLength)
{
    if (pFile == nullptr)
    {
        return -1;
    }
    fread(buffer, 1, maxLength, pFile);
    return 0;
}

int File::readLine(char* buffer, int maxLength)
{
    if (pFile == nullptr)
    {
        return -1;
    }

    int count = 0;
    for(int i = 0; i < maxLength; i++)
    {
        int c = fgetc(pFile);
        if (c == 13)
        {
            printf("ignoring 13\n");
            continue;
        }
        if (c == EOF)
        {
            printf("EOF found\n");
            count = -1;
            break;
        }
        if (c == 10)
        {
            printf("found 10 so aborting\n");
            break;
        }
        buffer[count] = (char)c;
        count++;
    }
    return count;
}

int File::getChar() {
    return fgetc(pFile);
}

int File::size(const char* path)
{
    FILE* file = fopen(path, "rt");

    if (file == nullptr)
    {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);
    return size;
}

bool File::exists(const char* path)
{
    FILE* file = fopen(path, "r");

    if (file == nullptr)
    {
        return false;
    }

    fclose(file);
    return true;
}
