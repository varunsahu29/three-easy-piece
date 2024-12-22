#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cassert>

int main() {
    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed." << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ") started." << std::endl;
        
        // Attempt to wait for a child process (which does not exist)
        int status;
        pid_t child_pid = wait(&status);
        if (child_pid == -1) {
            std::cerr << "Child process: wait() failed, no child processes to wait for." << std::endl;
        }
        
        sleep(2); // Simulate some work in the child process
        std::cout << "Child process (PID: " << getpid() << ") finished." << std::endl;
        exit(42); // Exit with status 42
    } else {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ") waiting for child." << std::endl;

        // Wait for the child process to finish
        int status;
        std::cout<<"Status before:"<<status<<std::endl;
        pid_t child_pid = wait(&status);
        std::cout<<"Status after wait:"<<status<<std::endl;
        if (child_pid > 0) {
            std::cout << "Parent process: Child (PID: " << child_pid << ") exited with status " << WEXITSTATUS(status) << "." << std::endl;
        } else {
            std::cerr << "Parent process: wait() failed." << std::endl;
        }
    }

    return 0;
}