#include <stdio.h>
#include <stdlib.h>
#define max(a,b) (a>b)?a:b
#define min(a,b) (a>b)?b:a
/**
 * This is the Kitsune coding language compiler. v 0.1.2* It is written in c, but will be made compatible for modules/extensions in other languages.
 * Currently compatible with extensions .c, .kitsune, and .tenko.
 * 
*/
void raise_kitsune_error(char *ErrorType, char **FileLines, long long int line, long long int pos) {
    int line_number_size=snprintf(NULL,0,"%lld",line)+1;
    int pos_number_size=snprintf(NULL,0,"%lld",pos)+1;
    int enclosuresize=line_number_size+pos_number_size+21;
    for (int i=0;i<enclosuresize;i++) {fprintf(stderr,"-");}
    fprintf(stderr,"\n%s error at line %lld; %lld.\n",ErrorType,line,pos);
    for (int i=0;i<enclosuresize;i++) {fprintf(stderr,"-");}
    fprintf(stderr,"\n");
    return;
}
char **for_k(char **filelines, long long int *filelinelengths, long long int *depths) {
    return filelines;
}
char **while_k(char **filelines, long long int *filelinelengths, long long int *depths) {
    return filelines;
}
char **goto_k(char **filelines, long long int *filelinelengths, long long int *depths) {
    return filelines;
}
int main(int argc, char *argv[]) {
    //get argument options
    ///finding location of file in argv
    int FileLocation=0;
    for (int i=1;i<argc;i++) {
        if (argv[i][0]!='-') {
            FileLocation=i;
            break;
        }
    }
    if (FileLocation==0) {
        fprintf(stderr,"File not provided.");
        return 1;
    }
    ///defining default values
    int debug=0;
    int preprocessed=0;
    int ArgvCharSize;
    ///getting values
    for (long long int i=1;i<argc;i++) {
        ////if argument is not a flag, skip to next argument
        if (argv[i][0]!='-') {continue;}
        ArgvCharSize=sizeof(argv[i])/sizeof(char);
        for (long long int k=1;k<ArgvCharSize;k++) {
            ////if any char/string in the argument matches, change its constant
            if (argv[i][k]=='d') {debug=1;}
        }
    }
    //copy file from argument
    /*debug*/if (debug) {printf("1\n");}/*debug*/
    FILE *fptr=fopen(argv[FileLocation],"r");
    /*fptr=File Pointer*/
    if (fptr==NULL) {
        fprintf(stderr,"File \"%s\" could not be opened. Maybe it doesn't exist?",argv[FileLocation]);
        return 1;
    }
    ///make array for file, and figure out how long it should be
    fseek(fptr, 0, SEEK_END);
    char file[ftell(fptr)+1];
    fseek(fptr, 0, SEEK_SET);
    if (file==NULL) {
        fprintf(stderr,"Memory allocation failed.");
        return 1;
    }
    ///copy from file
    if (fread(file,1,sizeof(file),fptr)==0) {
        fprintf(stderr,"File is empty.");
        return 1;
    }
    ///safety \0
    file[sizeof(file)]='\0';
    fclose(fptr);
    //Kitsune prepreprocessor
    ///figure out how large the file is, and how many lines it should be split up into
    /*debug*/if (debug) {printf("2\n");}/*debug*/
    long long int FileSize=sizeof(file)/sizeof(file[0]);
    char LineSeperators[]="{\n;}";
    int LineSeperatorSize=sizeof(LineSeperators)/sizeof(LineSeperators[0]);
    long long int FileLineCount=0;
    ////figure out how many lines there should be, by checking over all characters if any are a line seperator
    for (int i=0;i<FileSize;i++) {
        for (int k=0;k<LineSeperatorSize;k++) {
            if (file[i]==LineSeperators[k]) {FileLineCount++;}
        }
    }
    ///split the file into lines, and figure out how long they are
    /*debug*/if (debug) {printf("3\n");}/*debug*/
    char **FileLines;
    ////allocate space for the filelines
    FileLines=(char **)calloc(FileLineCount,sizeof(char*));
    long long int temp;
    long long int k=0;
    /*k=address pointer to the file*/
    ////get the space for the lengths of the lines
    long long int *FileLineLengths=(long long int *)calloc(FileLineCount,sizeof(long long int));
    int temp_bool;
    for (long long int i=0;i<FileLineCount;i++) {
        temp=0;
        /*temp=length of line*/
        temp_bool=1;
        ////figure out how long a line is
        while (temp_bool&&k<FileSize) {
            /////check if the character at k is a line seperator
            for (long long int j=0;j<LineSeperatorSize;j++) {
                if (file[k]==LineSeperators[j]) {temp_bool=0; break;}
            }
            temp++;
            k++;
        }
        ////get the space for the line
        FileLines[i]=(char *)calloc(temp+1,sizeof(char));
        FileLines[i][temp]='\0';
        ////record the length of the line
        FileLineLengths[i]=temp+1;
        k-=temp;
        ////copy the data in a line to the FileLines section it should be in
        for (long long int j=0;j<temp;j++) {
            FileLines[i][j]=file[k+j];
        }
        k+=temp;
    }
    ///cleaning lines of prefixxed 0 values
    /*debug*/if (debug) {
        for (int i=0;i<FileLineCount;i++) {
            printf("Line number %d:",i);
            for (int k=0;k<FileLineLengths[i];k++) {
                printf("%d:%d ",k,FileLines[i][k]);
            }
            printf("\n");
        }
    }/*debug*/
    char **char_temp_ptr_ptr;
    /*char_temp_ptr_ptr=filelines*/
    temp=FileLineCount;
    /*temp=old filelinecount*/
    ////updating filelinecount
    for (long long int i=0;i<temp;i++) {
        if ((int)FileLines[i][0]==0) {FileLineCount--;}
    }
    ////getting new space for filelines
    char_temp_ptr_ptr=(char **)calloc(FileLineCount,sizeof(char *));
    temp=0;
    /*temp=position offset*/
    ////adding proper lines
    for (long long int i=0;i<FileLineCount;i++) {
        while ((int)FileLines[i+temp][0]==0) {temp++;}
        char_temp_ptr_ptr[i]=FileLines[i+temp];
    }
    ////replacing filelines with updated filelines in char_temp_ptr_ptr (remember to be memory safe, kids)
    free(FileLines);
    FileLines=char_temp_ptr_ptr;
    /*debug*/if (debug) {
        for (int i=0;i<FileLineCount;i++) {
            printf("Line number %d:",i);
            for (int k=0;k<FileLineLengths[i];k++) {
                printf("%d ",FileLines[i][k]);
            }
            printf("\n");
        }
    }/*debug*/
    ///Clean the lines of unneccicary junk, such as prefixed spaces, newlines, tabs, or semicolons
    if (debug) {printf("4\n");}
    char *char_temp_arr;
    char UnwantedCharacters[]="\n\t";
    int UnwantedCharactersSize=sizeof(UnwantedCharacters)/sizeof(char);
    char UnwantedCharactersBack[]="\n; ";
    int UnwantedCharactersBackSize=sizeof(UnwantedCharactersBack)/sizeof(char);
    char UnwantedCharactersFront[]=" ";
    int UnwantedCharactersFrontSize=sizeof(UnwantedCharactersFront)/sizeof(char);
    ////check which size is largest, for max efficiency
    int MaxUnwantedCharactersSize=max(UnwantedCharactersSize,max(UnwantedCharactersFrontSize,UnwantedCharactersBackSize));
    long long int *int_temp_arr;
    for (long long int i=0;i<FileLineCount;i++) {
        /*debug*/if (debug) {printf("line %lld:\"%s\"\n",i,FileLines[i]);}/*debug*/
        ////allocate space to file line adresses
        int_temp_arr=(long long int *)calloc(FileLineLengths[i],sizeof(long long int));
        /*int_temp_arr=updated file line addresses*/
        ////write 1 to 1 mapping to file line addresses
        for (long long int k=0;k<FileLineLengths[i];k++) {
            int_temp_arr[k]=k;
        }
        ////for all unwanted characters, set all bad addresses to -1
        for (int k=0;k<MaxUnwantedCharactersSize;k++) {
            /////if k is a good address, check with its value
            if (k<UnwantedCharactersFrontSize) {
                temp=0;
                /*temp=targeted address in the fileline*/
                /////set bad addresses to -1
                while (UnwantedCharactersFront[k]==FileLines[i][temp]&&k<FileLineLengths[i]) {int_temp_arr[temp]=-1; temp++;}
            }
            if (k<UnwantedCharactersBackSize) {
                temp=FileLineLengths[i]-1;
                /////set bad addresses to -1
                while (UnwantedCharactersBack[k]==FileLines[i][temp]&&k<FileLineLengths[i]) {int_temp_arr[temp]=-1; temp--;}
            }
            if (k<UnwantedCharactersSize) {
                temp=0;
                /////set bad addresses to -1
                for (long long int j=0;j<FileLineLengths[i];j++) {
                    if (UnwantedCharacters[k]==FileLines[i][j]) {
                        int_temp_arr[j]=-1; temp++;
                    }
                }
            }
        }
        for (int k;1;k++) {
                if (int_temp_arr[k]!=-1) {break;}
                if (k==FileLineLengths[i]-1) {
                        FileLines[i]=(char *)calloc(1,sizeof(char));
                        FileLines[i][0]='\0';
                        goto next_pre_pre_processor;
                }
        }
        /////change filelinelengths
        temp=FileLineLengths[i];
        FileLineLengths[i]=0;
        for (long long int k=0;k<temp;k++) {
            if (int_temp_arr[k]==-1) {continue;}
            FileLineLengths[i]++;
        }
        /////write new fileline
        temp=0;
        /*temp=-address offset*/
        printf("%d\n",FileLineLengths[i]);
        char_temp_arr=(char *)calloc(FileLineLengths[i]+1,sizeof(char));
        /*char_temp_arr=new fileline*/
        for (long long int k=0;k<FileLineLengths[i]+temp;k++) {
            //////if char is bad, increase the negative offset, and skip to the next address
            if (int_temp_arr[k]==-1) {
                temp++;
                continue;
            }
            //////write to fileline address
            char_temp_arr[k-temp]=FileLines[i][int_temp_arr[k]];
        }
        /////char safety \0
        char_temp_arr[FileLineLengths[i]]='\0';
        /////writing fileline, memory safe
        free(FileLines[i]);
        FileLines[i]=char_temp_arr;
        next_pre_pre_processor:
        /*debug*/if (debug) {printf("line %lld:\"%s\"\n",i,FileLines[i]);}/*debug*/
    }
    ///Cleaning lines array of code that do nothing
    /*debug*/if (debug) {printf("5\n");}/*debug*/
    temp=0;
    /*temp=-address offset*/
    ////free the addresses that are empty
    for (long long int i=0;i<FileLineCount-temp;i++) {
        /////if fileline is not empty, skip, else free the line and increase the -offset
        if (FileLines[i-temp]!="\0") {continue;}
        free(FileLines[i-temp]);
        FileLines[i-temp]=NULL;
        temp++;
    }
    long long int NonEmptyItems=0;
    ////allocate memory to new filelines
    char_temp_ptr_ptr=(char **)calloc(FileLineCount-temp,sizeof(char *));
    ////copy lines to new filelines and count them
    temp=0;
    /*temp=address of new filelines*/
    for (long long int i=0;i<FileLineCount;i++) {
        if (FileLines[i]==NULL||(int)FileLines[i][0]==0) {continue;}
        printf("line %lld, char 0:%d\n",i,FileLines[i][0]);
        char_temp_ptr_ptr[temp]=FileLines[i];
        NonEmptyItems++;
        temp++;
    }
    FileLineCount=NonEmptyItems;
    ////assign filelines, memory safe
    free(FileLines);
    FileLines=char_temp_ptr_ptr;
    ////allocate memory space for file line lengths
    int_temp_arr=(long long int *)calloc(NonEmptyItems,sizeof(long long int));
    /*int_temp_arr=new File Line Lengths*/
    temp=0;
    /*temp=address of new file line lengths*/
    ////store all useful file lengths
    for (long long int i=0;i<FileLineCount;i++) {
        /////if fileline is empty, do not store, else, store
        if (FileLines[i][0]=='\0'||FileLines[i][0]=='\1'||FileLineLengths[i]==0) {continue;}
        int_temp_arr[temp]=FileLineLengths[i];
        temp++;
    }
    ////assign file line lengths, memory safe
    free(FileLineLengths);
    FileLineLengths=int_temp_arr;
    FileLineCount=NonEmptyItems;
    //Kitsune preprocessor
    ///if we don't need to do this, skip
    if (preprocessed) {goto processor;}
    /*debug*/if (debug) {printf("6\n");}/*debug*/
    /*debug*/if (debug) {
        for (int i=0;i<FileLineCount;i++) {
            for (int k=0;k<FileLineLengths[i];k++) {printf("line %lld, char %d: %c,%d\n",i,k,FileLines[i][k],FileLines[i][k]);}
        }
        for (int i=0;i<FileLineCount;i++) {
            printf("line %lld: %s\n",i,FileLines[i]);
        }
    }/*debug*/
    long long int *LineDepths;
    LineDepths=(long long int *)calloc(FileLineCount,sizeof(long long int));
    LineDepths[0]=0;
    temp=0;
    for (long long int i=0;i<FileLineCount;i++) {
        for (long long int k=0;k<FileLineLengths[i];k++) {
            if (FileLines[i][k]=='}'&&k>0) {
                raise_kitsune_error("syntax",FileLines,i,k);
                temp++;
            }
            if (FileLines[i][k]=='{'&&k<FileLineLengths[i]-1) {
                raise_kitsune_error("syntax",FileLines,i,k);
                temp++;
            }
        }
        if (FileLines[i][0]=='}') {LineDepths[i]=1+LineDepths[i-1]; continue;}
        if (FileLines[i][FileLineLengths[i]-1]=='{') {LineDepths[i]=LineDepths[i-1]-1; continue;}
        LineDepths[i]=LineDepths[i-1];
    }
    if (temp!=0) {
        fprintf(stderr, "Could not compile file, from %lld syntax errors", temp);
        return temp;
    }
    ///defining jump functions
    int jmpSize=3;
    ///Define your jump functions here
    char **(*jmp[3])(char **,long long int *, long long int *)={for_k, while_k, goto_k};
    char *jmpnames[3]={"for (#var;#condition;#iterator) {#code}","while (#condition) {#code}","goto #label;"};
    ///end defining jump functions
    //Kitsune processor
    processor:
    /*debug*/if (debug) {printf("7\n");}/*debug*/


    //goto label to safely exit
    exit:
    printf("end\n");
    return 0;
}