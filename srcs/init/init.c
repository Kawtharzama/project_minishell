#include "../includes/minishell.h"
void	init_token(t_tmptoken *tmp)
{
	tmp->start = 0;
	tmp->end = 0;
	tmp->value = NULL;
}

t_all *init_strcuts(t_all *as)
{     
    as = malloc(sizeof(t_all)); //free
    if(!as) exit(1); 
    as->exit_status = 0;
    as->token = NULL;
    as->cmd =NULL;

    as->tmp = malloc(sizeof(t_tmptoken)); //free
    if (!as->tmp)
    {
        free(as); 
        exit(1);//exit function
    }
  
    as->cp_envp = malloc(sizeof(t_envp)); //free
		if (!as->cp_envp)
            exit(1);//exit
    return as;
}