/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:38 by eklymova          #+#    #+#             */
/*   Updated: 2025/02/21 19:08:16 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_fd(t_command commands, int **pipes)
{
	int	i;

	i = 0;
	while (i < commands.num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	child_process(int i, int **pipes, char *envp[], t_command commands)
{
	if (i == 0)
	{
		if (dup2(commands.input_fd, STDIN_FILENO) == -1)
			error (1);
	}
	else
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			error (1);
	}
	if (i == commands.num_cmds - 1)
	{
		if (dup2(commands.output_fd, STDOUT_FILENO) == -1)
			error (1);
	}
	else
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			error (1);
	}
	close(commands.input_fd);
	close(commands.output_fd);
	close_fd(commands, pipes);
	execute(commands.args[i + 2], envp);
}

int	**malloc_pipes(t_command commands)
{
	int	i;
	int	**pipes;

	pipes = malloc(sizeof(int *) * (commands.num_cmds - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < commands.num_cmds - 1)
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

void	fork_plz(t_command commands, int **pipes, char **envp)
{
	int		i;
	pid_t	pid;

	i = 0;
	commands.input_fd = open(commands.args[1], O_RDONLY);
	if (commands.input_fd == -1)
		error (4);
	commands.output_fd = open(commands.args[commands.argc - 1],
			O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (commands.output_fd == -1)
		error (4);
	while (i < commands.num_cmds)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
			child_process(i, pipes, envp, commands);
		i++;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int			**pipes;
	int			i;
	t_command	commands;

	if (argc < 6)
		return (ft_putstr_fd("Error\n", 2), 1);
	if (argv[1][0] == '\0' || argv[2][0] == '\0' || argv[3][0] == '\0')
		return (ft_putstr_fd("Error\n", 2), 1);
	commands.argc = argc;
	commands.num_cmds = argc - 3;
	commands.args = argv;
	pipes = malloc_pipes(commands);
	if (!pipes)
		return (1);
	create_pipes(commands.num_cmds, pipes);
	fork_plz(commands, pipes, envp);
	close_fd(commands, pipes);
	i = 0;
	while (i < commands.num_cmds)
	{
		wait(NULL);
		i++;
	}
	free_arr(0, pipes, commands.num_cmds, 1);
	return (0);
}
