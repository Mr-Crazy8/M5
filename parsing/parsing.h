/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:46:36 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 17:26:57 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ctype.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>

# define SET 0
# define GET 1

typedef struct t_data
{
	int		exit_status;
	char	*new_pwd;
}	t_data;

typedef struct s_token
{
	char			*data; // change to data
	char			*tokin; // change to tokin
	char			*befor_data_expand; // befor_data_expand
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_not_active;
	int				for_path;
	struct s_env	*next;
}	t_env;

typedef struct s_var
{
	char	*name;
	int		is_valid;
	int		length;
	int		name_allocated;
}	t_var;

typedef struct s_redir
{
	int				type;
	char			*file;
	int				*fd;
	int				ambiguous; // change to ambiguous
	char			*orig_token;
	char			*heredoc_delemter;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redirs;
	t_data			data;
	int				pipe_out;
	int				fd_pipe[2];
	char			**args_befor_quotes_remover;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exp_helper
{
	char	*original;
	char	*expanded;
	int		i;
	int		j;
	int		quote_state;
	char	*var_name;
	char	*var_value;
	int		k;
	int		start;
	size_t	buffer_size;
	int		var_expanded;
	int		had_removed_var;
}	t_exp_helper;

void	process_redir_helper(char str, int *quote_state);
void	scan_for_token(char *input, int *i, int input_length, int *start_index);
void	handle_block(char *input, int *i, int input_length,
			t_token **token_list);
void	handle_block_helper(char *input, int *i, t_token **token_list);
void	*ft_memcpy_q(void *dest, const void *src, size_t n);
void	count_words_q_hp(char s, int *quote_state);
void	split_string_q_hp0(char s, int *quote_state);
void	skip_redirections_hp(char str, int *quote_state);
void	cmd_extracter_hp_0(char str, int *quote_state);
void	check_missing_filename(char *str);
void	print_error(char *str, int *i);
void	redirection_helper(char *str, int *i, int *count);
void	cmd_extracter_hp_1(char *str, int *quote_state, int *i,
			int *result_len, char *result);
void	cmd_extracter_hp_2(char *str, int *i, char *result,
			int *result_len, int quote_state);
void	preprocess_cmd_hp_1(char *input, int *i, int *in_quotes,
			char *result, int *j);
void	value_extracter(t_exp_helper *expand, t_env *env);
void	free_env_struct(t_env *env_struct);
void	free_token_list(t_token *token_list);
void	free_split_array(char **split);
void	process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode);
void	process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode);
void	ft_putstr_fd(char *s, int fd);
void	print_file_error(char *file, int i);
void	file_opener(t_cmd *cmd, t_env *env);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_lstadd_back_token(t_token **lst, t_token *new);
void	add_redir_back(t_redir **lst, t_redir *new);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status);
void	free_redirs(t_redir *redir_list);
void	free_cmd_list(t_cmd *cmd_list);
void	ambiguous_checker(t_redir *redir);
void	free_string_array(char **array);
void	ambiguous_finder(t_cmd *cmd);
void	split_the_rest_helper(t_cmd *current, int *i);
void	split_the_rest(t_cmd *current);
void	apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand);
void	print_tokens(t_token *token_list);
void	free_split_str(char **split_str);
void	print_env(t_env *env_struct);
void	fill_num_string(char *str, long num, int len);
void	print_cmd(t_cmd *cmd_list);
void	process_string(char *str, t_exp_helper *expand, t_env *env,
			int exit_status, int pipe_out);
void	print_ambiguous_redir_errors(t_cmd *cmd);
void	*ft_calloc(size_t nmemb, size_t size);
void	check_here_doc(t_cmd *cmd, t_env *env);
void	free_expanded(t_exp_helper *expand);
void	prepare_new_args(char **new_args, t_cmd *current, int i);

char	*ft_itoa(int n);
char	*ft_strdup(const char *s);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*redir_extracter(char *str);
char	**ft_split_q(char const *s, char c);
char	*cmd_extracter_hp_3(char *result, int *result_len, int had_quotes);
char	*init_cmd_buffer(char *str, int *i, int *result_len, int *quote_state);
char	*preprocess_command(char *input);
char	*cmd_extracter(char *str);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*chenger_back(char *str);
char	*chenger(char *str);
char	**split_if_needed(char *str);
char	*ft_strtrim(char const *s1, char const *set);
char	*change_space(char *str);
char	**free_split_q(char **split, size_t j);
char	**free_split(char **split, size_t j);
char	*allocate_and_init(char *str, int *i, int *j, int *quote_state);
char	*selective_remove_quotes(char *str, int remove_mode);
char	*lookup_variable(char *var_name, t_env *env_struct);

int		key_is_var(char *str);
int		check_for_space(char *str);
int		is_var_key_append(char *original_arg);
int		is_append_assignment(char *str);
int		ft_strlen(const char *str);
int		signal_static(int type, int status);
int		here_doc_static(int type, int status);
int		has_quotes_in_previous_args(t_cmd *current, int current_index);
int		ft_lint(char **str);
int		helper3(t_exp_helper *expand, int exit_status, int pipe_out);
int		process_quote_char(char c, int *quote_state, char *new_str,
			int *j, int remove_mode);
int		expand_handle_helper0(t_exp_helper *expand);
int		ensure_buffer_space(t_exp_helper *expand, size_t additional_needed);
int		expand_fill_str(t_exp_helper *expand, char *str);
int		ft_isdigiti(int c);
int		expand_handle_helper1(t_exp_helper *expand, int exit_status,
			t_env *env, int pipe_out);
int		get_num_length(long num);
int		helper3(t_exp_helper *expand, int exit_status, int pipe_out);
int		extracting_the_key_value(t_exp_helper *expand, int exit_status,
			t_env *env, int pipe_out);
int		var_name_extract(t_exp_helper *expand);
int		alloc_var_name(t_exp_helper *expand);
int		preprocess_cmd_hp_0(char *input, int *i, int *in_quotes,
			char *result, int *j);
int		is_whitespace(char c);
int		special_character_cheker(char character);
int		handle_quotes(char c, int *quote_state);
int		special_character_cheker_with_quotes(char character, int quote_state);
int		ft_isalnum(int c);
int		split_string_q_hp1(char **split, size_t *i, size_t *j,
			size_t *start, char const *s);
int		creat_redir_list_helper(char *str);
int		is_valid_var_char(char c);
int		*open_file(t_cmd *cmd, int type, char *file, int Ambiguous);
int		preprocess_cmd_hp_2(char *input, int *i, int *in_quotes,
			char *result, int *j);
int		adding_var_value(t_exp_helper *expand);
int		check_for_redirection(char *str);
int		*heredoc_opener(void);
int		invalid_filename_checker(char *str, int *i);
int		check_invalid_filename(char *str);
int		check_quotes(char *input);
int		check_quotes_helper(int in_quote);
int		print_error_pipe(void);
int		error_pipi(t_token *token_list);
int		check_syntax_errors(t_token *token_list);
int		get_or_set(int type, int status);
int		is_valid_key(char *key);
int		rebuild_cmd_args(char **new_args, t_cmd *current, char **split,
			int i, int word_count);
t_token	*creat_token(char *data, char *token_type);
t_token	*special_character_handle(char *input, int *i);
t_token	*tokin_list_maker(char *input);
t_redir	*creat_redir_list_helper0(char *str1, char *str2);
t_redir	*creat_redir_list(char *str);
t_redir	*creat_redir_node(int type, char *file);
t_cmd	*creat_cmd_node(char *str, t_token *tp, int pipe_out);
t_cmd	*parser(t_token *token_list);

#endif