#include <iostream>
#include <unistd.h> // For pipe(), fork(), dup2(), close()
#include <sys/types.h>
#include <sys/wait.h> // For wait()

int main() {
    int pipe_fd[2]; // Array to hold the pipe file descriptors
    if (pipe(pipe_fd) == -1) {
        std::cerr << "Pipe creation failed." << std::endl;
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        std::cerr << "Fork for child 1 failed." << std::endl;
        return 1;
    }

    if (pid1 == 0) {
        // First child process
        close(pipe_fd[0]); // Close the read end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(pipe_fd[1]); // Close the write end of the pipe (not needed after dup2)

        // Execute a command that writes to stdout
        execlp("ls", "ls", NULL); // Replace process image with "ls"
        perror("execlp failed"); // Only executed if execlp fails
        _exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        std::cerr << "Fork for child 2 failed." << std::endl;
        return 1;
    }

    if (pid2 == 0) {
        // Second child process
        close(pipe_fd[1]); // Close the write end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
        close(pipe_fd[0]); // Close the read end of the pipe (not needed after dup2)

        // Execute a command that reads from stdin
        execlp("wc", "wc", "-l", NULL); // Replace process image with "wc -l"
        perror("execlp failed"); // Only executed if execlp fails
        _exit(1);
    }

    // Parent process
    close(pipe_fd[0]); // Close the read end of the pipe
    close(pipe_fd[1]); // Close the write end of the pipe

    // Wait for both children to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    std::cout << "Parent process: Both children have finished." << std::endl;

    return 0;
}