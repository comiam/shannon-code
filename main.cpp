#include <fcntl.h>
#include "encoder.h"
#include "decoder.h"

int shannon(bool compress, FILE* f0, FILE* f1);

int main(int argc, char* argv[])
{
    FILE* f0;
    FILE* f1;
    char* code;
    bool compress;

    if(argc == 4)
    {
        code = argv[1];

        if(strcmp("-c", code) && strcmp("-d", code) && strcmp("c", code) && strcmp("d", code))
            argError

        compress = !strcmp("-c", code) || !strcmp("c", code);

        f0 = fopen(argv[2], "rb");
        f1 = fopen(argv[3], "wb");
    }
    else
        argError

    _setmode(_fileno(f0), _O_BINARY);
    _setmode(_fileno(f1), _O_BINARY);

    return shannon(compress, f0, f1);
}

int shannon(bool compress, FILE* f0, FILE* f1)
{
    if(!f0 || !f1)
    {
        fileError
        if(f0)
            fclose(f0);
        if(f1)
            fclose(f1);

        return 0;
    }

    if(compress)
    {
        auto* probTable = getProbTable(f0);
        auto* codeTable = getCodeTable(probTable);
        auto* headTree  = getTree(probTable, codeTable);

        bitEncode(f0, f1, headTree, codeTable);
        clearTree(headTree);

        delete [] probTable;
        delete codeTable;
    }else
    {
        unsigned char packageByte = 0;
        int packageIndex = 0;
        fread(&packageByte, sizeof(char), 1, f0);

        auto tail = getTail(f0, &packageByte, &packageIndex);
        checkErrorReading(fclose(f0), fclose(f1), ;)

        Node* headTree = readTree(f0, &packageByte, &packageIndex);
        checkErrorReading(fclose(f0), fclose(f1), clearTree(headTree))

        gotoNextByte(f0, &packageByte, &packageIndex);
        checkErrorReading(fclose(f0), fclose(f1), clearTree(headTree))

        bitDecode(f0, f1, &packageByte, &packageIndex, headTree, tail);
        checkErrorReading(fclose(f0), fclose(f1), clearTree(headTree))

        clearTree(headTree);
    }

    fclose(f0);
    fclose(f1);

    return 0;
}
