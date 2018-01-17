php_var chr(php_var num)
{
	int i = (int)num;
	char c[2] = {0, 0};
	sprintf( c, "%c", (char)i );
	return php_var(c);
}