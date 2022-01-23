#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc , char *argv[]) {
        FILE *fout;
        fout = fopen(argv[1], "r");
        int n = atoi(argv[2]);

        if (!fout) {
                printf("Cannot open file\n");
                return 1;
        }

        if (n == 0) {
            n = INT_MAX;
        }

        char s;
        int count = 1;
        while ((s=getc(fout)) != EOF) {
            if ((count == n) && (s == '\n')) {
                count = 1;
                if (getc(stdin)) {
                    continue;
                }
            }
            if (count <= n) {
                printf("%c", s);
            }
            if ( s == '\n') {
                count++;
            }
        }

        if (fclose(fout) == -1) {
                printf("Cannot close error\n");
                return 1;
        }

        return 0;
}