#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

void replace(int position, char **text, char *redact_string)
{
    int i;

    while (*redact_string == ' ')
    {
        redact_string++;
    }

    for (i = 0; i < strlen(redact_string); i++)
    {
        (*text)[i + position] = '*';
    }
}

int get_words(char **line, char ***line_array, const char *filename)
{
    FILE *file_pointer_text;
    file_pointer_text = fopen(filename, "r");
    if (file_pointer_text == NULL)
    {
        printf("ERROR");
        exit(1);
    }

    int line_check = -1;
    int index = 0;
    char current_char;
    int size_line = 10;
    int size_array = 10;

    while((current_char = getc(file_pointer_text)) != EOF)
    {
        if (current_char != '\n')
        {
            (*line)[index++] = current_char;
            if (index == size_line)
            {
                size_line += 10;
                *line = realloc(*line, size_line);
            }
        }
        else
        {
            line_check++;
            (*line)[index++] = '\0';
            if (line_check == (size_array - 1))
            {
                size_array += 10;
                *line_array = realloc(*line_array, (sizeof(char *) * size_array));
            }
            (*line_array)[line_check] = malloc(strlen(*line) + 1);
            strcpy((*line_array)[line_check], *line);
            index = 0;
            size_line = 10;
            *line = calloc(size_line, sizeof(char));
        }
    }
    (*line)[index++] = '\0';
    line_check++;

    (*line_array)[line_check] = malloc(strlen(*line) + 1);
    strcpy((*line_array)[line_check], *line);

    fclose(file_pointer_text);

    return line_check;
}

int match_letters(char*, char*);

void write_to_file(char **line_array, int array_count)
{
    FILE *file_pointer_text;
    file_pointer_text = fopen("result.txt", "w"); // Opens text file (text_filename)
    if (file_pointer_text == NULL)
    {
        printf("ERROR");
        exit(1);
    }

    int i;
    for (i = 0; i < (array_count + 1); i++)
    {
        fprintf(file_pointer_text, "%s\n", line_array[i]);
    }

    fclose(file_pointer_text);
}

void redact_words(const char *text_filename, const char *redact_words_filename)
{
    char **text_line_array = malloc(sizeof(char *) * 10); // Initially declare array of pointers with an initial size of 10
    char *text_line = malloc(10);
    char **redact_line_array = malloc(sizeof(char *) * 10);
    char *redact_line = malloc(10);
    int i;
    int position;
    char *temp_redact = malloc(10);
    char *temp_text = malloc(10);
    char *token;
    int redact_array_count;
    int text_array_count;
    int row;

    redact_array_count = get_words(&redact_line, &redact_line_array, redact_words_filename);
    text_array_count = get_words(&text_line, &text_line_array, text_filename);

    text_line_array = realloc(text_line_array, (sizeof(char *) * (text_array_count + 1)));
    redact_line_array = realloc(redact_line_array, (sizeof(char *) * (redact_array_count + 1)));

    for (i = 0; i < (text_array_count + 1); i++)
    {
        for (row = 0; row < (redact_array_count + 1); row++)
        {
            temp_redact = realloc(temp_redact, strlen(redact_line_array[row]) + 1);
            strcpy(temp_redact, redact_line_array[row]);
            token = strtok(temp_redact, ",");

            temp_text = realloc(temp_text, strlen(text_line_array[i]) + 1);
            strcpy(temp_text, text_line_array[i]);
            while (token != NULL)
            {
                while ((position = match_letters(temp_text, token)) != -1)
                {
                    replace(position, &temp_text, token);
                }
                token = strtok(NULL, ",");
            }
            text_line_array[i] = realloc(text_line_array[i], strlen(temp_text) + 1);
            strcpy(text_line_array[i], temp_text);
        }
    }

    write_to_file(text_line_array, text_array_count);

    free(temp_redact);
    free(temp_text);

    for (i = 0; i < (redact_array_count + 1); i++)
    {
        free(redact_line_array[i]);
    }
    free(redact_line);
    free(redact_line_array);

    for (i = 0; i < (text_array_count + 1); i++)
    {
        free(text_line_array[i]);
    }
    free(text_line);
    free(text_line_array);
}

int match_letters(char *text, char *redact_string)
{
    int position = 0;
    char *temp_text, *temp_redact_string;

    while (*redact_string == ' ')
    {
        redact_string++;
    }

    temp_text = text;
    temp_redact_string = redact_string;

    while (*text != '\0')
    {
        for (; ;)
        {
            if (tolower(*temp_text) == tolower(*temp_redact_string))
            {
                temp_text++;
                temp_redact_string++;
                if (*temp_text == '\0' || *temp_redact_string == '\0')
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
   
        if (*temp_redact_string == '\0')
        {
            break;
        }
           
        text++;
        position++;
        temp_text = text;
        temp_redact_string = redact_string;
    }

    if(*text != '\0')
    {
        return position;
    }
    else
    {
        return -1;
    }
}