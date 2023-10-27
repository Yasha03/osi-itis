#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Необходимо ввести аргумент N" << endl;
        return 1;
    }

    int N = atoi(argv[1]);

    if (N <= 0) {
        cout << "N должен быть больше нуля" << endl;
        return 1;
    }

    pid_t parentPid = getpid();

    for(int i = 0; i < N; i++) {
        pid_t childPid = fork();

        if (childPid == 0) {
            // дочерний процесс
            srand(time(nullptr));

            string randomNumber = to_string(rand() % 6 + 5);

            execlp("./child", "./child", randomNumber.c_str(), nullptr);

            exit(1);
        } else {
            // итерация основного процесса
            cout << "Parent[" << parentPid << "]: I run children process with PID " << childPid << "." << endl;

            int status;
            pid_t terminatedChild = wait(&status);

            cout << "Parent[" << parentPid << "]: Child with PID " << childPid << " terminated. Exit Status " << status << endl;

            if (status != 0) {
                i--;
            }
        }
    }

    return 0;
}
