# include <stdio.h> // fur prinf()
# include <string.h> // fuer strcpy(), strlen(), strstr()
# include <stdlib.h> // fuer malloc(), free()

char * aufgabe1_int2str(int zahl){
    int length = (zahl <= 0) ? 1 : 0; 
    int temp = zahl; 
    while (temp != 0 ){
        length++; 
        temp /= 10; 
    }

    char *result = (char*) malloc((length + 1) * sizeof(char)); 
    if(result == null){
        return NULL; 
    }

    // 
    int written = sprintf(result, "%d", zahl); 

    //
    if(written > length){
        free(result); 
        return NULL; 
    }
    return result; 
    
}

long aufgabe2_str2int(const char *str){
    char *endptr; 
    long value = strol(str, &endptr, 10); 
    
}