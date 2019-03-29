#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

char log_tmp[2048];

void log_write(char *level, char *file, const char *func, int line, char *msg)
{
	struct timeb timebuffer;
	struct tm *now;
	time_t ltime;
	int milisec;
	ftime(&timebuffer);
	ltime = timebuffer.time;
	milisec = timebuffer.millitm;
	now = localtime(&ltime);

	char pwd[1024];
	char logpwd[1024];
	char logtext[1024];
	FILE *logfp = NULL;

	getcwd(pwd, 1024);
	sprintf(logpwd, "%s/logs/dht_temp_%d%02d%02d.log", pwd, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
	sprintf(logtext, "[%02d:%02d:%02d.%03d\t(%s:%s:%d)  (...%s)] > %s", now->tm_hour, now->tm_min, now->tm_sec, milisec, file, func, line, level, msg);
	logfp = fopen(logpwd, "a");
	fputs(logtext, logfp);
	fclose(logfp);
}

char *read_config(char *find)
{
	static char config[256];
	char pwd[1024];
	char *find_result;
	char *split_value;

	getcwd(pwd, 1024);
	strcat(pwd, "/config/dht_temp.config");

	FILE *fp = fopen(pwd, "r");
	if (fp == NULL)
	{
		log_write("ERROR", __FILE__, __func__, __LINE__, "Config File Not Open");
		return "ERROR";
	}

	while (!feof(fp))
	{
		fgets(config, sizeof(config), fp);
		config[strlen(config) - 1] = '\0';
		find_result = strstr(config, find);
		if (find_result != '\0')
		{
			sprintf(log_tmp, "Read Config File Full Text = %s\n", find_result);
			log_write("INFO", __FILE__, __func__, __LINE__, log_tmp);
			split_value = strtok(find_result, "=");
			split_value = strtok(NULL, " ");
			sprintf(log_tmp, "Full Text Result Split Value = %s\n", split_value);
			log_write("INFO", __FILE__, __func__, __LINE__, log_tmp);
			break;
		}
	}

	fclose(fp);
	return split_value;
}
