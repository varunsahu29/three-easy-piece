#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void childTask(int childNumber) {
    std::cout << "Child " << childNumber << " (PID: " << getpid() << ") doing work." << std::endl;
    sleep(5); // Simulate work
    std::cout << "Child " << childNumber << " (PID: " << getpid() << ") finished." << std::endl;
    _exit(childNumber); // Exit with the child number as the status
}

int main() {
    pid_t pid1, pid2;

    // Create two child processes
    pid1 = fork();
    if (pid1 == 0) {
        // First child
        childTask(1);
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Second child
        childTask(2);
    }

    // Parent process
    std::cout << "Parent (PID: " << getpid() << ") waiting for children." << std::endl;

    int status;

    // Case 1: Wait for a specific child (pid1)
    pid_t result = waitpid(pid1, &status, 0);
    if (result > 0) {
        std::cout << "Parent: Child " << result << " exited with status " << WEXITSTATUS(status) << "." << std::endl;
    }

    // Case 2: Wait for any child (-1)
    result = waitpid(-1, &status, 0);
    if (result > 0) {
        std::cout << "Parent: Child " << result << " exited with status " << WEXITSTATUS(status) << "." << std::endl;
    }

    // Case 3: Use non-blocking option (WNOHANG)
    result = waitpid(-1, &status, WNOHANG);
    if (result == 0) {
        std::cout << "Parent: No child has exited yet (non-blocking)." << std::endl;
    } else if (result > 0) {
        std::cout << "Parent: Child " << result << " exited with status " << WEXITSTATUS(status) << " (non-blocking)." << std::endl;
    }

    return 0;
}