#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#define PROGRESS_BAR_WIDTH 15

#define CLEARSCREEN() printf("\033[H\033[J")

int main() {
    struct sysinfo info;
    struct winsize w;
    
    while (1)
    {
        CLEARSCREEN();
        sysinfo(&info);
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        if (&info == NULL) {
            perror("Error getting system information");
            return 1;
        }

        long used_ram = info.totalram - info.freeram;
        double percent_used = (double)(used_ram) / info.totalram;

        printf("  ├─ Uptime: \033[1;32m%ld seconds\033[0m\n", info.uptime);
        printf("  ├─ Total RAM: \033[1;32m%.2f GB\033[0m\n", info.totalram / 1073741824.0 * info.mem_unit);
        printf("  ├─ Free RAM: \033[1;32m%.2f GB\033[0m\n", info.freeram / 1073741824.0 * info.mem_unit);
        printf("  └─ Used RAM: \033[1;32m%.2f GB\033[0m\n\n", (info.totalram - info.freeram) / 1073741824.0 * info.mem_unit);

        /* PROGRESS BAR */
        if (percent_used > 0.75) {
            printf("\033[31m");
            printf("  ! CRITICAL !\n");
        } else if (percent_used > 0.5) {
            printf("\033[33m");
            printf("  ! NORMAL !\n");
        } else {
            printf("\033[32m");
            printf("  ! OK !\n");
        }

        printf("  [");
        for (int i = 0; i < PROGRESS_BAR_WIDTH; i++)
            printf("%c", (double)(i) / PROGRESS_BAR_WIDTH <= percent_used ? '#' : ' ');
        
        printf("]\033[0m %.2f%%\n", percent_used*100);

        sleep(1);
    }

    return 0;
}