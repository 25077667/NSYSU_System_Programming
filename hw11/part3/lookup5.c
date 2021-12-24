/*
 * lookup5 : local file ; setup a memory map of the file
 *           Use bsearch. We assume that the words are already
 *           in dictionary order in a file of fixed-size records
 *           of type Dictrec
 *           The name of the file is what is passed as resource
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "dict.h"
#define N2_PAGE 2

static const Dictrec *dict_table;
static unsigned int dict_len = 1;

/*
 * This obscure looking function will be useful with bsearch
 * It compares a word with the word part of a Dictrec.
 *
 * # Well, reserve the origin cmp function. (SCC)
 */
static int dict_cmp(const void *a, const void *b)
{
    return strcmp((const char *) a, ((const Dictrec *) b)->word);
}

// Mmap file into memory is faster then often open and close O_RDONLY the file
// in disk, moreover the @res is immutable when this program is running.
static void init_dict_table(const char *const res)
{
    const int res_fd = open(res, O_RDWR);
    // At most #N2_PAGE page of dictionary
    const unsigned int SIZE__ = getpagesize() << N2_PAGE;
    EE(ftruncate(res_fd, SIZE__));
    EE(dict_table =
           (Dictrec *) mmap(NULL, SIZE__, PROT_READ, MAP_PRIVATE, res_fd, 0));
    close(res_fd);

    // Get the dict size
    for (int i = 0; dict_table[i].word[0]; i++)
        ++dict_len;
}

int my_connect(const char *res)
{
    init_dict_table(res);
    return (dict_len == 0) ? UNAVAIL : (int) dict_len;
}

// Default @sought is "Not found"
int lookup(Dictrec *sought, const char *resource)
{
    (void) resource;
    for (unsigned int i = 0; i < dict_len; i++) {
        const Dictrec *const cur = &(dict_table[i]);
        if (!dict_cmp(sought->word, cur)) {
            memcpy(sought->text, cur->text, sizeof(cur->text));
            return FOUND;
        }
    }
    return NOTFOUND;
}

int my_disconnect(void)
{
    const unsigned int SIZE__ = getpagesize() << N2_PAGE;
    return munmap((void *) dict_table, SIZE__);
}
