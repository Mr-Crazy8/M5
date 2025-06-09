#ifndef PARSING_H
# define PARSING_H


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>  // For printf
#include <stdlib.h> // For malloc
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<limits.h>
#include <errno.h>

#define SET 0
#define GET 1

int key_is_var(char *str);
int is_var_key_append(char *original_arg);
int is_append_assignment(char *str);
char *chenger_back(char *str);
// void remove_node(void *addr_str, t_collet **head);
// void ft_free_unused(t_collet **addres_list);
// void ft_in_use(void *adders, t_collet *addres_list);
// void ft_not_in_use(void *adders, t_collet *addres_list);
// void	ft_lstadd_back_addres(t_collet **lst, t_collet *new);
// t_collet	*addres_node_maker(void *addres, size_t size);
// t_collet *collet(t_collet *addres_list, void *adders, size_t size);
// void	*ft_malloc(t_collet **addres_list,size_t size);
// char	*addres_to_string(void *ptr);
// void ft_free(void *addr, t_collet **addres_list);
int	ft_strlen(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);

typedef struct t_data
{
    int     exit_status;
    char    *new_pwd;
} t_data;

typedef struct s_token
{
   char *DATA;
   char *TOKIN;
   char *befor_DATA_expand;
   struct s_token *next;
} t_token;

typedef struct s_env
{
    char    *key;
    char    *value;
    int     is_not_active;
    int     for_path;
    struct s_env *next;
} t_env;

typedef struct s_var {
    char *name;         // Variable name
    int is_valid;       // Is this a valid variable ref
    int length;         // Length of entire reference
    int name_allocated; // Did we allocate memory for name
} t_var;

typedef struct s_redir
{
    int type;             /* 0:<, 1:>, 2:>>, 3:<< */
    char *file;           /* Filename or heredoc delimiter */
    int *fd;
    int Ambiguous;
    char    *orig_token;  /* Original token with quotes */
    char *heredoc_delemter; /* Fully processed delimiter for matching */
    struct s_redir *next; /* Next redirection */
} t_redir;

typedef struct s_cmd
{
    char *cmd;            /* Command name */
    char **args;          /* Command args (including cmd as args[0]) */
    t_redir *redirs;      /* Redirections list */
    t_data    data;
    int pipe_out;         /* 1 if command pipes to next */
    int     fd_pipe[2];
    char **args_befor_quotes_remover;
    struct s_cmd *next;   /* Next command in pipeline */
} t_cmd;


typedef struct s_exp_helper {
    
    char *original;
    char *expanded;
    int i;
    int j;
    int quote_state;
    char *var_name;
    char *var_value;
    int k;
    int start;
    size_t buffer_size;
    int var_expanded;
    int had_removed_var;
    
} t_exp_helper;



void check_here_doc(t_cmd *cmd, t_env *env);
int	signal_static(int type, int status);
int	here_doc_static(int type, int status);


int has_quotes_in_previous_args(t_cmd *current, int current_index);
int	ft_isdigiti(int c);
char *chenger(char *str);
char *trim_whitespace(char *str);
int pls_conter(char *str);
int helper3(t_exp_helper *expand, int exit_status, int pipe_out);
int is_valid_var_name(char *str, int len);
int should_split_arg(char *arg, char *original_arg);
void free_expanded(t_exp_helper *expand);
void	ft_putstr_fd(char *s, int fd);
void print_file_error(char *file, int i);
void file_opener(t_cmd *cmd, t_env *env);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_lstadd_back_token(t_token **lst, t_token *new);
void	add_redir_back(t_redir **lst, t_redir *new);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void ambiguous_checker(t_redir *redir);
void ambiguous_finder(t_cmd *cmd);
void	print_tokens(t_token *token_list);
void	print_env(t_env *env_struct);
void print_cmd(t_cmd *cmd_list);
void print_ambiguous_redir_errors(t_cmd *cmd);
char	*ft_strtrim(char const *s1, char const *set);
int process_quote_char(char c, int *quote_state, char *new_str, int *j, int remove_mode);
char *allocate_and_init(char *str, int *i, int *j, int *quote_state);
char *selective_remove_quotes(char *str, int remove_mode);
void free_split_array(char **split);
void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode);
void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode);
char *lookup_variable(char *var_name, t_env *env_struct);
int expand_fill_str(t_exp_helper *expand, char *str);
char **split_if_needed(char *str);
void free_string_array(char **array);
int check_var_quotes(char *orig_arg, char *orig_equals);
int check_export_cmd(t_cmd *cmd);
int is_special_export_case(t_cmd *cmd);
int ft_lint(char **str);
void cmd_splitting_helper(t_cmd *current, char **new_args, char **split, int word_count, int arg_count);
void prepare_new_args(char **new_args, t_cmd *current, int i);
int rebuild_cmd_args(char **new_args, t_cmd *current, char **split, 
                    int i, int word_count);
