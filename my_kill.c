#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define FN_LEN 256
struct Options {
    bool using_h; // -h, hello world
    bool using_f; // -f, has a file optarg
    char filename[FN_LEN]; // -f optarg
};

static void init_opts(struct Options* opts) {
    opts->using_h = false;
    opts->using_f = false;
    for (int i = 0; i < FN_LEN; i++)
        opts->filename[i] = 0;
}

struct Options opts;

struct Options get_opts(int argc, char* argv[]) {
    init_opts(&opts);
    int opt;

    while ((opt = getopt(argc, argv, ":f:h")) != -1) {
        switch (opt) {
            case 'h': opts.using_h = true; break;
            case 'f': 
                opts.using_f = true; 
                strcpy(opts.filename, optarg); 
                break;
            case ':': 
                printf("-f needs a value\n");
                break;
            case '?': 
                printf("Unknown option\n");
                exit(-1);
        }
    }

    return opts;
}

int main(int argc, char *argv[]) {
    struct Options o = get_opts(argc, argv);
    if ((argc - optind) !=1) {
        printf("Error - command format is $ my_kill - options pid\n");
        exit(-1);
    }
    int pid = 0;
    pid = atoi(argv[argc-1]);
    if (opts.using_h) {
        printf("Hello World\n");
    }
    if (opts.using_f) {
        printf("Printing file: %s\n", opts.filename);
        FILE *ftp = fopen(opts.filename, "r");
        char c;
        if(ftp == NULL) {
            printf("Nope. Cannot open file \n");
            exit(0);
        }
        c = fgetc(ftp);
        while (c != EOF) {
            printf("%c", c);
            c = fgetc(ftp);
        }
        fclose(ftp);
        
    }
    printf("my_kill pid: %d\n", getpid()); //get my_kill's pid
    int status = kill(pid, SIGINT);
    int errnum = errno;
    if (status == -1) {
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error killing process: %s\n", strerror(errnum));
    
    }
    return 0;
}
