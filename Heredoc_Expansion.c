#include "minishell.h"





// static char	*add(char const *s1, char const *s2)
// {
// 	size_t	s1_len;
// 	size_t	s2_len;
// 	size_t	i;
// 	size_t	j;
// 	char	*s3;

// 	i = 0;
// 	j = 0;
// 	s1_len = strlen(s1);
// 	s2_len = strlen(s2);
// 	s3 = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
// 	if (s3 == NULL)
// 		return (NULL);
// 	while (i < s1_len)
// 	{
// 		s3[i] = s1[i];
// 		i++;
// 	}
// 	while (j < s2_len)
// 		s3[i++] = s2[j++];
// 	s3[i] = '\0';
// 	return (s3);
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*s3;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s1)
// 		return (strdup(s2));
// 	if (!s2)
// 		return (strdup(s1));
// 	s3 = add(s1, s2);
// 	return (s3);
// }

char *random_dir(void)
{
    char **dir;
    int fd;
    unsigned char *raw_bytes;
    char *result;
    int dir_size = 16;
    static int i = 0;
    unsigned int index;
    int j = 0;
   
    /* Allocate memory for random bytes */
    raw_bytes = malloc(sizeof(unsigned char) * 5);
    if (raw_bytes == NULL) {
        return NULL;
    }
    dir = malloc(sizeof(char *) * 17);
    if (dir == NULL) {
        free(raw_bytes);
        return NULL;
    }

    dir[0] = strdup("/mnt/homes/anel-men/");
    dir[1] = strdup("/mnt/homes/anel-men/Public/");   ///remove
    dir[2] = strdup("/mnt/homes/anel-men/Pictures/");
    dir[3] = strdup("/mnt/homes/anel-men/Library/Application Support/");
    dir[4] = strdup("/tmp/");
    dir[5] = strdup("/var/tmp/");
    dir[6] = strdup("/Users/Shared/");
    dir[7] = strdup("/dev/");   ///remove
    dir[8] = strdup("/mnt/homes/anel-men/Desktop/");
    dir[9] = strdup("/mnt/homes/anel-men/Documents/");
    dir[10] = strdup("/mnt/homes/anel-men/Downloads/");
    dir[11] = strdup("/mnt/homes/anel-men/Library/");
    dir[12] = strdup("/mnt/homes/anel-men/Library/Caches/");
    dir[13] = strdup("/mnt/homes/anel-men/Library/Preferences/");
    dir[14] = strdup("/mnt/homes/anel-men/Movies/");
    dir[15] = strdup("/mnt/homes/anel-men/Music/");
    dir[16] = NULL;

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
        free(dir[j]);
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










// void heredoc_delemter(t_cmd *cmd_list) /// 2
// {
//     t_cmd *tmp = cmd_list;
//     char *new_delemter;
//     while (tmp)
//     {
//         t_redir *redir = tmp->redirs;
//         while (redir)
//         {
//             if (redir->type == 3) // Heredoc
//             {
//                 if (!was_delimiter_quoted(redir->orig_token))
//                 {
//                     if (redir->orig_token[0] == '$')
//                     {
//                         new_delemter = check_for_doller(redir->orig_token);
//                         free(redir->orig_token);
//                         redir->orig_token = new_delemter;
//                     }
//                     else 
//                     {
//                         new_delemter = selective_remove_quotes(redir->orig_token, 1);
//                         free(redir->orig_token);
//                         redir->orig_token = new_delemter;
//                     }
//                 }
//             }
//             redir = redir->next;
//         }
//         tmp = tmp->next;
//     }
// }





int was_delimiter_quoted(char *orig_token)
{
    // Check if original delimiter had quotes
    if (orig_token && 
        ((orig_token[0] == '\'' && orig_token[strlen(orig_token)-1] == '\'') ||
         (orig_token[0] == '\"' && orig_token[strlen(orig_token)-1] == '\"')))
        return 1;
    return 0;
}


//for this func my logic it to count fo $ and if  is even (doller % 2 == 0)
//i will just remove the quotes with selective_remove_quotes
//else i will use sudstr func to creat new string from tab 1 not 0 so 
//if we have $$$ the new string will have $$
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



// char *heredoc_delemter(char *orig_token) ///0
// {
//     char *new_delemter;

//     if (!orig_token)
//         return NULL;
//     if (was_delimiter_quoted(orig_token) == 0)
//     {
//         if (orig_token[0] == '$')
//         {
//             new_delemter = check_for_doller(orig_token);
//         }
//         else 
//         {
//             new_delemter = selective_remove_quotes(orig_token, 1);
//         }
        
//         return new_delemter;
//     }
//     return ft_strdup(orig_token);
// }


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


void doc_expand(char *str, t_exp_helper *expand,
                   t_env *env, int exit_status)
{
    if (!expand_fill_str(expand, str))
        return;
    
    while (expand->original[expand->i]) {
        if (!expand_handle_helper0(expand)
            && !expand_handle_helper1(expand, exit_status, env, 0))
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

    // printf("ep =========> %s\n", result);
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
    char *processed_delimiter = delmeter;

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
       tmp1 = ft_strjoin(process_heredoc_epxand(line, env, exit_status, orig_delimiter), "\n");
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


//===========================
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// // Function to extract delimiter from heredoc syntax
// char *extract_delimiter(char *cmd)
// {
//     // char *delimiter = NULL;/
//     char *start = strstr(cmd, "<<");
    
//     if (!start)
//         return NULL;
    
//     // Skip the << and any spaces
//     start += 2;
//     while (*start && (*start == ' ' || *start == '\t'))
//         start++;
    
//     if (!*start)
//         return NULL;
    
//     // Create a copy of the whole delimiter
//     return strdup(start);
// }

// int main(int argc, char *argv[], char *env[])
// {
//     char *input;
//     char *delimiter;
//     // char *content;
//     t_env *env_struc = NULL;  // Use your existing env or NULL
//     int exit_status = 42;
//     int running = 1;
//     env_maker(env, &env_struc);
//     printf("\033[1;33m===== Heredoc Shell Simulator (%s) =====\033[0m\n", "2025-05-08");
//     printf("Current User: %s\n\n", "Mr-Crazy8");
//     printf("Type commands like: << EOF or << 'EOF' (with quotes)\n");
//     printf("Type 'exit' to quit\n\n");
    
//     // You can initialize env here with your existing code if needed
    
//     while (running)
//     {
//         input = readline("heredoc-sim$ ");
//         if (!input)
//             break;
        
//         if (strlen(input) > 0)
//             add_history(input);
        
//         if (strcmp(input, "exit") == 0)
//         {
//             free(input);
//             running = 0;
//             continue;
//         }
        
//         if (strncmp(input, "<<", 2) == 0)
//         {
//             // Extract delimiter from command
//             delimiter = extract_delimiter(input);
//             // printf("delimiter : ==============> %s\n", delimiter);
//             if (delimiter)
//             {
//                 // Call your heredoc function
//                 heredoc(delimiter, env_struc, exit_status, delimiter);
                
//                 // Display the content
//                 // printf("\n--- Heredoc Content ---\n");
//                 // printf("%s", content ? content : "(null)");
//                 // // Make sure output ends with a newline
//                 // if (content && content[0] != '\0' && content[strlen(content)-1] != '\n')
//                 //     printf("\n");
//                 // printf("--- End of Content ---\n\n");
                
//                 // Free memory
//                 free(delimiter);
//                 // if (content)
//                 //     free(content);
//             }
//             else
//             {
//                 printf("Error: Invalid heredoc syntax\n");
//             }
//         }
//         else if (strlen(input) > 0)
//         {
//             printf("Unknown command. Use << DELIMITER to start a heredoc.\n");
//         }
        
//         free(input);
//     }
    
//     printf("\nExiting heredoc simulator.\n");
//     // Free your environment if needed using your existing code
    
//     return 0;
// }



