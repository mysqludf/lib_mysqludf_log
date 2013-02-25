LIB\_MYSQLUDF\_LOG
================

Often it can be useful to write to a log file from inside a SQL statement or stored procedure. Currently this library contains only one function that allows you to write to the error log file.

This library lib\_mysqludf\_log can be used to write debug information to the log files. The following function is currently supported:

* `log_error` - writes a line to the log.

Use lib_mysqludf_log_info() to obtain information about the currently installed version of lib_mysqludf_log.

Functions
---------

###log_error(message)###

`log_error` writes the string argument as a new line to the error log. The log line is written for each call to log_error. The log is flushed after the completion of the statement containing the call to the log_error call. So, if `log_error` is used in a SELECT statement, the log messages may become visible after the SQL statement is executed. During execution, log message may become visible, but there is no guarantee that all messages are written until the statement finishes.

The location of the error log can be found using the following statement:

```
mysql> SHOW VARIABLES LIKE 'log_error';
+---------------+-------------------+
| Variable_name | Value             |
+---------------+-------------------+
| log_error     | ./mysql_error.log |
+---------------+-------------------+
```
    
A relative path for the error log locatoin, such as what is shown above, is resolved against the data directory. This can be found using the following statement:

```
mysql> SHOW VARIABLES LIKE 'datadir';
+---------------+----------------------+
| Variable_name | Value                |
+---------------+----------------------+
| datadir       | /opt/mysql/5.1/data/ |
+---------------+----------------------+
```
    
#####message#####
The string message to be written to the error log. If message is not a string type, it is coerced into one. If message is NULL, the string NULL will be written. The message argument can be as long as you like, but only the first 9999 bytes will be written to the error log.

#####returns#####
The return value should be 0 and has no significance.

####Examples####

```
SELECT log_error('whoops....');
```

