#include <stdio.h>

int main(int argc, char** argv) {
    int myArray[] = {21, 5, 66, 14, 37};
    int i = 0;
    int result = 0;
    while (i < 5) {
        if (myArray[i] >= 30) {
            result = result + myArray[i];
        }
        i = i + 1;
    }
    printf("%d\n", result);
}