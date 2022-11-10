#include "oct.h"

ap_manager_t* mo =  oct_manager_alloc();

int main () {
    int j = 0;
    for (int i = 0; i < 4; i++){
    	j = i + 1;
    }
    return 0;
}

