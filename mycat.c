#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ReadFile(int fd)
{
    int temp;
    char ch;
    while(1)
    {
        temp = read(fd, &ch, sizeof(char));
        if(temp < 0){
            printf("Failed to read this file!\n");
            break;
        }
        if(temp == 0){
            break;
        }
        printf("%c", ch);
    }

}

int main(int argc, char *argv[])
{
    int fd;
    int counter = 1;
    while(counter < argc){
        fd = open(argv[counter], O_RDONLY);
        ReadFile(fd); 
        close(fd);
        counter ++;
    }
    return 0;
}
