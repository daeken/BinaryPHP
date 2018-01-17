php_var mysql_connect(php_var host = "localhost:3306", php_var user = "", php_var passwd = "")
{
	MYSQL* mysql = NULL;
	mysql = mysql_init(mysql);
	unsigned int port = 3306;
	if(!mysql_real_connect(mysql, host.container.c_str(), user.container.c_str(), passwd.container.c_str(), NULL, port, NULL, 0))
	{
		global_mysql.res = (void*)mysql;
		global_mysql.type = PHP_RESOURCE;
		return (php_var)false;
	}
	php_var ret;
	ret.res = (void*)mysql;
	ret.type = PHP_RESOURCE;
	global_mysql.res = (void*)mysql;
	global_mysql.type = PHP_RESOURCE;
	return ret;
}
