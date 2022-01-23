#include <stdio.h>
#include <stdlib.h>

int main(int argc , char *argv[]) {
        FILE *fout;
        fout=fopen(argv[1], "r");
        FILE *fin;
        fin=fopen(argv[2], "wt");
        if (!fout || !fin) {
                printf("Cannot open file\n");
                return 1;
        }
        int s;
        while ((s=getc(fout)) != EOF) {
            fputc(s, fin);
        }

        if ((fclose(fout) == -1) || (fclose(fin) == -1)) {
                printf("Cannot close file\n");
                return 1;
        }
        
        printf("Success\n");
        return 0;
}