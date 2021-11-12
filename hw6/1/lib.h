#ifndef __LIB_H__
#define __LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

void add_watch_file(const char *path, int base, int wd);
void erase_watch_file(const char *path);


// Tool
const char *concat(const char *s1, const char *s2);
const char *wd2dir(int wd);


#ifdef __cplusplus
}
#endif


#endif