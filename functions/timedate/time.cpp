php_var time()
{
	//php vars
	php_var ret; //The return variable

	ret = ((long)time(NULL));
	return ret;
}