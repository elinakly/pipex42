#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "pipex.h"

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	if (file == -1)
		error(1);
	return (file);
}

int	error(int status)
{
	if (status == 1)
		perror("Dup2 failed");
	else if (status == 2)
		perror("Path failed");
	else if (status == 3)
		perror("Execve failed");
	else if (status == 126)
		ft_putstr_fd("Error: command not found\n", 2);
	else if (status == 127)
		ft_putstr_fd("Execve failed\n", 2);
	exit (status);
}

void	create_pipes(int num_cmds, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*find_valid_path(const char *com, char **envp)
{
	int		i;
	char	**paths;
	char	path[PATH_MAX];

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		ft_strlcpy(path, paths[i], PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, com, PATH_MAX);
		if (access(path, F_OK) == 0)
			return (free_arr(paths), ft_strdup(path));
		i++;
	}
	return (free_arr(paths), ft_strdup(com));
}

void	execute(char *com, char **envp)
{
	char	*find_path;
	char	**command;

	command = ft_split(com, ' ');
	if (!command)
		exit(error(3));
	find_path = find_valid_path(command[0], envp);
	if (command[0] == NULL)
	{
		free_arr(command);
		exit(error(3));
	}
	if (find_path == NULL)
	{
		free_arr(command);
		exit(error(127));
	}
	if (execve(find_path, command, envp) == -1)
	{
		free(find_path);
		free_arr(command);
		exit(error(126));
	}
}

void	child_process(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error(1);
	pid = fork();
	if (pid == -1)
		error(1);
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute(argv, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	int		num_cmds;
    int		pipes[argc - 4][2];
    int		input_fd;
    int		output_fd;
    int		i;

    if (argc < 5)
    {
        ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", 2);
        return (1);
    }
    num_cmds = argc - 3;
    input_fd = open_file(argv[1], 2);
    output_fd = open_file(argv[argc - 1], 1);

    create_pipes(num_cmds, pipes);

    for (i = 0; i < num_cmds; i++)
    {
        if (fork() == 0)
        {
            if (i == 0)
            {
                dup2(input_fd, STDIN_FILENO);
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            else if (i == num_cmds - 1)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                dup2(output_fd, STDOUT_FILENO);
            }
			else
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < num_cmds - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            execute(argv[i + 2], envp);
        }
    }

    for (i = 0; i < num_cmds - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    close(input_fd);
    close(output_fd);

    for (i = 0; i < num_cmds; i++)
        wait(NULL);

    return (0);
}
