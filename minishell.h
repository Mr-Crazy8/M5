#ifndef MINISHELL_H
# define MINISHELL_H


#include <sys/stat.h>
#include <errno.h>
#include<signal.h>
#include <termios.h>
#include "parsing/parsing.h"


// #define SET 0
// #define GET 1

// typedef struct t_data
// {
//     int     exit_status;
//     char    *new_pwd;
// } t_data;

// typedef struct s_token
// {
//    char *DATA;
//    char *TOKIN;
//    char *befor_DATA_expand;
//    struct s_token *next;
// } t_token;

// typedef struct s_env
// {
//     char    *key;
//     char    *value;
//     int     is_not_active;
//     int     for_path;
//     struct s_env *next;
// } t_env;

// typedef struct s_var {
//     char *name;         // Variable name
//     int is_valid;       // Is this a valid variable ref
//     int length;         // Length of entire reference
//     int name_allocated; // Did we allocate memory for name
// } t_var;

// typedef struct s_redir
// {
//     int type;             /* 0:<, 1:>, 2:>>, 3:<< */
//     char *file;           /* Filename or heredoc delimiter */
//     int fd;
//     int Ambiguous;
//     char    *orig_token;  /* Original token with quotes */
//     char *heredoc_delemter; /* Fully processed delimiter for matching */
//     int dont_run;
//     struct s_redir *next; /* Next redirection */
// } t_redir;

// typedef struct s_cmd
// {
//     char *cmd;            /* Command name */
//     char **args;          /* Command args (including cmd as args[0]) */
//     t_redir *redirs;      /* Redirections list */
//     t_data    data;
//     int pipe_out;         /* 1 if command pipes to next */
//     int     fd_pipe[2];
//     char **args_befor_quotes_remover;
//     struct s_cmd *next;   /* Next command in pipeline */
// } t_cmd;

// typedef struct s_exp_helper {
    
//     char *original;
//     char *expanded;
//     int i;
//     int j;
//     int quote_state;
//     char *var_name;
//     char *var_value;
//     int k;
//     int start;
//      int var_expanded;
    
// } t_exp_helper;


// void process_redir_helper(char str, int *quote_state);

// t_cmd *parse_tokens(t_token *token_list);
// // int open_file(int type, char *file);
// int open_file(t_cmd *cmd, int type, char *file);

// //env_helper.c
// void	*ft_memcpy(void *dest, const void *src, size_t n);
// // static size_t	count_words(char const *s, char c);
// // static char	**split_string(char const *s, char c, char **split, size_t n);
// char	**ft_split(char const *s, char c);

// //env.c
// t_env *env_node_maker(char *env[]);
// // void env_maker(char *env[], t_env **env_struct);
// void	add_redir_back(t_redir **lst, t_redir *new);
// // void file_opener(t_cmd *cmd);
// void file_opener(t_cmd *cmd, t_env *env);
// void print_file_error(char *file, int i);
// //lexer_heplper_1.c
// int is_whitespace(char c);
// // char	*ft_substr(char const *s, unsigned int start, size_t len);
// int special_character_cheker(char character);
// t_token *special_character_handle(char *input, int *i);
// void handle_block_helper(char *input, int *i, t_token **token_list);
// void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode);
// t_redir *creat_redir_node(int type, char *file);
// void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode);
// t_cmd *parser(t_token *token_list);
// void free_split_str(char **split_str);
// char **free_split_q(char **split, size_t j);
// char **ft_split_q(char const *s, char c);
// char *redir_extracter(char *str);
// // static char *process_redir(char *str, int *pos);
// void	add_cmd_back(t_cmd **lst, t_cmd *new);
// t_redir *creat_redir_list(char *str);
// void free_env_struct(t_env *env_struct);
// char *preprocess_command(char *input);
// int preprocess_cmd_hp_2(char *input, int *i, int *in_quotes, char *result, int *j);
// void preprocess_cmd_hp_1(char *input, int *i, int *in_quotes, char *result, int *j);
// int preprocess_cmd_hp_0(char *input, int *i, int *in_quotes, char *result, int *j);
// void	ft_lstadd_back_token(t_token **lst, t_token *new);
// void free_string_array(char **array);
// //lexer_heplper_0.c
// int ft_strlen(const char *str);
// char	*ft_strdup(const char *s);
// t_token	*creat_token(char *data, char *token_type);
// char	*ft_strncpy(char *dest, const char *src, unsigned int n);
// void print_cmd(t_cmd *cmd_list);

