#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <sqlite3.h>

#include "tlv-rewrite.h"


static sqlite3 *db;

static int check_db(sqlite3 * db, const char * sql, char * result_string)
{

	int retval = 0;
        sqlite3_stmt *res;
        int try_counter = 10;
        const unsigned char * result_text;
	int text_length;

        int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

        if (rc != SQLITE_OK) {
                fprintf(stderr, "sqlite3_prepare_v2: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                exit(1);
        }


        while (try_counter > 0)
        {
                rc = sqlite3_step(res);
                if (rc == SQLITE_ROW) break;
                if (rc == SQLITE_DONE) break;
                if (rc != SQLITE_BUSY) break;
                try_counter --;
                usleep(200000);
        }

        /* histogram[try_counter]++;  */

	switch (rc)
	{
	case SQLITE_ROW:
		result_text = sqlite3_column_text(res, 0);
		text_length = sqlite3_column_bytes(res, 0);

		if (text_length == 4)
		{
			memcpy(result_string, result_text, 4);
			retval = 1;
		}
		break;

	case SQLITE_DONE:    /*  nothing found, empty result */
		break;

	default:
                fprintf(stderr, "returncode: %d\n", rc);
                fprintf(stderr, "sqlite3_step: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(res);
                sqlite3_close(db);
                exit(1);
	}



        sqlite3_finalize(res);

        return retval;
}








void tlv_rewrite_opendb(void)
{
        int rc = sqlite3_open_v2("/var/lib/radsecproxy/data.db", &db, SQLITE_OPEN_READONLY, 0);

        if (rc != SQLITE_OK) {

                fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);

                exit(1);
        }
}



void tlv_rewrite ( uint8_t * value, int tlv_type, int tlv_length )
{

   char buf[20];
   char sql_buf[80];
   const char * table;
   char result_string[10];
   int res;
   int i;

   for (i=0; i < 17; i++)
   {
      buf[i] = tolower(value[i]);
   }
   buf[17] = 0;




   table = "called_station";

   if (tlv_type == 31)
   {
     table = "calling_station";
   }
   else
   {
      if ((tlv_length > 18) && (value[17] == ':'))  /* WLAN-AP with SSID */
      {
	buf[16] = '0'; /* set last 4 bits of MAC address to '0' */
      }
   }

   snprintf(sql_buf, sizeof sql_buf, "SELECT v FROM %s WHERE addr = '%s'", table, buf );


   res = check_db(db, sql_buf, result_string);

   if (res == 1)
   {
	value[2] = value[3];
	value[3] = value[4];

	value[4] = value[6];
	value[5] = value[7];

	value[6] = value[9];
	value[7] = value[10];

	value[8] = value[12];
	value[9] = value[13];

	value[10] = value[15];
	value[11] = value[16];

	value[12] = '-';

	value[13] = result_string[0];
	value[14] = result_string[1];
	value[15] = result_string[2];
	value[16] = result_string[3];

   }
}

