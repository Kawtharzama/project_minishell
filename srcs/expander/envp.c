#include "../includes/minishell.h"

t_envp *copy_envp(char **envp, t_all *as)
	{
		
		int count ;
		int r ;
		r = 0;
		
		count = 0 ;
		while(envp[count])
			count++;
		as->cp_envp->tmp_envp = malloc(sizeof(char*) * (count+1)); //free
		if (!as->cp_envp->tmp_envp)
        	exit(1); //exit function
		while(envp[r])
		{
			as->cp_envp->tmp_envp[r] = ft_strdup(envp[r]);
        	if (!as->cp_envp->tmp_envp[r])
				exit(1); //exit function

			
			r++;
		}
		as->cp_envp->tmp_envp[r] = NULL;
		as->cp_envp->counter = r;
		return as->cp_envp;

	}
	void print_envp(t_envp *cp_envp)
	{
		int r = 0 ;
		while(cp_envp->tmp_envp[r])
		{
			printf("%s\n", cp_envp->tmp_envp[r]);
			r++;
		}

	}