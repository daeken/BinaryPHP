php_var gettimeofday()
{
	//php vars
	php_var ret; //The return variable
	ret.to_array(); //An assosiative array will be returned.
	//time vars
	struct timeval tp;
	struct timezone tz;
	if( gettimeofday( &tp, &tz ) == 0 ){
		ret["sec"] = tp.tv_sec;
		ret["usec"] = tp.tv_usec;
#ifdef __WIN32
		ret["minuteswest"] = tz.tz_minuteswest/60;
#else
		ret["minuteswest"] = tz.tz_minuteswest;
#endif
		ret["dsttime"] = tz.tz_dsttime;
		return ret;
	}	else	{
		return false;
	}
}
