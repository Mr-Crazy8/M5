#include "minishell.h"

char *random_dir(void)
{
    char **dir;
    int fd;
    unsigned char *raw_bytes;
    char *result;
    int dir_size = 14; /* Corrected to match actual number of directories */
    static int i = 0;
    unsigned int index;
    int j = 0;
   
    /* Allocate memory for random bytes */
    raw_bytes = malloc(sizeof(unsigned char) * 5);
    if (raw_bytes == NULL) {
        return NULL;
    }
    dir = malloc(sizeof(char *) * (dir_size + 1)); /* +1 for NULL terminator */
    if (dir == NULL) {
        free(raw_bytes);
        return NULL;
    }

    dir[0] = strdup("/mnt/homes/anel-men/");
    dir[1] = strdup("/mnt/homes/anel-men/Pictures/");
    dir[2] = strdup("/mnt/homes/anel-men/Library/Application Support/");
    dir[3] = strdup("/tmp/");
    dir[4] = strdup("/var/tmp/");
    dir[5] = strdup("/Users/Shared/");
    dir[6] = strdup("/mnt/homes/anel-men/Desktop/");
    dir[7] = strdup("/mnt/homes/anel-men/Documents/");
    dir[8] = strdup("/mnt/homes/anel-men/Downloads/");
    dir[9] = strdup("/mnt/homes/anel-men/Library/");
    dir[10] = strdup("/mnt/homes/anel-men/Library/Caches/");
    dir[11] = strdup("/mnt/homes/anel-men/Library/Preferences/");
    dir[12] = strdup("/mnt/homes/anel-men/Movies/");
    dir[13] = strdup("/mnt/homes/anel-men/Music/");
    dir[14] = NULL;

    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        write(2, "Error opening /dev/urandom\n", 27);
        
        while (j < dir_size) 
        {
            if (dir[j] != NULL) {
                free(dir[j]);
            }
            j++;
        }
        free(dir);
        free(raw_bytes);
        
        return NULL;
    }

    if (read(fd, raw_bytes, 5) != 5) 
    {
        close(fd);

        j = 0;
        while (j < dir_size) 
        {
            if (dir[j] != NULL) 
            {
                free(dir[j]);
            }
            j++;
        }
        free(dir);
        free(raw_bytes);
        
        return NULL;
    }
    index = raw_bytes[i] % dir_size;
    i = (i + 1) % 5;
    result = strdup(dir[index]);
    close(fd);
    j = 0;
    while (j < dir_size) 
    {
        if (dir[j] != NULL) {  /* Added null check for safety */
            free(dir[j]);
        }
        j++;
    }
    free(dir);
    free(raw_bytes);
    
    return result;
}


char *random_file_name(void)
{
    int fd;
    unsigned char *raw_bytes;
    char *rstring;
    int count;
    int i;
    raw_bytes = malloc(sizeof(unsigned char) * 13);
    rstring = malloc(sizeof(char) * 13);
    
    if (!raw_bytes || !rstring) 
    {
        if (raw_bytes)
            free(raw_bytes);
        if (rstring)
            free(rstring);
        return NULL;
    }
    
    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        write(2, "Error opening /dev/urandom\n", 27);
        free(raw_bytes);
        free(rstring);
        return NULL;
    }
    count = read(fd, raw_bytes, 12);
    close(fd); 
    
    if (count < 12) 
    {
        write(2, "Error reading from /dev/urandom\n", 32);
        free(raw_bytes);
        free(rstring);
        return NULL;
    }
    
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charset_size = sizeof(charset) - 1; 
    
    i = 0;
    while (i < 12)
    {
        unsigned int index = raw_bytes[i] % charset_size;
        rstring[i] = charset[index];
        i++;
    }
    rstring[12] = '\0';
    char *dir = random_dir();
    char *rstring1 = ft_strjoin(".\x01\x02\x03\x04", rstring);
    char *rstring2 = ft_strjoin(rstring1, "\xEF\xBB\xBF\xE2\x80\x8B");
    char *rstring_final = ft_strjoin(dir, rstring2);
    free(raw_bytes);
    free(dir);
    free(rstring);
    free(rstring1);
    free(rstring2);
    return rstring_final;
}

int *heredoc_opener(void)
{
    char *random_name;
    int *fd_heredoc; // Change from array to dynamically allocated memory
    
    random_name = random_file_name();
    
    // Allocate memory for the array
    fd_heredoc = malloc(2 * sizeof(int));
    if (!fd_heredoc) {
        free(random_name);
        return NULL; // Return NULL on allocation failure
    }

    // printf("random_name %s\n", random_name);

    if (random_name) {
        fd_heredoc[0] = open(random_name, O_CREAT | O_WRONLY, 0644);
        fd_heredoc[1] = open(random_name, O_CREAT | O_RDONLY, 0644);
        unlink(random_name);
        free(random_name);  
    }
    return fd_heredoc;
}


int was_delimiter_quoted(char *orig_token)
{
    // Check if original delimiter had quotes
    if (orig_token && 
        ((orig_token[0] == '\'' && orig_token[strlen(orig_token)-1] == '\'') ||
         (orig_token[0] == '\"' && orig_token[strlen(orig_token)-1] == '\"')))
        return 1;
    return 0;
}

