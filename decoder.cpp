#include "decoder.h"

int nextByteIsEOF(FILE* f0)
{
    if(feof(f0))
        return 1;

    char buf2;
    size_t i = fread(&buf2, sizeof(char), 1, f0);
    if(!i && feof(f0))
        return 1;
    else
        setAHeadByte((unsigned char)buf2);
    return 0;
}

unsigned int getTail(FILE *f0, unsigned char *readByte, int *currentIndex)
{
    unsigned int value = 0;
    for (int i = 0; i < 3; ++i)
    {
        value <<= (unsigned int)1;
        value += readBit(f0, readByte, currentIndex);
        if(checkError())
            return 0;
    }
    return value;
}

void bitDecode(FILE* f0, FILE* f1, unsigned char* readByte, int* currentIndex, Node* tree, int tail)
{
    unsigned char buf;
    unsigned long long size = 0;
    int lastSizeCode;

    while(true)
    {
        lastSizeCode = 0;

        buf = readCode(f0, tree, readByte, currentIndex, &lastSizeCode);

        if(checkError())
            return;
        else
        {
            size += lastSizeCode;
            fwrite(&buf, sizeof(char), 1, f1);
        }

        if(size % 8 == tail && nextByteIsEOF(f0))
        {
            returnError();
            break;
        }
    }
}

unsigned char readCode(FILE *f0, const Node* tree, unsigned char *readBytev, int *currentIndex, int* lengthBuf)
{
    Node node = *tree;
    while(node.left || node.right)
    {
        if(readBit(f0, readBytev, currentIndex))
        {
            if(checkError())
                return 0;
            if(!node.right)
            {
                throwError();
                return  0;
            }
            node = *node.right;
        }else
        {
            if(checkError())
                return 0;

            if(!node.left)
            {
                throwError();
                return  0;
            }
            node = *node.left;
        }
        (*lengthBuf)++;
    }
    return node.symbol;
}
