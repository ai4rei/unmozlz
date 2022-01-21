/* disclaimer: this code is intentionally crappy, but it works */

#include <stdio.h>
#include <stdlib.h>

#include <btypes.h>
#include <bvecod.h>
#include <bvlz4.h>

int main(int nArgc, char** lppszArgv)
{
    ubyte_t* lpInBlock;
    ubyte_t* lpOuBlock;
    ulong_t uFileSize, uUncompSize;
    size_t uActualSize;
    FILE* hFile;

    if(nArgc<2)
    {
        puts("unmozlz <file>");
        return 0;
    }

    hFile = fopen(lppszArgv[1], "rb");
    fseek(hFile, 0, SEEK_END);
    uFileSize = ftell(hFile);
    fseek(hFile, 0, SEEK_SET);
    lpInBlock = malloc(uFileSize);
    fread(lpInBlock, 1, uFileSize, hFile);
    fclose(hFile);
    if(strcmp(lpInBlock, "mozLz40")!=0) return 1;
    uUncompSize = *((ulong_t*)&lpInBlock[8]);
    lpOuBlock = malloc(uUncompSize);
    BvLz4BlockDecode(&lpInBlock[12], uFileSize-12, lpOuBlock, uUncompSize, &uActualSize);
    if(uUncompSize!=uActualSize) return 1;
    hFile = fopen("out.json", "wb");
    fwrite(lpOuBlock, 1, uActualSize, hFile);
    fclose(hFile);
    free(lpOuBlock);
    free(lpInBlock);
    return 0;
}
