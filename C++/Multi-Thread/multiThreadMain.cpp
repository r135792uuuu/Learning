#include <stdio.h>
#include <pthread.h>
#include <iostream>
using namespace std;

// 多线程使用必须在CmakeLists中导入pthread库加入set编译
// 详情看CMakeLists

#define NUM_THREADS 4
#define ARRAY_SIZE 10000
int array[ARRAY_SIZE];
int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 线程执行函数，函数指针类型，因为需要随时响应；参数是void *类型
void* calculateSum(void* arg){
    // 线程函数的第一句通常是获取传入参数：Param tmp = *(Param *)arg;
    // 强行转换为原本的数据格式 例如Param是一种自定义的结构体
    int threadID = *((int*)arg);
    // 分成4份，根据传入的threadID计算对应区域的值
    int start = threadID * (ARRAY_SIZE / NUM_THREADS); 
    int end = start + (ARRAY_SIZE / NUM_THREADS);
    int localSum = 0;
    for(int i = start; i < end; ++i) {
        localSum += array[i];
    }
    // 拆分了四个线程，每个线程都在计算。
    // 所以这里使用互斥锁，防止每个线程都在计算不同sum导致互相覆盖。
    // 往往使用在多个线程中，彼此互相调用某个共同变量的情况下，防止冲突
    // 如果这个锁正在被其他线程调用，那么此时某个线程无法使用，进入阻塞状态
    pthread_mutex_lock(&mutex); // 拿到锁，下面开始操作  看情况决定要不要上锁
    sum += localSum;   // 在操作的过程中，上锁的线程其他线程不能操作
    pthread_mutex_unlock(&mutex);  // 解锁
    pthread_exit(NULL);
    return NULL;
}
int main(){
    pthread_t threads[NUM_THREADS];
    int threadIDs[NUM_THREADS];
    for(int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = i + 1;
    }
    for(int i = 0; i < NUM_THREADS; ++i) {
            threadIDs[i] = i;
            // 创建线程用create
            // 参数：线程id标识；线程属性一般都是NULL；
            // 线程的运行函数；传递到线程的参数（必须是void*类型的，要强转）
            int ret = pthread_create(&threads[i], NULL, calculateSum, (void*)&threadIDs[i]);
            if (ret != 0) {
                printf("线程创建失败");
                return 0;
            }
    }
    for(int i = 0; i < NUM_THREADS; ++i) {
            // 用于等待其他线程结束，相当于把main函数变成阻塞等待了
            // 如果注释掉的话，那就会直接return 0，不会等待线程计算
            // 所以需要加一个这个 等待子线程计算完成退出之后再进行下面
            pthread_join(threads[i], NULL);
        }
    pthread_mutex_destroy(&mutex); // 销毁锁
    printf("Sum of array elements: %d\n", sum);
    return 0;
}
