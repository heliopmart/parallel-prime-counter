#ifndef THREADS_H // "Include Guards" para evitar inclusão múltipla
#define THREADS_H

#include "default.h"

#include <vector>
#include <thread>
#include <numeric>
#include <atomic>
#include <windows.h>
#include <queue>
#include <cmath> 

int startThreads(Matrix* mat, int n_threads, int chunk_size);

DWORD WINAPI dynamic_worker(void* param);

#endif // THREADS_H