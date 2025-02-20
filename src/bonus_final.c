#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "pipex.h"

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

void	create_pipes(int num_cmds, int **pipes)
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
		if (access(path, X_OK) == 0)
			return (free_arr(paths), ft_strdup(path));
		i++;
	}
	return (free_arr(paths), NULL);
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

void close_fd(int input_fd, int output_fd, int **pipes, int num_cmds)
{
	int	i;

	close(input_fd);
	close(output_fd);
	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	child_process(int i, int num_cmds, int **pipes, int input_fd, int output_fd, char *argv[], char *envp[])
{
	if (i == 0)
		dup2(input_fd, STDIN_FILENO);
	else
		dup2(pipes[i - 1][0], STDIN_FILENO);

	if (i == num_cmds - 1)
		dup2(output_fd, STDOUT_FILENO);
	else
		dup2(pipes[i][1], STDOUT_FILENO);
	close_fd(input_fd, output_fd, pipes, num_cmds);
	execute(argv[i + 2], envp);
}

int	**malloc_pipes(int num_cmds)
{
	int	i;
	int	**pipes;

	pipes = malloc(sizeof(int *) * (num_cmds - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		num_cmds;
	int		**pipes;
	int		input_fd;
	int		output_fd;
	int		i;
	pid_t	pid;

	if (argc < 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", 2);
		return (1);
	}
	num_cmds = argc - 3;
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1)
		return (1);
	output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (output_fd == -1)
		return (1);
	pipes = malloc_pipes(num_cmds);
	if (!pipes)
		return (1);
	create_pipes(num_cmds, pipes);
	i = 0;
	while (i < num_cmds)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			return (1);
		}
		if (pid == 0)
			child_process(i, num_cmds, pipes, input_fd, output_fd, argv, envp);
		i++;
	}
	close_fd(input_fd, output_fd, pipes, num_cmds);
	i = 0;
	while (i < num_cmds)
	{
		wait(NULL);
		i++;
	}
	free(pipes);
	return (0);
}
