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

int	open_file(const char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		perror("file open failed");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	child_process(int i, int num_cmds, int pipes[][2],
		int input_fd, int output_fd, char *argv[], char **envp)
{
	if (i == 0) {
		dup2(input_fd, STDIN_FILENO);
	} else {
		dup2(pipes[i - 1][0], STDIN_FILENO);
	}
	if (i == num_cmds - 1) {
		dup2(output_fd, STDOUT_FILENO);
	} else {
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	for (int j = 0; j < num_cmds - 1; j++) {
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	close(input_fd);
	close(output_fd);
	char *cmd_args[] = {argv[i + 2], NULL};
	execve(cmd_args[0], cmd_args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}



int main(int argc, char *argv[], char *envp[])
{
	int	num_cmds = argc - 3;
	int	pipes[num_cmds - 1][2];
	pid_t pids[num_cmds];
	create_pipes(num_cmds, pipes);
	int input_fd = open_file(argv[1], O_RDONLY, 0);
	int output_fd = open_file(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	for (int i = 0; i < num_cmds; i++) {
		pids[i] = fork();
		if (pids[i] == -1) {
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0) {
			child_process(i, num_cmds, pipes, input_fd, output_fd, argv, envp);
		}
	}
	for (int i = 0; i < num_cmds - 1; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	close(input_fd);
	close(output_fd);
	for (int i = 0; i < num_cmds; i++) {
		waitpid(pids[i], NULL, 0);
	}
	return 0;
}
