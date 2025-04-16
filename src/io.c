#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buckai.h"
#include "io.h"

bool prompt_s(char *prompt_str, char *dest)
{
    printf("%s", prompt_str);

    char input_str[IO_BUFFER_SIZE];
    if (!fgets(input_str, IO_BUFFER_SIZE, stdin))
    {
        perror("Could not get user input");
        return false;
    }

    input_str[strcspn(input_str, "\n")] = '\0';
    strcpy_s(dest, IO_BUFFER_SIZE, input_str);

    return true;
}

int prompt_i(char *prompt_str)
{
    printf("%s", prompt_str);

    char input_str[IO_BUFFER_SIZE];
    if (!fgets(input_str, IO_BUFFER_SIZE, stdin))
    {
        perror("Could not get user input");
        return false;
    }

    input_str[strcspn(input_str, "\n")] = '\0';

    return (char)atoi(input_str);
}

bool prompt_b(char *prompt_str)
{
    char input_char = '\0';
    char input_str[IO_BUFFER_SIZE];

    while (input_char != 'y' && input_char != 'n')
    {
        printf("%s", prompt_str);

        if (!fgets(input_str, IO_BUFFER_SIZE, stdin))
        {
            perror("Could not get user input");
            return false;
        }

        input_char = (char)tolower(input_str[0]);
    }

    if (input_char == 'y')
        return true;
    else
        return false;
}

bool prompt_v(char *prompt_str, char **dest, int max_count, int *dest_count)
{
    char input_str[IO_BUFFER_SIZE];
    if (!prompt_s(prompt_str, input_str))
        return false;

    char *tokens[IO_BUFFER_SIZE / 2] = {NULL};
    int i = 0;
    tokens[i] = strtok(input_str, " ");
    while (tokens[i] != NULL && i < (IO_BUFFER_SIZE / 2) - 1)
    {
        i++;
        tokens[i] = strtok(NULL, " ");
    }

    i = 0;
    while (tokens[i] != NULL && i < max_count)
    {
        dest[i] = tokens[i];
        i++;
    }

    if (dest_count != NULL)
        *dest_count = i;

    return true;
}

bool prompt_vi(char *prompt_str, int *dest, int max_count, int *dest_count)
{
    char input_str[IO_BUFFER_SIZE];
    if (!prompt_s(prompt_str, input_str))
        return false;

    char *tokens[IO_BUFFER_SIZE / 2] = {NULL};
    int i = 0;
    tokens[i] = strtok(input_str, " ");
    while (tokens[i] != NULL && i < (IO_BUFFER_SIZE / 2) - 1)
    {
        i++;
        tokens[i] = strtok(NULL, " ");
    }

    i = 0;
    while (tokens[i] != NULL && i < max_count)
    {
        dest[i] = atoi(tokens[i]);
        i++;
    }

    if (dest_count != NULL)
        *dest_count = i;

    return true;
}
