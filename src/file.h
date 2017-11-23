#pragma once

#include <stdio.h>


enum class FileMode
{
    Read,
    Write,
    Append
};


class File
{
public:
    File();
    virtual ~File();

    bool open(const char* path, const FileMode& mode);
    void close();

    int read(char* buffer, int maxLength);

    int readLine(char* buffer, int maxLength);

    int getChar();

    static int size(const char* path);

    static bool exists(const char* path);

private:
    FILE* pFile;

};
