#include "parsing.h"

int preprocess_cmd_hp_0(char *input, int *i, int *in_quotes, char *result, int *j)
{
        if (input[(*i)] == '\'' && *in_quotes != 2)
        {
            if (*in_quotes == 1)
                *in_quotes = 0;
            else
                *in_quotes = 1;
            result[(*j)++] = input[(*i)++];
            return (1);
        }
        else if (input[(*i)] == '\"' && *in_quotes != 1)
        {
            if (*in_quotes == 2)
                *in_quotes = 0; 
            else
                *in_quotes = 2;
            result[(*j)++] = input[(*i)++];
            return (1);
        }
        return(0);

}


void preprocess_cmd_hp_1(char *input, int *i, int *in_quotes, char *result, int *j)
{
    if (input[(*i)] == '>' || input[(*i)] == '<')
    {
        if ((*j) > 0 && result[(*j)-1] != ' ' && result[(*j)-1] != '>' && result[(*j)-1] != '<')
                result[(*j)++] = ' ';
            result[(*j)++] = input[(*i)++];
            if (input[(*i)] == '>' || input[(*i)] == '<')
                result[(*j)++] = input[(*i)++];
            if (input[(*i)] && input[(*i)] != ' ')
                result[(*j)++] = ' ';
    }
    else
        result[(*j)++] = input[(*i)++];

}

int preprocess_cmd_hp_2(char *input, int *i, int *in_quotes, char *result, int *j)
{
    if (*in_quotes)
    {
        result[(*j)++] = input[(*i)++];
        return (1);
    }
    return (0);
}

char *preprocess_command(char *input)
{
    int i = 0;             
    int j = 0;             
    int in_quotes = 0;   
    char *result;
    
    if (!input)
        return NULL;
        
    result = malloc(strlen(input) * 3 + 1);
    if (!result)
        return NULL;
        
    i = 0;             
    j = 0;             
    in_quotes = 0;    
    while (input[i])
    {
        if (preprocess_cmd_hp_0(input, &i, &in_quotes, result, &j))
            continue;
        if (preprocess_cmd_hp_2(input, &i, &in_quotes, result, &j))
            continue;
        preprocess_cmd_hp_1(input, &i, &in_quotes, result, &j);
    }
    return (result[j] = '\0', result);
}
