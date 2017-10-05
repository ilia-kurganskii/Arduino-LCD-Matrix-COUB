#include <stdio.h>
#include <windows.h>
#include <locale.h>

#define SIZE 8
#define COLLOCATION_SIZE 7
#define EMPTY_ARRAY (char *)malloc(sizeof(char) * SIZE * SIZE)
#define GENERATE_HORIZONTAL_LAYOUT 0
#define GENERATE_VERTICAL_LAYOUT 1
int bestSize = SIZE + 1;
int bestEmptyCell = 0;
char *bestLayout;

typedef struct Collocation
{
    char **words;
    int size;
} Collocation;

Collocation collocations[COLLOCATION_SIZE];

int getEmprtyCellCount(char *layout)
{
    int count = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (layout[i * SIZE + j] == 0)
            {
                ++count;
            }
        }
    }
    return count;
}

void printLayout(char *layout)
{
    printf("--------------------\n");
    for (int i = 0; i < SIZE; i++)
    {
        printf("|");
        for (int j = 0; j < SIZE; j++)
        {
            if (layout[i * SIZE + j] == 0)
            {
                printf("_|");
            }
            else
            {
                printf("%c|", layout[i * SIZE + j]);
            }
        }
        printf("\n");
    }
}

void checkBestResult(char *layout)
{
    // if (bestSize>size){
    //     bestSize = size;
    //     bestEmptyCell = getEmprtyCellCount(layout);
    //     //todo add copy
    //     //bestLayout = layout;
    // } else if (bestSize == size){
    int layoutEmptyCell = getEmprtyCellCount(layout);
    if (bestEmptyCell < layoutEmptyCell)
    {
        bestEmptyCell = layoutEmptyCell;
        printLayout(layout);
    }
    free(layout);
    //}
}

void initArray(char *array)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            array[i * SIZE + j] = 0;
        }
    }
}

//result move to matrix parametr
void merge(char *matrix1, char *matrix2)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (matrix1[i * SIZE + j] == 0)
            {
                matrix1[i * SIZE + j] = matrix2[i * SIZE + j];
            }
        }
    }
}

int tryPutWordHorisontaly(int row, int col, char *word, char *matrix, char *layout)
{
    int wordSize = strlen(word);
    if (col + wordSize > SIZE)
        return FALSE;
    if ((col - 1) >= 0 && (matrix[row * SIZE + col - 1] != 0))
        return FALSE;
    for (int wordChar = 0; wordChar < wordSize; ++wordChar)
    {
        if ((matrix[row * SIZE + col + wordChar] != 0) || ((layout[row * SIZE + col + wordChar] != 0) && (layout[row * SIZE + col + wordChar] != word[wordChar])))
            return FALSE;
    }
    return TRUE;
}

int tryPutWordVerticaly(int row, int col, char *word, char *matrix, char *layout)
{
    int wordSize = strlen(word);
    if (row + wordSize > SIZE)
        return FALSE;
    if ((row - 1) >= 0 && (matrix[(row - 1) * SIZE + col] != 0))
        return FALSE;
    for (int wordChar = 0; wordChar < wordSize; ++wordChar)
    {
        int layout_index = (row + wordChar) * SIZE + col;
        if ((matrix[layout_index] != 0) || ((layout[layout_index] != 0) && (layout[layout_index] != word[wordChar])))
            return FALSE;
    }
    return TRUE;
}

void generateWords(int collocationIndex, int wordIndex, int rowMin, int colMin, char *build_matrix, char *layout, int type)
{
    boolean buildNewLayout = (wordIndex == (collocations[collocationIndex].size));
    if (buildNewLayout)
    {
        char *new_layout = EMPTY_ARRAY;
        initArray(new_layout);
        merge(new_layout, layout);
        merge(new_layout, build_matrix);
        ++collocationIndex;
        if (collocationIndex == COLLOCATION_SIZE)
        {
            checkBestResult(new_layout);
            free(new_layout);
            return;
        }
        char *new_build_matrix = EMPTY_ARRAY;
        initArray(new_build_matrix);
        generateWords(collocationIndex, 0, 0, 0, new_build_matrix, new_layout, GENERATE_HORIZONTAL_LAYOUT);
        generateWords(collocationIndex, 0, 0, 0, new_build_matrix, new_layout, GENERATE_VERTICAL_LAYOUT);
        free(new_build_matrix);
        free(new_layout);
        return;
    }
    char *word = collocations[collocationIndex].words[wordIndex];
    int wordSize = strlen(word);
    if (type == GENERATE_HORIZONTAL_LAYOUT)
    {
        for (int row = rowMin; row < SIZE; ++row)
        {
            int start_col = 0;
            if (row == rowMin)
            {
                start_col = colMin;
            }
            if (SIZE - wordSize < start_col)
            {
                continue;
            }
            for (int col = start_col; col < SIZE - wordSize + 1; ++col)
            {
                if (tryPutWordHorisontaly(row, col, word, build_matrix, layout))
                {
                    char *new_build_matrix = EMPTY_ARRAY;
                    initArray(new_build_matrix);
                    merge(new_build_matrix, build_matrix);
                    for (int charIndex = 0; charIndex < wordSize; ++charIndex)
                    {
                        new_build_matrix[row * SIZE + col + charIndex] = word[charIndex];
                    }
                    generateWords(collocationIndex, wordIndex + 1, row, col + wordSize, new_build_matrix, layout, GENERATE_HORIZONTAL_LAYOUT);
                    free(new_build_matrix);
                }
            }
        }
    }
    else
    {
        for (int col = colMin; col < SIZE; ++col)
        {
            int start_row = 0;
            if (col == colMin)
            {
                start_row = rowMin;
            }
            if (SIZE - wordSize < start_row)
            {
                continue;
            }
            for (int row = start_row; row < SIZE - wordSize + 1; ++row)
            {
                if (tryPutWordVerticaly(row, col, word, build_matrix, layout))
                {
                    char *new_build_matrix = EMPTY_ARRAY;
                    initArray(new_build_matrix);
                    merge(new_build_matrix, build_matrix);
                    for (int charIndex = 0; charIndex < wordSize; ++charIndex)
                    {
                        new_build_matrix[(row + charIndex) * SIZE + col] = word[charIndex];
                    }
                    generateWords(collocationIndex, wordIndex + 1, row + wordSize, col, new_build_matrix, layout, GENERATE_VERTICAL_LAYOUT);
                    free(new_build_matrix);
                }
            }
        }
    }
}

int main()
{
    SetConsoleOutputCP(1251);
    // addWord()
    collocations[0].words = (char * [3]){"я", "тебя", "люблю"};
    collocations[0].size = 3;

    collocations[1].words = (char * [3]){"с", "днем","рождения"};
    collocations[1].size = 3;

    collocations[2].words = (char * [3]){"свет","моей","жизни"};
    collocations[2].size = 3;

    collocations[3].words = (char * [3]){"души", "не", "проблема"};
    collocations[3].size = 3;

    collocations[4].words = (char * [1]){"милая"};
    collocations[4].size = 1;

    collocations[5].words = (char * [1]){"любимая"};
    collocations[5].size = 1;

    collocations[6].words = (char * [1]){"солнце"};
    collocations[6].size = 1;


    char *layout = EMPTY_ARRAY;
    char *start_matrix = EMPTY_ARRAY;
    initArray(layout);
    initArray(start_matrix);
    generateWords(0, 0, 0, 0, layout, start_matrix, GENERATE_HORIZONTAL_LAYOUT);
}