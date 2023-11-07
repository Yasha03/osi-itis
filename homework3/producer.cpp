#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    
    srand(time(nullptr));
    
    int N = rand() % 61 + 120;
    
    for (int i = 0; i < N; i++) {
        int X = rand() % 9 + 1;
        char operators[] = {'+', '-', '*', '/'};
        char s = operators[rand() % 4];
        int Y = rand() % 9 + 1;
        
        cout << X << " " << s << " " << Y << endl;
        sleep(1);
    }

    cout << EOF;

    return 0;
}
