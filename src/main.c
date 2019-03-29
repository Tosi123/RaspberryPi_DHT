#include "dht_temp_data.h"
#include "db_manager.h"

int main()
{
	//config���� ���� �ʱ� ������ Ȯ��
	if (wiringPiSetup() == -1)
	{
		printf("Setup Error Not Started");
		exit(1);
	}
	else
	{
		sprintf(log_tmp, "=======================START=======================\n");
		log_write("INFO", __FILE__, __func__, __LINE__, log_tmp);
	}
	//��������� ����
	uint8_t pin_num = atoi(read_config("@DHT_PIN_NUMBER"));
	int cycle_time = atoi(read_config("@DHT_CYCLE_TIME"));
	int divide_num = atoi(read_config("@DHT_DIVIDE_COUNT"));
	int if_time = cycle_time * divide_num;
	
	//config�� �ּҰ� Ȯ��
	if (cycle_time < 1999)
	{
		printf("DHT_CYCLE_TIME ���� 2000�̸� �Դϴ�.\n����(%d), �ּ�(2000)\n", cycle_time);
		exit(1);
	}
	else if (if_time < 59999)
	{
		printf("DB ������ �Է� ������ �ʹ� ª���ϴ�.\n����(%d), �ּ�(60000)\n", if_time);
		exit(1);
	}
	
	//���� ������ ����
	int data_cnt = 0;
	float temp_total = 0;
	float hum_total = 0;
	float temp_stat = 0;
	float hum_stat = 0;
	//DB config ��������
	strcpy(db_name, read_config("@DHT_DB_NAME"));
	strcpy(db_table, read_config("@DHT_DB_TALBE"));
	strcpy(db_ip, read_config("@DHT_DB_IP"));
	strcpy(db_user, read_config("@DHT_DB_USER"));
	strcpy(db_passwd, read_config("@DHT_DB_PASS"));
	db_port = atoi(read_config("@DHT_DB_PORT"));

	//���� ������ �о����
	while (1)
	{
		read_dht_data(pin_num);
		if (temp != -1 || hum != -1)
		{
			temp_total += temp;
			hum_total += hum;
			data_cnt++;

			if (data_cnt == divide_num)
			{
				temp_stat = temp_total / (float)divide_num;
				hum_stat = hum_total / (float)divide_num;
				for (int j = 0; j < 3; j++)
				{
					if (db_data_isnert(temp_total, hum_total, temp_stat, hum_stat, data_cnt) == 0)
					{
						break;
					}
				}
				temp_total = 0;
				hum_total = 0;
				data_cnt = 0;
			}
			//printf("temp t=%.1f,hum t=%.1f,temp n=%.1f,hum n=%.1f,data_cnt=%d\n", temp_total, hum_total, temp_stat, hum_stat, data_cnt);
		}
		delay(cycle_time);
	}
}
