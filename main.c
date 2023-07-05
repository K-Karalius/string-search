// @author Kristupas Karalius
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 255
#define TRUE 1
#define FALSE 0

void GetFileName(char *name);
int FileNameFormat(char *name, int length);
int OpenForReading(FILE **file, char *name);
int OpenForWriting(FILE **file, char *name);
int CountFileLines(FILE *file);
void FindWords(FILE *file, char **shortWords, char **longWords, int lineCount);
void FindShortAndLong(char **shortest, char **longest, char *str, int len);
void PrintToFile(FILE *file, char **shortW, char **longW, int lineCount);

int main()
{
    printf("Application finds the longest and the shortest string in every line from the given .txt file.\n");
    FILE *input;
    FILE *output;
    char *inputFileName = malloc((BUFFER_SIZE + 1) * sizeof(char));
    char *outputFileName = malloc((BUFFER_SIZE + 1) * sizeof(char));

    printf("Please enter the input file name (<= 255 symbols):\n");

    do
    {
        GetFileName(inputFileName);
    } while (OpenForReading(&input, inputFileName) == FALSE);
    free(inputFileName);

    printf("Please enter the output file name (<= 255 symbols):\n");
    do
    {
        GetFileName(outputFileName);
    } while (OpenForWriting(&output, outputFileName) == FALSE);

    int lineCount = CountFileLines(input);
    char **shortWords = malloc(lineCount * sizeof(sizeof(char *)));
    char **longWords = malloc(lineCount * sizeof(sizeof(char *)));

    printf("Reading the input file...\n");
    FindWords(input, shortWords, longWords, lineCount);
    fclose(input);

    output = fopen(outputFileName, "w");
    free(outputFileName);
    printf("Results are being printed to the output file...\n");
    PrintToFile(output, shortWords, longWords, lineCount);
    fclose(output);

    for (int i = 0; i < lineCount; i++)
    {
        free(shortWords[i]);
        free(longWords[i]);
    }

    free(shortWords);
    free(longWords);

    printf("DONE!!!\n");

    return 0;
}

void PrintToFile(FILE *file, char **shortW, char **longW, int lineCount)
{
    for (int i = 0; i < lineCount; ++i)
    {
        fputc(i + 49, file);
        fprintf(file, " %s", "Lines:\n");
        if (strlen(shortW[i]) != strlen(longW[i]))
        {
            fprintf(file, "Longest: \"%s\"\n", shortW[i]);
            fprintf(file, "Shortest: \"%s\"\n", longW[i]);
        }
        else
        {
            fprintf(file, "%s", "The line doesn't have the shortest or the longest string\n");
        }
    }
}

int CountFileLines(FILE *file)
{
    int count = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            ++count;
        }
    }

    return count;
}

void FindWords(FILE *file, char **shortWords, char **longWords, int lineCount)
{
    rewind(file);
    char *str = malloc((BUFFER_SIZE) * sizeof(char));
    char *shortest = malloc((BUFFER_SIZE) * sizeof(char));
    char *longest = malloc((BUFFER_SIZE ) * sizeof(char));

    char temp;
    int strLength;

    for (int i = 0; i < lineCount; ++i)
    {
        strLength = 0;
        while (TRUE)
        {
            temp = fgetc(file);
            if (temp != '\n' && strLength < BUFFER_SIZE)
            {
                str[strLength] = temp;
                ++strLength;
            }

            if (temp == '\n')
            {
                break;
            }
        }
        str[strLength] = '\0';

        FindShortAndLong(&shortest, &longest, str, strLength);
        shortWords[i] = shortest;
        longWords[i] = longest;
        shortest = malloc((BUFFER_SIZE) * sizeof(char *));
        longest = malloc((BUFFER_SIZE) * sizeof(char *));
    }

    free(str);
    free(shortest);
    free(longest);
}

void FindShortAndLong(char **shortest, char **longest, char *str, int len)
{
    int longLen = 0;
    int shortLen = BUFFER_SIZE + 1;
    int longInd;
    int shortInd;
    int wLen = 0;

    for (int i = 0; i <= len; ++i)
    {
        if (str[i] != ' ' && str[i] != '\0')
        {
            ++wLen;
            continue;
        }

        if (wLen > longLen)
        {
            longLen = wLen;
            longInd = i - longLen;
        }

        if (wLen < shortLen && wLen > 0)
        {
            shortLen = wLen;
            shortInd = i - shortLen;
        }
        wLen = 0;
    }
    
    if(longLen == 0)
    {
        return;
    }

    char *temp = realloc((*longest), longLen);
    if (temp != NULL)
    {
        (*longest) = temp;
    }

    for (int i = 0; i < longLen; ++i)
    {
        (*longest)[i] = str[longInd + i];
    }
    (*longest)[longLen] = '\0';

    temp = realloc((*shortest), shortLen);
    if (temp != NULL)
    {
        (*shortest) = temp;
    }

    for (int i = 0; i < shortLen; ++i)
    {
        (*shortest)[i] = str[shortInd + i];
    }
    (*shortest)[shortLen] = '\0';
}

void GetFileName(char *name)
{
    int strLength;
    while (TRUE)
    {
        scanf(" %s", name);
        strLength = strlen(name);
        if (FileNameFormat(name, strLength) == FALSE)
        {
            printf("The file name is too long or the extension is not .txt \n");
            printf("You file name should be as follows: \"example.txt\"\n");
            printf("Try again:\n");
        }
        else
        {
            break;
        }
    }
}

int FileNameFormat(char *name, int length)
{
    if (length < 5 || length > BUFFER_SIZE)
        return FALSE;
    if (name[length - 1] != 't')
        return FALSE;
    if (name[length - 2] != 'x')
        return FALSE;
    if (name[length - 3] != 't')
        return FALSE;
    if (name[length - 4] != '.')
        return FALSE;

    return TRUE;
}

int OpenForReading(FILE **file, char *name)
{
    *file = fopen(name, "r");
    if (*file == NULL)
    {
        printf("Unable to open the file!\n");
        printf("Try again:\n");
        return FALSE;
    }

    return TRUE;
}

int OpenForWriting(FILE **file, char *name)
{
    *file = fopen(name, "w");
    if (*file == NULL)
    {
        printf("Unable to open the file\n");
        printf("Try again:");
        return FALSE;
    }
    return TRUE;
}
