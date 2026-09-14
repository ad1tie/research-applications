#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
struct lyric_position{
    long current_time;
    char text[1000];
};
long gettimeinms(){
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
int main(){
    int mins,secs,hunds;
    char temp_text[1000];
    struct lyric_position lyricnow[1000];
    int total_lines = 0;
    char line_buffer[1000];
    FILE *lrcptr = fopen("TSWTNYtest.lrc","r");
    if(lrcptr==NULL){
        printf("doesnt exist\n");
        return 0;
    }
    while(fgets(line_buffer,sizeof(line_buffer),lrcptr)!=NULL){
        if (sscanf(line_buffer, "[%d:%d.%d] %[^\n]",
           &mins, &secs, &hunds, temp_text) == 4)
{
     long totalms = (long)mins * 60000L +
               (long)secs * 1000L +
               (long)hunds * 10L;

    lyricnow[total_lines].current_time = totalms;
    strcpy(lyricnow[total_lines].text, temp_text);
    total_lines++;
}
    }
    fclose(lrcptr);
    int activeindex = -1;
    
    long start_time = gettimeinms();
    while(activeindex<total_lines-1){
        int newindex = -1;
        long currtime = gettimeinms();
        long elapsed_time = currtime - start_time;
        for(int i = 0;i<total_lines;i++){
        if(lyricnow[i].current_time<=elapsed_time){
            newindex = i;
        }else{
            break;
        }
        }
        if(newindex!=activeindex){
            fflush(stdout);
            activeindex = newindex;
            for (int i = 0; i < total_lines; i++) {
    if (i == activeindex) {
        printf("\033[1;32m  > %s \033[0m\n", lyricnow[i].text);
    } 
}
        }
    
    }
    return 0;

    
}

