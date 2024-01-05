#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>
#include<getopt.h>
#include<stdint.h>
#define NUM 100000
void create(char *inputFile, char *outputFile);
void extract(char *filename);
void tMethod(char *filename);
char const ** Map;
int header = 0;

int main(int argc, char *argv[])
{
    opterr = 0;
    char fstore[NUM];
    char cstore[NUM];
    char xstore[NUM];
    char tstore[NUM];
    int flag_cxt = 0, flag_f = 0, flag_ftar = 0;
    int c;
    while((c = getopt(argc, argv, "cxtf:")) != -1){
        if(optarg == NULL && c == ':'){
            fprintf(stderr, "Error: No tarfile specified.\n");
            exit(1);
        }
        switch(c)
        {
        case '?':{
            fprintf(stderr, "Error: No tarfile specified.\n");
            exit(1);
            break;
        }
        case 'c':{
            flag_cxt += 1;
            //you can use a strcpy to store the value of c somewhere. This is to call c in another method below. 
            strcpy(cstore, "c");
            break;

        }
        case 'x':{
            flag_cxt += 1;
            strcpy(xstore, "x");
            break;
        }
        case 't':{
            flag_cxt += 1;
            strcpy(tstore, "t");
            break;
        }
        case 'f':{
                if(flag_f == 1){
                    fprintf(stderr, "Error: No mode specified\n");
                    exit(1);
                }
            flag_f += 1;
            strcpy(fstore, "f");
            break;
        }
    }

    if(flag_cxt > 1){
            fprintf(stderr, "Error: Multiple modes specified.\n");
            exit(1);
        }

    }
    if(flag_f == 0){
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(1);

    }
    if(flag_cxt == 0){
        fprintf(stderr, "Error: No mode specified.\n");
        exit(1);
    }

    if((strcmp(xstore, "x") == 0 || strcmp(tstore, "t") == 0)){
            struct stat freadd;
            if(stat(argv[optind-1], &freadd) == -1){
                fprintf(stderr, "Error: Specified target(“%s”) does not exist\n", argv[optind-1]);
                exit(1);
            }
            if(stat(argv[optind-1], &freadd) == 0){
                FILE *fd = fopen(argv[optind-1], "r");
                if(fd != NULL){
                    int c;
                    fread(&c, 4, 1, fd);
                    fclose(fd);
                    if(S_ISDIR(freadd.st_mode)){
                        perror("fopen");
                        exit(1);
                    }
                    if(c != 0x7261746D){
                        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n", c, 0x7261746D);
                    }
                        
                }
            }
        }

    if(strcmp(cstore, "c") == 0){
        if(argv[optind] == NULL){
            fprintf(stderr, "Error: No directory target specified.\n");
            exit(1);
        }
        struct stat sBuf;
        stat(argv[optind], &sBuf);
        if(stat(argv[optind], &sBuf) == -1){
            fprintf(stderr, "Error: Specified target(“%s”) does not exist.\n", argv[optind]);
            exit(1);
        }
        if(!S_ISDIR(sBuf.st_mode)){
            fprintf(stderr, "Error: Specified target(“%s”) is not a directory.\n", argv[optind]);
            exit(1);
        }
    }
    if(strcmp(tstore, "t") == 0){
        tMethod(argv[optind-1]);
    }else if(strcmp(cstore, "c") == 0){
        create(argv[optind], argv[optind - 1]);
    }else if(strcmp(xstore, "x") == 0){
        extract(argv[optind-1]);
    }

}



void extract(char *filename){
    FILE *fd = fopen(filename, "r");
    int magicNum;
    long long iNode;
    int fileLength;
    char name[NUM];
    char content[NUM];
    mode_t mode;
    long long modTime;
    long long size;
    fread(&magicNum, 4, 1, fd);
    while(feof(fd) == 0){
        fread(&iNode, 8, 1, fd);//Reading general information for everything
        fread(&fileLength, 4, 1, fd);
        fread(&name, fileLength, 1, fd);
        fread(&mode, 4, 1, fd);
        if(S_ISDIR(mode)){
            fread(&modTime, 8, 1, fd);
            mkdir(name, mode);
            for(int i = 0; i < NUM; i++){
                name[i] = 0;
            }
            int status = mkdir(name, mode);
            
        }else if(S_ISREG(mode)){
            fread(&modTime, 8, 1, fd);
            FILE *fd2 = fopen(name, "w");
            fread(&size, 8, 1, fd);
            fread(&content, size, 1, fd);
            fwrite(&content, size, 1, fd2);
            for(int i = 0; i < NUM; i++){
                name[i] = 0;
            }
            for(int i = 0; i < NUM; i++){//clears the content of the file. 
                content[i] = 0;
            }
            fclose(fd2);
        }else if(S_ISLNK(mode)){
            symlink(name, content);
        }
        
        if(feof(fd) == 1){
            break;
        }
    }
    fclose(fd);

    
}


