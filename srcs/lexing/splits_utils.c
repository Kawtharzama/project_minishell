#include "../includes/minishell.h"

int toknize(char* input, t_all *as)
{
    split_input(input, &as->token, as->tmp);
	expand_var(as->token, as->cp_envp);
	remove_quotes(as->token);
	find_path(as->cp_envp, "cat");


	// split_cmds(as->token, &as->cmd);
	
    return 0;
}

void token_types(t_token *token) 

{
	 while(token)  {
		if (ft_strncmp(token->value, "<<",2) == 0) {
            token->type = HEREDOC;
        }
		else if  (ft_strncmp(token->value, ">>", 2) == 0 ||(ft_strncmp(token->value, ">",1) == 0) || (ft_strncmp(token->value, "<", 1) == 0) ) {
            token->type = REDIR;//addft_strcmp
	        }

		
        else if (ft_strncmp(token->value, "|",1) == 0) {
            token->type = PIPE;
        }
        else {
			
            	token->type = WORD;       

        }
        token = token->next;
       }  
}

int	parameter_token(char *input, int i, t_tmptoken *tmp, t_token **token)
{
	tmp->start = i;
	if (input[i + 1] && input[i] == input[i + 1] && input[i + 1] != '|')
		i++;
	tmp->end = i;
	tmp->value = ft_substr(input, tmp->start, (tmp->end - tmp->start
				+ 1)); //free
	if (!tmp->value)
	{
		printf("Memory allocation failed for tmptoken.value\n");
		return (-1);
	}
	if(add_node(token, tmp->value) == -1)
	{return -1;} //add if -1
	free(tmp->value);
	tmp->value = NULL;
	return (i);
}

int	is_parameter(char c)
{
	if (c == '|' || c == '>' || c == '<')
	{
		return (1);
	}
	return (0);
}

int	str(char *input, int i, t_tmptoken *tmp, t_token **token)
{
	tmp->start = i;
	int flag =0;
	while (input[i] && input[i] !=' ' && !is_parameter(input[i]))
		
	{
		if (input[i] == '"' || input[i] == '\'')
		{
		    i = closing_qoutes(input,i);
			flag = 1;
		}
		i++;
		if(flag == 1)
			break;
	}
	tmp->end = i;
	tmp->value = ft_substr(input, tmp->start, (tmp->end - tmp->start));
	if (!tmp->value)
	{
		printf("Memory allocation failed for token.value\n");
		return (-1);
	}
	if(add_node(token, tmp->value) == -1)
	{return -1;} 
	free(tmp->value);
	tmp->value = NULL;
	return (i - 1);
}