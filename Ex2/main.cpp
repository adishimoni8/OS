#include <iostream>
#include "uthreads.h"

using namespace std;
// Thread 1:

void func1() {
    cout << "Thread 1 start function..." << endl;
    cout << "Thread 1 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func2() {
    cout << "Thread 2 start function..." << endl;
    uthread_block(3);
    cout << "Thread 2 block thread 3" << endl;
    cout << "Thread 2 went to sleep" << endl;
    uthread_sleep(2);
    cout << "Thread 2 woke up" << endl;
    uthread_resume(3);
    cout << "Thread 2 released thread 3" << endl;
    uthread_block(2);
    cout << "Thread 2 blocked itself" << endl;
}

void func3() {
    cout << "Thread 3 start function..." << endl;
    uthread_terminate(2);
    cout << "Thread 3 terminated thread 2" << endl;
    cout << "Thread 3 is trying to block main" << endl;
    uthread_block(0);
    cout << "Thread 3 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func7() {
    cout << "Thread 7 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 1" << endl;
    cout << "Thread 7 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func8() {
    cout << "Thread 8 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 2" << endl;
    cout << "Thread 8 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func9() {
    cout << "Thread 9 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 4" << endl;
    cout << "Thread 9 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func4() {
    cout << "Thread 4 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 1" << endl;
    cout << "Thread 4 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func5() {
    cout << "Thread 5 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 2" << endl;
    cout << "Thread 5 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func6() {
    cout << "Thread 6 start function..." << endl;
    cout << "Good tid: " << uthread_get_tid() << " == 3" << endl;
    cout << "Thread 6 creating 7, 8 and 9" << endl;
    uthread_spawn(func7);
    uthread_spawn(func8);
    uthread_spawn(func9);
    cout << "Thread 6 terminating itself" << endl;
    uthread_terminate(uthread_get_tid());
}

void func10() {
    uthread_terminate(uthread_get_tid());
}

int main() {
    uthread_init(100);
    cout << "Main Thread has been created" << endl;

//    // Test 1:
//    uthread_spawn(func1);
//    cout << "1 Thread has been created" << endl;
//    uthread_spawn(func2);
//    cout << "2 Thread has been created" << endl;
//    uthread_spawn(func3);
//    cout << "3 Thread has been created" << endl;
//    for ( ;; ){}
//    return 0;

//    // Test 2:
//    uthread_spawn(func4);
//    cout << "4 Thread has been created" << endl;
//    uthread_spawn(func5);
//    cout << "5 Thread has been created" << endl;
//    uthread_spawn(func6);
//    cout << "6 Thread has been created" << endl;
//    for ( ;; ){}
//    return 0;

    // Test 3:
    for (int i = 0; i < MAX_THREAD_NUM + 100; ++i){
        if (uthread_spawn(func10)){
            cout << "created" << endl;
        }
        for (int j = 0; j <1000000; ++j){}
        cout << uthread_get_total_quantums() << endl;
    }

    return 0;
}