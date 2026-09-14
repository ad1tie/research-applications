#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void displaytime(int hrs, int min, int sec)
{
    printf("\rTime Elapsed -> %02d : %02d : %02d", hrs, min, sec);
    fflush(stdout);
}

int main()
{
    int choice;
    printf("What would you like to do?\n 1. Check current project progress\n 2. Activate a stopwatch for your work session\nChoice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input type.\n");
        return 1;
    }

    switch (choice)
    {
    case 1:
    {
        int total, status;
        printf("What is the total number of pages/slides/tasks left?: ");
        scanf("%d", &total);
        
        if (total <= 0) {
            printf("Total tasks must be greater than 0.\n");
            return 1;
        }

        int parts = total / 3;
        int rem = total % 3;
        int part1 = parts, part2 = parts, part3 = parts;
        if (rem >= 1) part1++;
        if (rem == 2) part2++;
        part2 += part1;
        part3 += part2;

        printf("How many tasks are you currently done with?: ");
        scanf("%d", &status);
        int perc = (status * 100) / total;
        int left = 100 - perc;

        if (status <= 0)
        {
            printf("Don't you think you should start?\n");
        }
        else if (0 < status && status <= part1)
        {
            printf("\033[0;31mYou're done with %d%% of the task!\nOnly %d%% is left!\033[0m\n", perc, left);
            printf("\033[0;31mSpeed up!!\033[0m\n");
        }
        else if (part1 < status && status <= part2)
        {
            printf("\033[1;33mYou're done with %d%% of the task!\nOnly %d%% is left!\033[0m\n", perc, left);
            printf("\033[1;33mCool! You're making good progress!\033[0m\n"); // Fixed the 'You're0' typo
        }
        else if (part2 < status && status < total)
        {
            printf("\033[0;32mYou're done with %d%% of the task!\nOnly %d%% is left!\033[0m\n", perc, left);
            printf("\033[0;32mAlmost at the finish line! Keep going!\033[0m\n");
        }
        else
        {
            char *colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
            int num_colors = 6;
            
            for (int loop = 0; loop < 5; loop++)
            {
                for (int i = 0; i < num_colors; i++)
                {
                    printf("\r%sYOU'RE DONE YAY!!!!\033[0m", colors[i]);
                    fflush(stdout);
                    usleep(300000);
                }
            }
            printf("\n");
            return 0;
        }
        break;
    }
    case 2:
    {
        int hour = 0;
        int minute = 0;
        int second = 0;
        printf("Stopwatch started. Press Ctrl+C to stop code execution.\n");
        while (1)
        {
            sleep(1);
            second++;
            if (second > 59)
            {
                minute++;
                second = 0;
            }
            if (minute > 59)
            {
                hour++;
                minute = 0;
                second = 0;
            }
            displaytime(hour, minute, second);
        }
        break;
    }
    default:
        printf("Invalid option selected.\n");
        break;
    }

    return 0;
}