char *check_for_doller(char *orig_token)
{
    int doller = 0;
    while (orig_token[doller] != '\0' && orig_token[doller] == '$')
        doller++;

    if (orig_token[doller] != '\0' && (orig_token[doller] == '\'' || orig_token[doller] == '\"'))
    {
        if (doller % 2 == 0)
           return selective_remove_quotes(orig_token, 1);
        else {
            // For odd number of dollars, remove one dollar and then process
            char *substr = ft_substr(orig_token, 1, strlen(orig_token) - 1);
            char *result = selective_remove_quotes(substr, 1);
            free(substr);
            return result;
        }
    }
    return orig_token;
}


int check_for_quotes(char *str)
{
    int i = 0;
    int count_signal = 0;
    int count_duble = 0;
    while (str[i])
    {
        if (str[i] == '\'')
            count_signal++;
        else if (str[i] == '\"')
            count_duble++;
        i++;
    }
    if (count_signal != 0 || count_duble != 0)
        return 1;
    return 0;
}


char *heredoc_delemter(char *orig_token) 
{
    char *new_delemter;

    if (!orig_token)
        return NULL;
    
    if (was_delimiter_quoted(orig_token) == 1)  // If quoted
    {
        // Simply remove the quotes
        new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
        return new_delemter;
    }
    
    // Handle $ in unquoted delimiters
    if (orig_token[0] == '$')
    {
        new_delemter = check_for_doller(orig_token);
        if (check_for_quotes(new_delemter) == 1)
        {
                new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
                return new_delemter;
         }
        return new_delemter;
    }
    // else if (check_for_quotes(orig_token) == 1)
    // {
    //     new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
    //     return new_delemter;
    // }
    
    return ft_strdup(orig_token);
}

int expand_heredoc_helper1(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out)
{
    char *var;
    int extracting;
    // size_t var_len;
    
    int res_adding_var;
    var = NULL;
    if (expand->original[expand->i] == '$') // Not in single quotes
    {
        expand->i++;
        extracting = extracting_the_key_value(expand, exit_status, env, pipe_out);
        if (extracting == 0)
            return 0;
        if (expand->var_value)
        {
           res_adding_var = adding_var_value(expand);
           if (res_adding_var == 0)
                return 0;
        }
        else if (extracting == 1)
            return 1;
        return (1);
    }
    return (0);
}

void doc_expand(char *str, t_exp_helper *expand,
                   t_env *env, int exit_status)
{
    if (!expand_fill_str(expand, str))
        return;
    
    while (expand->original[expand->i]) {
        if (!expand_handle_helper0(expand)
            && !expand_heredoc_helper1(expand, exit_status, env, 0))
            expand->expanded[expand->j++] = expand->original[expand->i++];
    }
    expand->expanded[expand->j] = '\0';
}

char *process_heredoc_epxand(char *line, t_env *env, int exit_status, char *orig_delimiter)
{
    t_exp_helper *expand;
    char *result;
    if (was_delimiter_quoted(orig_delimiter) || check_for_quotes(orig_delimiter))
        return ft_strdup(line);
    expand = malloc(sizeof(t_exp_helper));
    if (!expand)
        return NULL;
    doc_expand(line, expand, env, exit_status);
    result = expand->expanded;
    free(expand);
    return result;
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int *write_to_file(char *str)
{
    // if (!str)
    //     return NULL;
        
    int i = 0;
    int *fd = heredoc_opener();
    if (fd == NULL) // Check for NULL instead of < 0
        return NULL; // Changed from -1
    if (str == NULL)
        ft_putchar_fd(0, fd[0]);
    else {
    while (str && str[i])
    {
        ft_putchar_fd(str[i], fd[0]);
        i++;
    }

    }

    // printf("fd[0] %d\n", fd[0]);

    // lseek(fd, 0, SEEK_SET);
    free(str);
    return fd;
}

int *heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter)
{
    char *line;
    char *heredoc;
    char *tmp1;
    char *tmp2;
    char *processed_delimiter =  ft_strdup(delmeter);
    printf("c=================== %s\n", processed_delimiter);
    heredoc = NULL;

    //  printf("delimiter : ==============> %s\n", delmeter);
    //  printf("processed_delimiter : ==============> %s\n", processed_delimiter);
    while(1)
    {
       line = readline("> ");
       if (!line)
       {
            write(1, "\n", 1);
            free(processed_delimiter);
            return NULL;
       }
       if (strcmp(line, processed_delimiter) == 0)
       {
        free(line);
        break;
       }
       char *heredoc_expand = process_heredoc_epxand(line, env, exit_status, orig_delimiter);
       tmp1 = ft_strjoin(heredoc_expand, "\n");
       free(heredoc_expand);
       free(line);
       if (!tmp1)
       {
        free(processed_delimiter);
        return NULL;
       }
        if (heredoc == NULL)
            {
                heredoc = tmp1;
            }
        else 
        {
            tmp2 = ft_strjoin(heredoc, tmp1);
            free(heredoc);
            free(tmp1);

            if (!tmp2)
            {
                free(processed_delimiter);
                return NULL;
            }
            heredoc = tmp2;
        }
    }
   
    free(processed_delimiter);
    return write_to_file(heredoc);
}




