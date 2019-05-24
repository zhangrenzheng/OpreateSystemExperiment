#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

mode_t mode = 0664;

int main(int argc, char *argv[])
{
    int firstFile;
    int secondFile;
    char ch;
    firstFile = open(argv[1], O_RDONLY);
    secondFile = creat(argv[2], mode);
    open(argv[2], O_WRONLY);
    int temp;

    while(1)
    {
        temp = read(firstFile, &ch, sizeof(char));
        if(temp < 0){
            printf("Failed to read this file!\n");
            break;
        }
        if(temp == 0){
            break;
        }
        write(secondFile, &ch, sizeof(char));
    }

    close(firstFile);
    close(secondFile);
    return 0;
}
