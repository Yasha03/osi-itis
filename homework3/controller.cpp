// #include <iostream>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <cstdlib>
// #include <ctime>
// #include <sys/types.h>
// #include <signal.h>
// #include <string>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>


using namespace std;

int count = 0;

void sigusr1_handler(int signo) {
    if (signo == SIGUSR1) {
        cout << "Produced: " << count << endl;
    }
}

int main(int argc, char* argv[]) {

    srand(time(nullptr));
    
    int pipe1[2];
    int pipe0[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe0) == -1 || pipe(pipe2) == -1) {
        return 1;
    }

    signal(SIGUSR1, sigusr1_handler);

    pid_t childPid1 = fork();

    if (childPid1 == 0) {
        close(pipe1[0]);
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        execl("./producer", "producer", nullptr);
        return 1;
    }

    pid_t childPid2 = fork();

    if (childPid2 == 0) {
        close(pipe0[1]);
        dup2(pipe0[0], STDIN_FILENO);
        close(pipe0[0]);

        close(pipe2[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        execl("/usr/bin/bc", "bc", nullptr);
        return 1;

    } 

    cout << "Start controller. PID: " << getpid() << endl;
     
    close(pipe1[1]);

    while (true) {
        char expression[256];
          
        ssize_t read_size = read(pipe1[0], expression, sizeof(expression));

        if (read_size <= 0) {
            break;
        }

        pid_t terminatedChild;
        int status;

        do {
            terminatedChild = waitpid(-1, &status, WNOHANG);
            if (terminatedChild == childPid1) {
                break;
            }
        } while (terminatedChild > 0);

        if (terminatedChild == childPid1) {
            break;
        }

        expression[read_size] = '\0';

        write(pipe0[1], expression, read_size);

        char result[256];
        ssize_t result_size = read(pipe2[0], result, sizeof(result));

        result[result_size] = '\0';  

        for (int i = 0; i < sizeof(expression); i++) {
            if (expression[i] == '\n') expression[i] = '\0';
        }     

        std::cout << expression << " = " << result;
        count++;
    }

    close(pipe0[1]);
    close(pipe2[0]);
    
    waitpid(childPid1, nullptr, 0);

    return 0;
    
}
