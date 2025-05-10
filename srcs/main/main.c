#include "../includes/minishell.h"

void print_commands(t_command *cmd_list)
{
    int i;
    int cmd_num = 1;
    t_command *cmd = cmd_list;

    while (cmd)
    {
        printf("=== Command %d ===\n", cmd_num);

        // Print arguments
        if (cmd->args)
        {
            printf("Arguments:\n");
            for (i = 0; cmd->args[i]; i++)
                printf("  [%d]: %s\n", i, cmd->args[i]);
        }
        else
        {
            printf("Arguments: None\n");
        }

        // Print infile
        if (cmd->infile)
            printf("Infile: %s\n", cmd->infile);
        else
            printf("Infile: None\n");

        // Print outfile
        if (cmd->outfile)
            printf("Outfile: %s (append: %s)\n", cmd->outfile, cmd->append ? "yes" : "no");
        else
            printf("Outfile: None\n");

        printf("\n");
        cmd = cmd->next;
        cmd_num++;
    }
}



char *get_full_path(char *dir, char *cmd) {
    char *addslash;
    char *fullpath;

    addslash = ft_strjoin(dir, "/");
    fullpath = ft_strjoin(addslash, cmd);
    free(addslash);

    if (access(fullpath, X_OK) == 0) {
        return fullpath;
    }

    free(fullpath);
    return NULL;
}

char *find_path(t_envp *cp_envp, char *cmd) {
    char *path;
    char *start;
    char *end;
    char *fullpath;

    path = ft_getenv("PATH", cp_envp);
    if (!path) 
        return NULL;
    
    start = path;
    while ((end = ft_strchr(start, ':')) || *start) 
    {
        if (end) 
            *end = '\0';     
        fullpath = get_full_path(start, cmd);
        if (fullpath) 
        {
            printf("%s\n", fullpath);
            return fullpath;
        } //delete it before submit
        free(fullpath);
        if (end) 
            start = end + 1; 
        else
            break; //does check the last dir?
    }
    return NULL;
}




   






   

int main(int argv, char** argc, char** envp)
{
    (void)argv;
    (void)argc;
    (void)envp;
    char *input;
    t_all *as;
    as = NULL;
    as = init_strcuts(as);
    if(!as)
    {
        printf("not as");
        exit(1);//free then exit
    }
    copy_envp(envp, as);
	// print_envp(as->cp_envp);
    while(1)
    {
        input = readline("input: "); //add path
        if (input == NULL)
		{
			write(1, "exit\n", 5); 
            free(input);

			break;
		}//add empty string
        add_history(input);
        // char *x= expand_variables(input,as);//move to before execve
        // printf("%s",x);
        toknize(input,as);
       
        // print_list(as->token);
       
        


        free(input);
        clean(as);
        
        //clean all except in init
    }
    clean(as); //causes seg fault
    return 0;
}