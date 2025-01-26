/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:23 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/24 15:49:24 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_funk(int *fd, char *file1, char *cmd1, char **envp)
{
	int	input_file;

	input_file = open(file1, O_RDONLY, 0777);
	if (input_file == -1)
		exit(1);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		close(fd[0]);
		exit(1);
	}
	if (dup2(input_file, STDIN_FILENO) == -1)
	{
		close(fd[0]);
		exit(1);
	}
	close(fd[0]);
	execute(cmd1, envp);
}

void	parent_funk(int *fd, char *file2, char *cmd2, char **envp)
{
	int	output_file;

	output_file = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (output_file == -1)
		exit(1);
	if (dup2(fd[0], STDOUT_FILENO) == -1)
	{
		close(fd[1]);
		exit(1);
	}
	if (dup2(output_file, STDIN_FILENO) == -1)
	{
		close(fd[1]);
		exit(1);
	}
	close(fd[1]);
	execute(cmd2, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	fd[2];
	int	pid1;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			return (ft_putstr_fd("pipe failed", 2), 1);
		pid1 = fork();
		if (pid1 == -1)
			return (ft_putstr_fd("fork failed", 2), 1);
		if (pid1 == 0)
			child_funk(fd, argv[1], argv[2], envp);
		else
		{
			if (waitpid(pid1, NULL, 0) == -1)
				return (ft_putstr_fd("waitpid failed", 2), 1);
			parent_funk(fd, argv[4], argv[3], envp);
		}
	}
	else
		return (ft_putstr_fd("Wrong number of arguments\n", 2), 1);
    return (0);
}