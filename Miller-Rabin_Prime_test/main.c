#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// quickPow
static long long quickPow(long long base, long long exponent, long long mod) {
    long long result = 1;
    base = base % mod;  // 在循环开始前进行一次取模

    while (exponent > 0) {
        // 如果 exponent 是奇数，将 base 乘到结果上
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        // 将 exponent 减半
        exponent = exponent >> 1;
        // base 自身平方
        base = (base * base) % mod;
    }

    return result;
}


// Miller-Rabin
bool millerRabin(int n) {
    srand((unsigned)time(NULL));
    int test_time = 8;
    if (n < 3 || n % 2 == 0) return n == 2;
    int u = n - 1, t = 0;
    while (u % 2 == 0) u /= 2, ++t;
    // test_time 为测试次数，建议设为不小于 8
    // 的整数以保证正确率，但也不宜过大，否则会影响效率
    for (int i = 0; i < test_time; ++i) {
        int a = rand() % (n - 2) + 2, v = quickPow(a, u, n);
        if (v == 1) continue;
        int s;
        for (s = 0; s < t; ++s) {
        if (v == n - 1) break;  // 得到平凡平方根 n-1，通过此轮测试
        v = (long long)v * v % n;
        }
        // 如果找到了非平凡平方根，则会由于无法提前 break; 而运行到 s == t
        // 如果 Fermat 素性测试无法通过，则一直运行到 s == t 前 v 都不会等于 -1
        if (s == t) return 0;
    }
    return 1;
}


int main(int argc, char* argv[]){

    int i = 13;
    i += 13;
    while (1) {
        if(millerRabin(i)) {
            break;
        }
        i++;
    }

    printf("%d",i);
    return 0;
}