// //lexer.c
// void handle_block(char *input, int *i, int input_length, t_token **token_list);
// t_token *tokin_list_maker(char *input);


// //syntx_check.c
// int error_pipi(t_token *token_list);
// int check_syntax_errors(t_token *token_list);
// int check_quotes(char *input);
// int check_quotes_helper(int in_quote);
// int	print_error_pipe(void);

// //syntx_check_helper_0.c
// int check_for_redirection(char *str);
// void  check_missing_filename(char *str);
// void print_error(char *str, int *i);
// int invalid_filename_checker(char *str, int *i);
// int check_invalid_filename(char *str);
// char *heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter);
// //syntx_check_helper_1.c
// void	redirection_helper(char *str, int *i, int *count);
// void process_string(char *str, t_exp_helper *expand,
//                    t_env *env, int exit_status);




// void ambiguous_finder(t_cmd *cmd);
// void ambiguous_checker(t_redir *redir);
// int scan_for_expand(char *str);
// char *extranct_valu(char *str);
// int	ft_isalnum(int c);
// int is_valid_var_char(char c);
// void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status);
// // void	expand_handle(t_token *token_list, t_env *env, int exit_status);
// char *lookup_variable(char *var_name, t_env *env_struct);
// int has_unquoted_spaces(char *str);
// void split_token(t_token *token);
// int get_num_length(long num);
// void fill_num_string(char *str, long num, int len);
// char *ft_itoa(int n);

// void free_cmd_list(t_cmd *cmd_list);
// void	free_token_list(t_token *token_list);
// /// HNa 

// char	*ft_strchr(char *s, int c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_lstadd_back(t_env **lst, t_env *new);
// char	*ft_substr(char *s, unsigned int start, size_t len);
char    *ft_substr(char const *s, unsigned int start, size_t len);
// void apply_word_splitting(t_cmd *cmd_list);
// int expand_fill_str(t_exp_helper *expand, char *str);
char	**free_split(char **split, size_t j);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
int    echo(char **argv);
int     ft_strcmp(char *s1, char *s2);
char	*ft_strchr(char *s, int c);

void    del(void *content);
int ft_unset(t_env **env_list, char **variables);
int     remove_env(t_env **env_list, char *variable);
int     is_valid_key(char *key);
int ft_exit(char **args, t_data data);
int    ft_env(t_cmd *cmd, t_env *env_list);
char *chenger_back(char *str);
t_env   *env_maker(char *env[], t_env **env_struct);
int ft_export(char **str, t_env **env_list);
void    updat_env(t_env **env_list, char *key, char *value);
int     pwd(t_data data);
char    *get_value_env(char *key, t_env **list);
void    ft_excute_commands(t_cmd *cmd, t_env **list);
void    env_null(t_env **list);
int    ft_cd(char **str, t_env **list, t_data data);

void    ft_free_split(char **str);
void value_empty(t_env **env_list, char *key);


// int *heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter);
void heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter, int fd);

int get_or_set(int type, int status); // for intialize or get exit status;
void wait_for_children(t_cmd *cmd, pid_t child_pid);
//for teste ;
void intialize_struct(t_redir *list);
void ft_redircte(t_redir *rederction, t_env *env, t_cmd *cmd);


void check_line(t_cmd **command, t_env *env, char *en[]);

int exec_builtin(char **args);
void minishell_cmd(char *env[]);
int		ft_list_size(t_env *begin_list);
char *check_path(t_env **list);
// void ft_ex(t_cmd *cmd, t_env *list_env, char *env);


void execute_single_command(t_cmd *cmd, t_env *list_env, char *env[]);

void check_here_doc(t_cmd *cmd, t_env *env);
// void check_close_red(t_cmd *cmd, t_cmd *prev);
void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env);
char *selective_remove_quotes(char *str, int remove_mode);

