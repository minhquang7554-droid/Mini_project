#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define DEFAULT_NUM_SENSORS 1
#define DEFAULT_SAMPLING_TIME 60
#define DEFAULT_INTERVAL 24

#define MIN_SAMPLING_TIME 10
#define MIN_INTERVAL 1

typedef struct{
    int id;
    char timeStamp[25];
    float value;
} SensorData;

void writeLog(const char *message)
{
    FILE *logFile = fopen("task1.log", "a");
    if(logFile != NULL)
    {
        fprintf(logFile, "%s\n", message);
        fclose(logFile);
    }
}

float generateDustValue(void)
{
    return (float)(rand() % 8001) / 10.0f;
}

void formatTime(time_t t, char *buffer)
{
    struct tm *timeInfo;
    timeInfo = localtime(&t);
    strftime(buffer, 25,"%Y:%m:%d %H:%M:%S", timeInfo);
}

int parseArguments(int argc, char *argv[], int *n, int *st, int *si)
{
    int i;
    int hasN = 0, hasSt = 0, hasSi = 0;
    char *endptr;

    *n = DEFAULT_NUM_SENSORS;
    *st = DEFAULT_SAMPLING_TIME;
    *si = DEFAULT_INTERVAL;

    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-n") == 0)
        {
            if(hasN || i + 1 >= argc)
            {
                writeLog("Error 01: invalid command");
                printf("Erorr 01: invalid command\n");
                return 0;
            }
            hasN =1;
            *n = (int)strtol(argv[++i], &endptr, 10);
            if (*endptr != '\0')
            {
                writeLog("Error 02: invalid argument");
                printf("Error 02: invalid argument\n");
                return 0;
            }
        }

        else if(strcmp(argv[i], "-st") == 0)
        {
            if(hasSt || i + 1 >= argc)
            {
                writeLog("Error 01: invalid command");
                printf("Error 01: invalid command\n ");
                return 0;
            }
            hasSt = 1;
            *st = (int)strtol(argv[++i], &endptr, 10);
             if (*endptr != '\0')
            {
                writeLog("Error 02: invalid arugment");
                printf("Error 02: invalid argument\n");
                return 0;
            }
        }

        else if(strcmp(argv[i], "-si") == 0)
        {
            if(hasSi || i + 1 >= argc)
            {
                writeLog("Error 02: invalid command");
                printf("Error 01: invalid command\n");
                return 0;
            }
            hasSi = 1;
            *si = (int)strtol(argv[++i], &endptr, 10);
             if (*endptr != '\0')
            {
                writeLog("Error 02: invalid argument");
                printf("Error 02: invalid argument\n");
                return 0;
            }
        }
        else
        {
            writeLog("Error 01: invalid command");
            printf("Error 01: invalid command\n");
            return 0;
        }
    }

    if(*n <= 0 || *st < MIN_SAMPLING_TIME || *si < MIN_INTERVAL)
    {
        writeLog("Error 02: invalid argument");
        printf("Error 02: invalid argument\n");
        return 0;
    }

    return 1;

}

void generateCSV(int n, int st, int si)
{
    FILE *csvFile;

    csvFile = fopen("dust_sensor.csv", "w");

    if(csvFile == NULL)
    {
        writeLog("Error 03: dust_sensor.csv access denied");
        printf("Error 03: dust_sensor.csv access denied\n");
        return;
    }

    fprintf(csvFile, "id,time,value\n");

    time_t currentTime = time(NULL);
    long long totalSeconds = (long long)si * 3600;
    time_t startTime = currentTime - totalSeconds;

    SensorData currentRecord;

    for(long long sec = 0; sec <= totalSeconds; sec += st)
    {
        time_t sampleTime = startTime + sec;

        formatTime(sampleTime, currentRecord.timeStamp);

        for(int id = 1; id <= n; id++)
        {
            currentRecord.id = id;
            currentRecord.value = generateDustValue();

            fprintf(csvFile, "%d,%s,%.1f\n", currentRecord.id, currentRecord.timeStamp, currentRecord.value);
            
        }
    }

    fclose(csvFile);
}

int main(int argc, char *argv[])
{
    int n;
    int st;
    int si;

    srand((unsigned int)time(NULL));

    if(!parseArguments(argc, argv, &n, &st, &si))
    {
        return 1;
    }

    generateCSV(n, st, si);
    return 0;
}