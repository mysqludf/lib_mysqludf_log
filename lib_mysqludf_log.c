/* 
    lib_mysqludf_log - a library of mysql udfs to write to log files
    Copyright (C) 2008  Roland Bouman 
    web: http:// 
    email: roland.bouman@gmail.com
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
    
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
    
    BUILDING:
    
    gcc -I/opt/mysql/mysql/include -shared -o lib_mysqludf_log.so lib_mysqludf_log.c
    
    INSTALLING:
    
    For MySQL 5.1, Copy the shared library to the plugin directory
    For MySQL < 5.1, copy it to some standard library location, like /lib, /usr/lib, /usr/lib64
    
    In MySQL, install the function like so:
    
    create function log_error returns integer soname 'lib_mysqludf_log.so';
    
    UNINSTALL:
    
    drop function log_error;
    
    EXECUTE: 
    
    select log_error('boehahaha');
    
    (the function always returns 0, but in the error log, you will see a line 'boehahaha')
    Location of the error log:
    
    mysql> show variables like 'log_error';
    +---------------+-------------------+
    | Variable_name | Value             |
    +---------------+-------------------+
    | log_error     | ./mysql_error.log |
    +---------------+-------------------+
     
    (relative paths resolved against data directory)
    
    mysql> show variables like 'datadir';
    +---------------+----------------------+
    | Variable_name | Value                |
    +---------------+----------------------+
    | datadir       | /opt/mysql/5.1/data/ |
    +---------------+----------------------+
    
    
*/
#if defined(_WIN32)
#define DLLEXP __declspec(dllexport) 
#else
#define DLLEXP
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mysql.h>

#define LIBVERSION "lib_mysqludf_log version 0.0.2"
#define ERR_ARG "Exactly one argument expected."
#define ERR_MEM "Out of memory."

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * lib_mysqludf_json_info
 */
DLLEXP 
my_bool lib_mysqludf_log_info_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	return 0;
}

DLLEXP 
void lib_mysqludf_log_info_deinit(
	UDF_INIT *initid
){}

DLLEXP 
char* lib_mysqludf_log_info(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
){
	*length = strlen(LIBVERSION);
	return LIBVERSION; 
}

DLLEXP 
my_bool log_error_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	if(args->arg_count==1){
		//alloc mem for format pattern:
		// "%s0.xxxx\n"
		// where xxxx is the maxi
		// 7 = length of %s0.\n + trailing \0
		if(!(initid->ptr = (char *)malloc(7 + 4))){
			strcpy(message, ERR_MEM);
			return 1;
		}
		args->arg_type[0] = STRING_RESULT;
		initid->maybe_null = 0;
	} else {
		strcpy(message, ERR_ARG);
		return 1;
	}
	return 0;
}

DLLEXP 
void log_error_deinit(
	UDF_INIT *initid
){
	if(initid->ptr)
	{
		fflush(stderr);
		free(initid->ptr);
	}
}

DLLEXP 
my_ulonglong log_error(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
){
	int numDigits;
	char *fmt;
	*is_null = 0;
	if(args->args[0]==NULL){
		fprintf(stderr, "NULL\n");
	} else {
		fmt = (char *)initid->ptr;
		memcpy(fmt, "%0.", 3);
		sprintf(fmt+3, "%d", args->lengths[0]<=9999? args->lengths[0]: 9999);
		numDigits = strlen(fmt + 4);
		memcpy(fmt + 4 + numDigits, "s\n\0", 3);
		fprintf(stderr, fmt, args->args[0]);
	}
	return 0;
}


#ifdef  __cplusplus
}
#endif
