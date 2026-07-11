#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include <sys/sysinfo.h>

#define PROGRESS_BAR_WIDTH 15

#ifdef _WIN32
#define CLEARSCREEN() system("cls")
#else
#define CLEARSCREEN() system("clear")
#endif

int main() {
    struct sysinfo info;

    while (1)
    {
        sysinfo(&info);

        if (&info == NULL) {
            perror("Error getting system information");
            return 1;
        }

        long used_ram = info.totalram - info.freeram;
        double percent_used = (double)(used_ram) / info.totalram;

        printf("______________________\nUptime: %ld seconds\n", info.uptime);
        printf("Total RAM: %.2f GB\n", info.totalram / 1073741824.0);
        printf("Free RAM: %.2f GB\n", info.freeram / 1073741824.0);
        printf("Used RAM: %.2f GB\n______________________\n", (info.totalram - info.freeram) / 1073741824.0);
        
        /* PROGRESS BAR */
        printf("RAM USAGE\n[");
        for (int i = 0; i < PROGRESS_BAR_WIDTH; i++)
            printf("%c", (double)(i) / PROGRESS_BAR_WIDTH <= percent_used ? '#' : ' ');

        printf("] %.2f%%\n", percent_used*100);
        
        CLEARSCREEN();
    }

    return 0;
}