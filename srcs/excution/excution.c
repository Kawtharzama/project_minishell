#include "../includes/minishell.h"
t_command *new_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->next = NULL;
    return cmd;
}
char **add_arg(char **args, char *value)
//appends a new argument string to the end of the args array (resizing as needed)
{
        int count; //to count amount of args
        int i;

        i = 0;
        count = 0;
        while (args && args[count])
                count++;

        char **new_args = malloc(sizeof(char *) * (count + 2)); //allocates one more for the new arg and NULL
        if (!new_args)
                return NULL;

        while (i < count)
        // copy existing args to new array
                {new_args[i] = args[i];
                        i++;}
        new_args[count] = ft_strdup(value); //adds the new arg to the end
        new_args[count + 1] = NULL;

        if (args)
                free(args); // reallocated, old pointer is no longer used
        //frees the old args array but not the strings inside it
        return new_args;
}

void append_command(t_command **cmd_list, t_command *new_cmd)
//appends a new command to the end of the command list
{
        if (!*cmd_list)
        // if the list is empty, set the new command as the head
        {
                *cmd_list = new_cmd;
                return;
        }

        t_command *tmp = *cmd_list; //start from the head
        while (tmp->next)
        //walk through the list until the last command
                tmp = tmp->next;

        tmp->next = new_cmd; // link the new command to the end of the list
}

t_command *split_cmds(t_token *token)
//splits the token list into a command list
{
        t_command *cmd_list = NULL; //starts with an empty command list
        t_command *current_cmd = new_command();

        while (token)
        //iterate through the token list
        {
                if (token->type == WORD)
                {
                        current_cmd->args = add_arg(current_cmd->args, token->value);
                }
                else if (token->type == REDIR)
                //then next token should be a filename
                {
                        if (!token->next || token->next->type != WORD)
                        {
                                printf("Syntax error: expected filename after redirection\n");
                                free(current_cmd); //full cleanup function
                                return NULL;
                        }

                        if (ft_strncmp(token->value, ">", 1) == 0)
                        {
                                if (ft_strncmp(token->value, ">>", 2) == 0)
                                        current_cmd->append = 1;
                                current_cmd->outfile = ft_strdup(token->next->value);
                        }
                        else if (ft_strncmp(token->value, "<", 1) == 0)
                        {
                                current_cmd->infile = ft_strdup(token->next->value);
                        }

                        token = token->next; // skip filename
                }
                else if (token->type == PIPE)
                //finially, if we hit a pipe, we need to add the current command to the list
                {
                        append_command(&cmd_list, current_cmd);
                        current_cmd = new_command();
                }

                token = token->next;
        }

        if (current_cmd->args) // only add if it has something
                append_command(&cmd_list, current_cmd);

        return cmd_list; //return the command list
}

void execute_commands(t_command *cmd_list, t_envp *env)
{
        int i;
        int num_commands = 0;
         t_command *cmd = cmd_list;

        i = 0;
        int status;
        while (cmd)
        // iterate through the command list
        {
                // Execute each command
                pid_t pid = fork();
                if (pid == -1)
                {
                        perror("fork mesh nafe3");
                        return;
                }
                if (pid == 0)
                // if child process
                // calls execve() with arguments
                {
                        // Redirect input/output if needed
                        if (cmd->infile)
                        {
                                int fd_in = open(cmd->infile, O_RDONLY);
                                if (fd_in == -1)
                                {
                                        perror("open infile");
                                        exit(EXIT_FAILURE);
                                }
                                dup2(fd_in, STDIN_FILENO);
                                close(fd_in);
                        }

                        if (cmd->outfile)
                        {
                                int fd_out;
                                if (cmd->append)
                                        fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644); // here we give it permissins so when it is created it know what permissions to give it otherwise there will be a permission denied error
                                else
                                        fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                if (fd_out == -1)
                                {
                                        perror("open outfile");
                                        exit(EXIT_FAILURE);
                                }
                                dup2(fd_out, STDOUT_FILENO);
                                close(fd_out);
                        }

    

                        // if built in cmd
                        char *path = find_path(env, cmd->args[0]);
                        if (!path)
                        {
                                perror("path not found");
                                exit(127) ;
                        }
                        printf("%s",path);
                        execv(path, cmd->args);
                        perror("execve");
                        exit(EXIT_FAILURE);
                }
                num_commands++;
                cmd = cmd->next;
        }
        // Wait for all child processes to finish
        while (i < num_commands)
        {
                wait(&status);
                // waits for any child process to terminate and stores it exit status
                // ensures no zombie processes are left
                i++;
        }
}
// execve() works only with absolute paths like bin/ls
//  need to add path to the command PATH

/*
-Redirect stdin fd[0] if cmd->infile is not NULL
-Redirect stdout fd[1] if cmd->outfile is not NULL (in append or truncate mode)
*/