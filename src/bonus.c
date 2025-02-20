/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:38 by eklymova          #+#    #+#             */
/*   Updated: 2025/02/20 13:51:17 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "pipex_bonus.h"

void	close_fd(int input_fd, int output_fd, int **pipes, int num_cmds)
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