int     is_builtin(char **args);
void    excute_builting(t_cmd **command, t_env *env_list, char *env[]);
char **convert_env_list(t_env **list);
void handel_signal(int sig);

// void ft_excute(t_cmd *head);

void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[]);
// void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[]);
// t_env *creat_new_env(char *key, char *value);
t_env *creat_new_env(char *key, char *value, int for_path);
void ft(char **args, t_env **list);
void handel_signal(int sig);
// void	remove_quotes_from_tokens(t_token *token_list);
// char	*remove_quotes(char *str);


// char *expand_token_data(char *original, t_env *env, int exit_status);



// int get_num_length(long num);
// void fill_num_string(char *str, long num, int len);
// char *ft_itoa(int n);
// int	ft_isalnum(int c);
// int is_valid_var_char(char c);
// int expand_handle_helper0(t_exp_helper *expand);
// int  expand_handle_helper1(t_exp_helper *expand, int exit_status , t_env *env);
// int expand_fill(t_exp_helper *expand, t_token *tmp);
// void process_token(t_token *tmp, t_exp_helper *expand, t_env *env, int exit_status);
// // void expand_handle(t_token *token_list, t_env *env, int exit_status);
// char *lookup_variable(char *var_name, t_env *env_struct);



// int handle_quotes(char c, int *quote_state);
// int special_character_cheker_with_quotes(char character, int quote_state);
// char *selective_remove_quotes(char *str, int remove_mode);
// //void process_quotes_for_tokens(t_token *token_list, int remove_mode);
// int	helper3(t_exp_helper *expand, int exit_status);
// int	expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env);
// void	process_token(t_token *tmp, t_exp_helper *expand,
//     t_env *env, int exit_status);
// char	*ft_itoa(int n);
// int	is_valid_var_char(char c);
// int	expand_fill(t_exp_helper *expand, t_token *tmp);
// // char	*lookup_variable(char *var_name, t_env *env_struct);
    
//     t_cmd *parser(t_token *token_list);
//     int is_redirection(char *str, int i);
//     char **ft_split_q(char const *s, char c);
//     void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode);
    
    
    
// //add back func
// void	add_cmd_back(t_cmd **lst, t_cmd *new);
// void	add_redir_back(t_redir **lst, t_redir *new);
// void	ft_lstadd_back_env(t_env **lst, t_env *new);
// void	ft_lstadd_back_token(t_token **lst, t_token *new);
    
// ///free func
// char	**free_split(char **split, size_t j);
// void free_env_struct(t_env *env_struct);
// void free_token_list(t_token *token_list);
// //void free_string_array(char **array, int count);
// void free_cmd_list(t_cmd *cmd_list);
// void free_split_str(char **split_str);
// char **free_split_q(char **split, size_t j);

// ///debugging tools
// void print_cmd(t_cmd *cmd_list);
// void	print_tokens(t_token *token_list);  ///// remove if the parsing finish
// void print_env(t_env *env_struct); // remove after the parsing finish

// void process_redir_helper(char str, int *quote_state);    
// char *redir_extracter(char *str);

// char	*ft_strjoin(char const *s1, char const *s2);
// t_redir *creat_redir_list(char *str);
// t_redir *creat_redir_list_helper0(char *str1, char *str2);
// int creat_redir_list_helper(char *str);
// static char *process_redir(char *str, int *pos);
// t_redir *creat_redir_node(int type, char *file);



// int	expand_fill_str(t_exp_helper *expand, char *str);
// void print_ambiguous_redir_errors(t_cmd *cmd);
// void ambiguous_finder(t_cmd *cmd);

// void	process_string(char *str, t_exp_helper *expand,
// 					t_env *env, int exit_status);


// int open_file(int type, char *file);
// void file_opener(t_cmd *cmd);


// char **split_if_needed(char *str);
// void free_string_array(char **array);
// int is_valid_var_char(char c);
// int is_in_assignment(char *str, int pos);
// char	*lookup_variable(char *var_name, t_env *env_struct);
// int is_special_export_case(t_cmd *cmd);
// void split_args_for_cmd(t_cmd *cmd_list);
// void apply_word_splitting(t_cmd *cmd_list);
// void print_file_error(char *file, int i);
#endif
    
    
    
     