void create(char *inputFile, char *outputFile){
    DIR *dir = opendir(inputFile);//opens a directory called dir
    FILE *fd = fopen(inputFile, "r");
    struct dirent *entry;//creates a pointer for the entry dirent structure. 
    while((entry = readdir(dir)) != NULL){

    
    if(header == 1){
        FILE *fd2 = fopen(outputFile, "a");
        if (dir == NULL) {
        perror("opendir failed");
        return;
    }

    
    
    struct stat fileStat;//creates a structure called fileStat
    char fullPath[1024];
        
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
    }
    sprintf(fullPath, "%s/%s", inputFile, entry->d_name);//stores the value of fullpath

    if (stat(fullPath, &fileStat) < 0) {
        perror("stat failed");
        continue;
    }

        
        
        


    if (S_ISDIR(fileStat.st_mode)) {
        // Ignore current and parent directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char name[NUM];
        long iNode = fileStat.st_ino;
        int length = strlen(fullPath);
        strcpy(name, fullPath);
        int mode = fileStat.st_mode;
        long modTime = fileStat.st_mtime;
        fwrite(&iNode, 8, 1, fd2);
        fwrite(&length, 4, 1, fd2);
        fwrite(&mode, 4, 1, fd2);
        fwrite(&modTime, 8, 1, fd2);
        create(fullPath, outputFile);
        }
        else {
            int mode = fileStat.st_mode;
            long iNode = fileStat.st_ino;
            int length = strlen(fullPath) + 1;
            fwrite(&iNode, 8, 1, fd2);
            fwrite(&length, 4, 1, fd2);
            fwrite(&fullPath, length, 1, fd2);

        }
    fclose(fd2);
    closedir(dir);
    }
    if(header == 0){//Global variable set to 0. 
        FILE *fd2 = fopen(outputFile, "w");//opens a file to write into (the output file)
        int magicNum = 0x7261746D;
        fwrite(&magicNum, 4, 1, fd2);
        struct stat direcStat;
        stat(inputFile, &direcStat);
        time_t mtime = direcStat.st_mtime;//Stores the modification time in the struct direcStat. 
        int mode = direcStat.st_mode;
        long iNode = direcStat.st_ino;
        int length = strlen(inputFile);
        char name[100];
        strcpy(name, inputFile);
        fwrite(&iNode, 8, 1, fd2);
        fwrite(&length, 4, 1, fd2);
        fwrite(&name, length, 1, fd2);
        fwrite(&mode, 4, 1, fd2);
        fwrite(&mtime, 8, 1, fd2);
        header = 1;//Makes the boolean global variable 1 so that it doesn't print the magic number and the main directory details again
        fclose(fd2);
    }
    
    
}
}

void tMethod(char *filename){
    FILE *fd = fopen(filename, "r");//reads the filename. 
    int magicNum;
    long long iNode;
    int fileLength;
    char name[NUM];
    char content[NUM];
    int mode;
    long long modTime;
    long long size;
    fread(&magicNum, 4, 1, fd);
    while(feof(fd) == 0){
        fread(&iNode, 8, 1, fd);
        fread(&fileLength, 4, 1, fd);
        fread(&name, fileLength, 1, fd);
        fread(&mode, 4, 1, fd);
        fread(&modTime, 8, 1, fd);
        char sTring[10];
        char sTring2[4];
        sprintf(sTring, "%o", mode);
        int len = strlen(sTring);
        for(int i = 0; i < 3; i++){//Stores the last 3 digits in string2.
            sTring2[i] = sTring[len -3 + i];
        }
        sTring2[3] = '\0';
        int mode2 = atoi(sTring2);
        if(feof(fd) == 1){
            break;
        }
        if(S_ISREG(mode)){
            fread(&size, 8, 1, fd);
            fread(&content, size, 1, fd);
            if((mode & S_IXUSR)){//condition to see if it is an executable file. 
                printf("Mode 2 is: %s\n", sTring2);
                printf("%s* inode: %llu, mode: %d, mtime: %llu, size: %llu\n", name, iNode, mode2, modTime, size);
                for(int i = 0; i < NUM; i++){
                    name[i] = 0;
                }
            }else{
                printf("%s inode: %llu, mode: %d, mtime: %llu, size: %llu\n", name, iNode, mode2, modTime, size);
                for(int i = 0; i < NUM; i++){
                    name[i] = 0;
                }
            }
        }else if(S_ISDIR(mode)){
            printf("%s/ inode: %llu, mode: %d, mtime: %llu\n", name, iNode, mode2, modTime);
            for(int i = 0; i < NUM; i++){//Clears the buffer to use it again. 
                name[i] = 0;
            }
        }else if(S_ISLNK(mode)){
            printf("%s/ inode: %llu\n", name, iNode);
            for(int i = 0; i < NUM; i++){
                name[i] = 0;
            }
        }
    }
    fclose(fd);

}