void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i);
void split_the_rest(t_cmd *current, int should_split, int had_removed_var);
void cmd_splitting(t_cmd *cmd_list );
void apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand);
int expand_handle_helper0(t_exp_helper *expand);
int ensure_buffer_space(t_exp_helper *expand, size_t additional_needed);
void process_string(char *str, t_exp_helper *expand, t_env *env, int exit_status, int pipe_out);
int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out);
int helper3(t_exp_helper *expand, int exit_status, int pipe_out);
void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status);
void free_redirs(t_redir *redir_list);
void free_cmd_list(t_cmd *cmd_list);
char	**free_split(char **split, size_t j);
void free_env_struct(t_env *env_struct);
void	free_token_list(t_token *token_list);
char **free_split_q(char **split, size_t j);
void free_split_str(char **split_str);
int	get_num_length(long num);
char *change_space(char *str);
void	fill_num_string(char *str, long num, int len);
char	*ft_itoa(int n);
int	ft_isalnum(int c);
int	is_valid_var_char(char c);
// int	ft_strlen(const char *str);
char	*ft_strdup(const char *s);
t_token	*creat_token(char *data, char *token_type);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
int	is_whitespace(char c);
int	special_character_cheker(char character);
t_token	*special_character_handle(char *input, int *i);
void	handle_block_helper(char *input, int *i, t_token **token_list);
int handle_quotes(char c, int *quote_state);
int special_character_cheker_with_quotes(char character, int quote_state);
void scan_for_token(char *input, int *i, int input_length, int *start_index);
void handle_block(char *input, int *i, int input_length, t_token **token_list);
t_token	*tokin_list_maker(char *input);
int *open_file(t_cmd *cmd, int type, char *file);
int *heredoc_opener(void);
// void print_file_error(char *file, int i, int Ambiguous);
char	*ft_strjoin(char const *s1, char const *s2);
void process_redir_helper(char str, int *quote_state);
char *redir_extracter(char *str);
int creat_redir_list_helper(char *str);
t_redir *creat_redir_list_helper0(char *str1, char *str2);
t_redir *creat_redir_list(char *str);
void *ft_memcpy_q(void *dest, const void *src, size_t n);
void count_words_q_hp(char s, int *quote_state);
void split_string_q_hp0(char s, int *quote_state);
int split_string_q_hp1(char **split, size_t *i, size_t *j, size_t *start, char const *s);
char **ft_split_q(char const *s, char c);
void skip_redirections_hp(char str, int *quote_state);
void  cmd_extracter_hp_0(char str, int *quote_state);
void  cmd_extracter_hp_1(char *str, int *quote_state, int *i, int *result_len, char *result);
void cmd_extracter_hp_2(char *str, int *i, char *result, int *result_len, int quote_state);
char *cmd_extracter_hp_3(char *result, int *result_len, int had_quotes);
char *init_cmd_buffer(char *str, int *i, int *result_len, int *quote_state);
char *cmd_extracter(char *str);
t_redir *creat_redir_node(int type, char *file);
t_cmd *creat_cmd_node(char *str,  t_token *tp,  int pipe_out);
t_cmd *parser(t_token *token_list);
int preprocess_cmd_hp_0(char *input, int *i, int *in_quotes, char *result, int *j);
void preprocess_cmd_hp_1(char *input, int *i, int *in_quotes, char *result, int *j);
int preprocess_cmd_hp_2(char *input, int *i, int *in_quotes, char *result, int *j);
char *preprocess_command(char *input);
int	check_for_redirection(char *str);
void	check_missing_filename(char *str);
void	print_error(char *str, int *i);
void	redirection_helper(char *str, int *i, int *count);
int	invalid_filename_checker(char *str, int *i);
int	check_invalid_filename(char *str);

int	check_quotes(char *input);
int	check_quotes_helper(int in_quote);
int	print_error_pipe(void);
int	error_pipi(t_token *token_list);
int	check_syntax_errors(t_token *token_list);
int get_or_set(int type, int status);
int	is_valid_key(char *key);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

int extracting_the_key_value(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out);
int var_name_extract(t_exp_helper *expand);
int alloc_var_name(t_exp_helper *expand);
void value_extracter(t_exp_helper *expand, t_env *env);
int adding_var_value(t_exp_helper *expand);

#endif