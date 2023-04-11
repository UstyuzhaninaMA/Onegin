// file.open -> buffer -> fclose

// for sym in buffer:
//      if sym == '\n':
//         arr[i].len = &sym - arr[i].str
//         arr[++i].str = &sym

// strtok() ???????? Как ее здесь применить?

// skip empty "\n\n" ????? Это в 

// output to file

// your very own sort

// reverse comparator

#include <stdio.h>
//#include <sys\stat.h>  не подключается не понимаю как с ней работать...
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBAG 0
#define STRIGHT_CMP 1
#define REV_CMP !STRIGHT_CMP


enum Constants {
    NUM_OF_STR = 7,
};

struct Line {
    size_t len;
    char  *begin;
};

struct Text {
    char  *begin;
    size_t buflen;
    Line*  arrLine[NUM_OF_STR];
};

char *fileRead ( Text *textStruct);
void writeLines( Text *text);
int divToStr(    Text *textStuct);
void freeAll(Text *textStruct);
int compareLines(const void *argA, const void *argB);
int revCompareLines(const void *argA, const void *argB);

// TODO: use fread
// TODO: logging only in debug

int main() {

    struct Text text = {};

    #if DEBAG
    FILE *logs;
    if ((logs = fopen("logs.txt", "w")) == NULL) {
        fprintf(logs, "---Cannot open file.\n");
        return 1;
    }
    #endif


    //чтение
    text.begin = fileRead(&text);
    if(text.begin == NULL) {
        printf("---Error in return value from fileread");
        return 1;
    }

    
    //printf("%s  -  %s \n", text.arrLine[0]->begin, text.arrLine[1]->begin);


    //compare
    #if STRIGHT_CMP
    qsort(*text.arrLine, NUM_OF_STR, sizeof(Line), compareLines);
    #endif

    #if REV_CMP
    qsort(*text.arrLine, NUM_OF_STR, sizeof(Line), revCompareLines);
    #endif

    //запись в файл
    writeLines(&text); 

    #if DEBAG
    fclose(logs);
    #endif

    freeAll(&text);

    return 0;
}

char *fileRead(struct Text *textStruct) {
    
    FILE *file = NULL;
    file = fopen("text.txt", "r");
    if (file == NULL) {
        printf("---Cannot open file.\n");
        return NULL;
    }

    //Определение длины текста в файле
    // TODO: use stat() -----------how???
    fseek(file, 0, SEEK_END);
    textStruct->buflen = ftell(file) + 1;
    rewind(file);
    #if DEBAG
        printf("Buflen: %zu \n", textStruct->buflen);
    #endif
    textStruct->begin = (char *)calloc(sizeof(char), textStruct->buflen);

    // TODO: check fread return value \n\r -> \r
    fread(textStruct->begin, sizeof(char), textStruct->buflen, file);
    #if DEBAG
    printf("read string: %s \n", textStruct->begin);
    #endif
    fclose(file);

    divToStr(textStruct);

    return textStruct->begin;
}

int revCompareLines(const void *argA, const void *argB) {
    assert(argA != NULL);
    assert(argB != NULL);
    
    const char *A = ((Line*)argA)->begin;
    const char *B = ((Line*)argB)->begin;

    //printf("In revlineCmp:\n%s vs %s\n", A, B);
    int i;
    int lenA = ((Line*)argA)->len;
    int lenB = ((Line*)argB)->len;

    for(i = 0; (A[i] != '\0') && (B[i] != '\0'); i ++) {
        //if for ignore sign
        if(A[lenA - i] != B[lenB - i]) {
            return A[lenA - i] - B[lenB - i];
        }
    }
    if(A[lenA - i] != B[lenB - i]) {
        return A[lenA - i] - B[lenB - i];
    }
    return 0;
}
int compareLines(const void *argA, const void *argB) {

    assert(argA != NULL);
    assert(argB != NULL);
    
    const char *A = ((Line*)argA)->begin;
    const char *B = ((Line*)argB)->begin;

    //printf("In lineCmp:\n%s vs %s\n", A, B);

    int i;
    int addA = 0;
    int addB = 0;
    for(i = 0; (A[i + addA] != '\0') && (B[i] != '\0'); i ++) {
        if((A[i + addA] < 'A') || ((A[i + addA] > 'z'))) {
            addA++;
            i--;
            continue;
        }
        if((B[i + addB] < 'A') || ((B[i + addB] > 'z'))) {
            addB++;
            i--;
            continue;
        }
        if(A[i+addA] != B[i+addB]) {
            return A[i+addA] - B[i+addB];
        }
    }
    if(A[i+addA] != B[i+addB]) {
        return A[i+addA] - B[i+addB];
    }
    return 0;
}

int divToStr(struct Text *textStuct) {

    int iCh = 0;
    int iStr = 0;
    char *text = textStuct->begin;
    Line* curLine = (Line*)calloc(1, sizeof(Line));
    curLine->begin = text;
    while (text[iCh] != '\0') {
        if(text[iCh] == '\n') {
            text[iCh] = '\0';

            curLine->len = &text[iCh] - curLine->begin;
            textStuct->arrLine[iStr] = curLine;
            iStr ++;
            curLine = (Line*)calloc(1, sizeof(Line));
            curLine->begin = text + iCh + 1;


        }
        iCh++;
    }
    curLine->len = &text[iCh] - curLine->begin;
    textStuct->arrLine[iStr] = curLine;

    #if DEBAG
    for(int i = 0; i < (textStuct->strNum); i++) {
    //fwrite(text->arrLine[i]->begin, sizeof(char), text->arrLine[i]->len, outFile);
    puts(textStuct->arrLine[i]->begin);
    }
    #endif

    return 0;
}

void freeAll(Text *textStruct) {   ////check memory leak
    free(textStruct->begin);       /// i free all buffer or not
    for(int i = 0; i < NUM_OF_STR; i++) {
        free(textStruct->arrLine[i]);
    }

}
// TODO: error handling
void writeLines(struct Text *text) {

    FILE *outFile;
    outFile = fopen("output.txt", "w");
    if (outFile == NULL) {
        printf("Cannot open outfile.\n");
        return;
    }

    for(int i = 0; i < NUM_OF_STR; i++) {
        fputs(text->arrLine[i]->begin, outFile);
        fputc('\n', outFile);
    }

    fclose(outFile);
}
