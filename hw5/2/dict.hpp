#ifndef __DICT_HPP__
#define __DICT_HPP__
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
void insert(int pid, FILE *fp);
void drop();
void erase(FILE *fp);
int get_pid(FILE *fp);
#ifdef __cplusplus
}
#endif

#endif