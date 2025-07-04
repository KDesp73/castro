#ifndef BENCH_H
#define BENCH_H
#include <stdio.h>
#include <time.h>

#define BENCH_VERSION_MAJOR 0
#define BENCH_VERSION_MINOR 0
#define BENCH_VERSION_PATCH 1
#define BENCH_VERSION "0.0.1"

#ifndef BENCHAPI
    #define BENCHAPI static
#endif // BENCHAPI


typedef struct {
    float start;
    float end;
    FILE* logfile;
} bench_t;

#define BENCH_LOG_FILE "bench.log"
static bench_t bench = {0};

BENCHAPI inline void bench_init(bench_t* bench)
{
    bench->logfile  = fopen(BENCH_LOG_FILE, "a"); 
}

BENCHAPI inline void bench_close(bench_t* bench)
{
    fclose(bench->logfile);
}

BENCHAPI void bench_start(bench_t* bench)
{
    bench->start = (float)clock()/CLOCKS_PER_SEC;
    bench_init(bench);
}
#define BENCH_START() bench_start(&bench)

BENCHAPI void bench_end(bench_t* bench)
{
    bench->end = (float)clock()/CLOCKS_PER_SEC;
}
#define BENCH_END() bench_end(&bench)

BENCHAPI inline float bench_elapsed(const bench_t* bench)
{
    return bench->end - bench->start;
}

BENCHAPI inline void bench_log(bench_t* bench, const char* tag)
{
    time_t now;
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    char timestamp[74];
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    if(tag != NULL)
        fprintf(bench->logfile, "[%s %s]", timestamp, tag);
    else 
        fprintf(bench->logfile, "[%s]", timestamp);
    fprintf(bench->logfile, " Time elapsed: %lf seconds\n", bench_elapsed(bench));
    bench_close(bench);
}
#define BENCH_LOG(tag) bench_log(&bench, tag)

#endif // BENCH_H
