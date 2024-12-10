#include <iostream>
#include <unistd.h>  // POSIX: getcwd()
#include <limits.h>  // PATH_MAX

int main() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }
    return 0;
}
