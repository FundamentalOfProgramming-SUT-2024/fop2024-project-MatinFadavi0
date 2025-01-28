#include "libs.h"
#include "funcs.h"

int str_to_num (char *str) {
    int num = 0;
    int p = strlen(str)-1;
    for(p; p>=0; p--) {
        num = num*10 + (str[strlen(str)-p-1]-'0');
    }
    
    return num;
}

void sort(int *arr, int scores[], int size) {
    for(int i=0; i<size-1; i++) {
        for(int j=i+1; j<size; j++) {
            if(scores[i] < scores[j]) {
                int temp = *(arr+i);
                *(arr+i) = *(arr+j);
                *(arr+j) = temp;
            }
        }
    }
}

int power(int num, int n) {
    if(n==0) {
        return 1;
    }
    if(n==1) {
        return num;
    }
    for(int i=2; i<=n; i++) {
        num *= num;
    }
    return num;
}

const char *txt_format(char username[]) {
    char *file_name = (char *) malloc(50*sizeof(char));
    strcpy(file_name,username);
    file_name[strlen(username)] = '.'; file_name[strlen(username)+1] = 't';
    file_name[strlen(username)+2] = 'x'; file_name[strlen(username)+3] = 't'; file_name[strlen(username)+4] = 0;
    return file_name;
}