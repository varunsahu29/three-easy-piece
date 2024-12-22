#include <iostream>
#include <unistd.h> // For fork(), getpid()
int main() {
    int x = 100; // Initialize variable x
    std::cout << "Initial value of x: " << x << std::endl;
    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << "): Initial value of x = " << x << std::endl;
        x = 200; // Change x in the child process
        std::cout << "Child process (PID: " << getpid() << "): Updated value of x = " << x << std::endl;
    } else {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << "): Initial value of x = " << x << std::endl;
        x = 300; // Change x in the parent process
        std::cout << "Parent process (PID: " << getpid() << "): Updated value of x = " << x << std::endl;
    }

    // Both processes print their final value of x
    std::cout << "Process (PID: " << getpid() << "): Final value of x = " << x << std::endl;

    return 0;
}