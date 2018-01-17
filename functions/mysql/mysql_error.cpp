php_var mysql_error(php_var mysql = global_mysql)
{
	return (php_var)mysql_error((MYSQL*)mysql.res);
}
