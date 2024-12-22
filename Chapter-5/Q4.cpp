#include <iostream>
#include <unistd.h> // For fork(), exec*(), _exit()
#include <cstdlib>  // For environ (global environment variables)

extern char** environ; // Global environment variables

void run_with_execl() {
    std::cout << "Running with execl()\n";
    execl("/bin/ls", "ls", "-l", NULL);
}

void run_with_execle() {
    std::cout << "Running with execle()\n";
    execle("/bin/ls", "ls", "-l", NULL, environ);
}

void run_with_execlp() {
    std::cout << "Running with execlp()\n";
    execlp("ls", "ls", "-l", NULL);
}

void run_with_execv() {
    std::cout << "Running with execv()\n";
    const char* args[] = {"/bin/ls", "-l", NULL};
    execv(args[0], const_cast<char* const*>(args));
}

void run_with_execvp() {
    std::cout << "Running with execvp()\n";
    const char* args[] = {"ls", "-l", NULL};
    execvp(args[0], const_cast<char* const*>(args));
}

void run_with_execvpe() {
    std::cout << "Running with execvpe()\n";
    const char* args[] = {"ls", "-l", NULL};
    execvpe(args[0], const_cast<char* const*>(args), environ);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return 1;
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ")\n";

        // Choose one of the exec* variants to run
        // Uncomment one of the following lines to test each variant:

        run_with_execl();
        // run_with_execle();
        // run_with_execlp();
        // run_with_execv();
        // run_with_execvp();
        // run_with_execvpe();

        // If exec() fails, exit
        perror("exec failed");
        _exit(1);
    } else {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ")\n";
    }

    return 0;
}