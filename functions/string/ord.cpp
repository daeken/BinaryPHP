php_var ord(php_var ch)
{
	const char *str = (const char *)ch;
	return php_var( (int)*str );
}