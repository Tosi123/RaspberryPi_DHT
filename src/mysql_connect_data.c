#include "/usr/include/mysql/mysql.h"
#include <errno.h>
#include <sys/timeb.h>

char db_name[256];
char db_table[256];
char db_ip[256];
char db_user[256];
char db_passwd[256];
int db_port;

int db_data_isnert(float temp_tot, float hum_tot, float temp_st, float hum_st, int count)
{
	//쿼리 변수
	struct timeb timebuffer;
	struct tm *now;
	time_t ltime;
	ftime(&timebuffer);
	ltime = timebuffer.time;
	now = localtime(&ltime);
	char ins_query[2048];
	int ins_query_stat;

	static MYSQL *connection = NULL;
	MYSQL conn;

	//초기화
	if (mysql_init(&conn) == NULL)
	{
		log_write("ERROR", __FILE__, __func__, __LINE__, "Mysql Init Fail\n");
		return -1;
	}
	else {
		log_write("INFO", __FILE__, __func__, __LINE__, "Mysql Init Success\n");
	}

	//연결
	connection = mysql_real_connect(&conn, db_ip, db_user, db_passwd, db_name, db_port, (char *)NULL, 0);

	if (connection == NULL)
	{
		sprintf(log_tmp, "Mysql Connection Fail [No=%d,Detail=%s] (dbname=%s,ip=%s,user=%s,passwd=%s,port=%d)\n", mysql_errno(&conn), mysql_error(&conn), db_name, db_ip, db_user, db_passwd, db_port);
		log_write("ERROR", __FILE__, __func__, __LINE__, log_tmp);
		return -1;
	}
	else
	{
		log_write("INFO", __FILE__, __func__, __LINE__, "Mysql Connection Sucess\n");
	}

	sprintf(ins_query, "INSERT INTO `%s`(`DATE`, `TIME`, `TEMPERATURE_TOTAL`, `HUMIDITY_TOTAL`, `TEMPERATURE_AVERAGE`, `HUMIDITY_AVERAGE`, `DATA_COUNT`) VALUES ('%d%02d%02d', '%02d%02d', '%.1f', '%.1f', '%.1f', '%.1f', '%d')",
		db_table, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, temp_tot, hum_tot, temp_st, hum_st, count);
	ins_query_stat = mysql_query(connection, ins_query);

	if (ins_query_stat != 0)
	{
		sprintf(log_tmp, "Mysql Insert Fail [No=%d,Detail=%s]\n", mysql_errno(&conn), mysql_error(&conn));
		log_write("ERROR", __FILE__, __func__, __LINE__, log_tmp);
		mysql_close(connection);
		log_write("INFO", __FILE__, __func__, __LINE__, "Mysql Connection Close Success\n");;
		return -1;
	}
	else
	{
		log_write("INFO", __FILE__, __func__, __LINE__, "Mysql Insert Success\n");
	};

	mysql_close(connection);
	log_write("INFO", __FILE__, __func__, __LINE__, "Mysql Connection Close Success\n");
	return 0;
}
