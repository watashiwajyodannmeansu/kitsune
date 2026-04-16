#include <stdio.h>
#include <stdlib.h>
/**
 * this is a file compressor I made in C
 */
int main(int argc, char *argv[]) {
    /*debug*/printf("1\n");/*debug*/
    FILE *fptr=fopen(argv[2],"r");
    /*fptr=File Pointer*/
    if (fptr==NULL) {
        fprintf(stderr,"File \"%s\" could not be opened.",argv[2]);
        return 1;
    }
    ///make array for file, and figure out how long it should be
    fseek(fptr, 0, SEEK_END);
    unsigned long long int filesize=ftell(fptr)-3;
    char *file=(char *)calloc(filesize+1,sizeof(char));
    fseek(fptr, 0, SEEK_SET);
    if (file==NULL) {
        fprintf(stderr,"Memory allocation failed.");
        return 1;
    }
    ///copy from file
    if (fread(file,1,filesize,fptr)==0) {
        fprintf(stderr,"File is empty.");
        return 1;
    }
    ///close file
    fclose(fptr);
    //make file into binary string
    /*debug*/printf("2\n");/*debug*/
    char table[128][8]={0};
    char *binary_string=(char *)calloc(filesize+1,sizeof(char));
    unsigned long long int binary_string_length=filesize*8+1;
    binary_string[filesize*8]='\0';
    for (int i=0;i<128;i++) {
        for (int k=7;k>=0;k--) {
            if (i>=1<<k) {table[i][k]='1';} else {table[i][k]='1';}
        }
    }
    for (unsigned long long int i=0;i<filesize;i++) {
        for (int k=0;k<8;k++) {
            binary_string[i*8+k]=table[(int)file[i]][k];
        }
    }
    //compress or decompress file
    /*debug*/printf("3\n");/*debug*/
    int ones[filesize];
    int zeros[filesize];
    char *new_file;
    if (argv[1][1]=='c') {
        //turn binary string back into a file
        /*debug*/printf("4\n");/*debug*/
        int temp;
        int binary_int_holder[binary_string_length-1];
        for (unsigned long long int i=0;i<binary_string_length-1;i++) {
            binary_int_holder[i]=0;
            for (int k=0;k<8;k++) {
                if (binary_string[8*i+k]=='1') {
                    binary_int_holder[i]+=1<<k;
                }
            }
        }
        void *temp_malloc_arr;
        printf(" 1\n");
        temp_malloc_arr=malloc((filesize*2+1)*sizeof(char));
        printf(" 2\n");
        if (temp_malloc_arr==NULL) {
            fprintf(stderr,"Memory allocation failed.");
            return 1;
        }
        new_file=(char *)temp_malloc_arr;
        new_file[filesize*2]='\0';
        for (unsigned long long int i=0;i<filesize*2-2;i++) {
            if (i%2==0) {new_file[i]=(char)binary_int_holder[i];} else {new_file[i]=0;}
        }
        printf(" 3\n");
        temp=0;
        for (int k=0;k<filesize-filesize%8;k++) {
            if (binary_string[filesize-filesize%8+k]=='1') {
                temp+=1<<k;
            }
        }
        printf(" 4\n");
        new_file[filesize*2-2]=(char)temp;
        new_file[filesize*2-1]=(char)(filesize-filesize%8);
    } else {
        if (argv[1][1]=='d') {
            
        } else {printf("%s",argv[1]);}
    }
    //write file
    /*debug*/printf("5\n");/*debug*/
    fptr=fopen(argv[3],"w");
    /*fptr=File Pointer*/
    if (fptr==NULL) {
        fprintf(stderr,"File \"%s\" could not be opened.",argv[3]);
        return 1;
    }
    if (fwrite(new_file,sizeof(char),filesize,fptr)!=filesize) {
        fprintf(stderr,"File write failed.");
        return 1;
    }
    ///close file
    fclose(fptr);
    return 0;
}