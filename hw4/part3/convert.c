/*
 * convert.c : take a file in the form
 *  word1
 *  multiline definition of word1
 *  stretching over several lines,
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv)
{
    FILE *in = NULL;
    FILE *out = NULL; /* defaults */
    char *line = NULL;

    /* If args are supplied, argv[1] is for input, argv[2] for output */
    if (argc == 3) {
        if ((in = fopen(argv[1], "r")) == NULL) {
            DIE(argv[1]);
        }
        if ((out = fopen(argv[2], "w")) == NULL) {
            DIE(argv[2]);
        }
    } else {
        printf("Usage: convert [input file] [output file].\n");
        return -1;
    }

    /* Main reading loop : read word first, then definition into dr */
    // I cannot understand your English, what are you writing about?

    /* Loop through the whole file. */
    while (!feof(in)) {
        /* Create and fill in a new blank record.
         * First get a word and put it in the word field, then get the
         * definition and put it in the text field at the right offset.  Pad the
         * unused chars in both fields with nulls.
         */

        /* Read word and put in record.  Truncate at the end of the "word"
         * field.
         *
         * Fill in code. */
        Dictrec rec = {.word = {0}, .text = {0}};
        size_t len = 0;
        Wunused(getline(&line, &len, in));
        if (strlen(line) == 0)
            continue;
        line[strlen(line) - 1] = ' ';
        strcpy(rec.word, line);

        /* Read definition, line by line, and put in record.
         *
         * Fill in code. */
        while (getline(&line, &len, in) != 1) {
            len = strlen(line);
            line[len - 1] = '\0';  // Replace '\n' into ' '
            strcat(rec.text, line);
        }
        /* Write record out to file.
         *
         * Fill in code. */
        fwrite(rec.word, 1, sizeof(rec.word), out);
        fwrite(rec.text, 1, sizeof(rec.text), out);
        fwrite("\n", 1, 1, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
