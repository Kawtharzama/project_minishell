#include "../includes/minishell.h"

int free_envp(t_envp *cp_envp)
	{
		if(cp_envp && cp_envp->tmp_envp)
		{
			while(cp_envp->counter-- >= 0)
			{
				free(cp_envp->tmp_envp[cp_envp->counter]);
			}
			free(cp_envp->tmp_envp);
			free(cp_envp);
			return 1;
		}
		return 0;
	}
void free_all(t_all *as)
{
    if (as)
    {
        // clean(as->token);
        if (as->tmp)
        {
            free(as->tmp); //end of project
        } 
   
        if (as->cp_envp)
        {
            free_envp(as->cp_envp); //end of project
        }    
        free(as); //end of project
    }
}


void free_tokens(t_token *token)
{
    t_token *tmp;
    
    while (token)
    {
        tmp = token;
        token = token->next;
        if(tmp->value)
            free(tmp->value); 
        free(tmp);    //??    
    }
}



int clean(t_all *as)
{
    if(as)
    {
        free_tokens(as->token);
        // free_all(as);
    }
    return 0;
}
