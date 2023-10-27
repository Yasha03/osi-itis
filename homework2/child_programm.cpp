#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Необходимо ввести аргумент S" << endl;
        return 1;
    }

    int S = atoi(argv[1]);

    if (S <= 0) {
        cout << "S должен быть больше нуля" << endl;
        return 1;
    }

    pid_t childPid = getpid();
    pid_t parentPid = getppid();

    cout << "Child[" << childPid << "]: I am started. My PID " << childPid << ". Parent PID " << parentPid <<  "." << endl;

    sleep(S);

    cout << "Child[" << childPid << "]: I am ended. PID " << childPid << ". Parent PID " << parentPid << "." << endl;

    srand(time(nullptr));
    
    exit(rand() % 2);
}
