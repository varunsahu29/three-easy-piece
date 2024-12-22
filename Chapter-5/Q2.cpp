#include <iostream>
#include <unistd.h> // For fork(), write()
#include <fcntl.h>  // For open(), O_WRONLY, O_CREAT
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include<string>
#include <ctime>
#include <iomanip>
#include <sstream>
std::string get_current_time_with_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&ts.tv_sec), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setw(9) << std::setfill('0') << ts.tv_nsec;
    return oss.str();
}
int main() {
    // Open a file for writing
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    const int COUNT=50;
    if (fd < 0) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    pid_t pid = fork(); // Create a new process

    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed." << std::endl;
        close(fd);
        return 1;
    } else if (pid == 0) {
        // Child process
        const char* child_msg = "Child process writing...\n";
        std::string s;
        for (int i = 0; i < COUNT; ++i) {
            time_t now =time(0);
            char *dt=ctime(&now);
            s += get_current_time_with_ns();
            s+=child_msg;
        }
        write(fd, s.c_str(), s.length());
    } else {
        // Parent process
        const char* parent_msg = "Parent process writing...\n";
        std::string s;
        for (int i = 0; i < COUNT; ++i) {
            time_t now =time(0);
            char *dt=ctime(&now);
            s += get_current_time_with_ns();
            s+=parent_msg;
        }
        write(fd, s.c_str(), s.length());
    }

    // Close the file descriptor in both processes
    close(fd);
    return 0;
}