#include <iostream>
#include <unistd.h> // For fork(), getpid()
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // Create a pipe for synchronization
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        std::cerr << "Failed to create pipe" << std::endl;
        return 1;
    }

    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
    std::cout<<"Current state of pipe_fd[0]: "<<pipe_fd[0]<<" pipe_fd[1]: "<<pipe_fd[1]<<std::endl;
        // Child process
        close(pipe_fd[0]); // Close the read end of the pipe
        std::cout << "hello" << std::endl;
        // Notify the parent that the child is done
        write(pipe_fd[1], "done", 4);
        close(pipe_fd[1]); // Close the write end of the pipe
    } else {
    std::cout<<"Current state of pipe_fd[0]: "<<pipe_fd[0]<<" pipe_fd[1]: "<<pipe_fd[1]<<std::endl;
        // Parent process
        close(pipe_fd[1]); // Close the write end of the pipe
        char buffer[4];
        // Wait for the child to notify via the pipe
        read(pipe_fd[0], buffer, 4);
        std::cout << "goodbye" << std::endl;
        close(pipe_fd[0]); // Close the read end of the pipe
    }

    return 0;
}