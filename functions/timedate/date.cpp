char *mon_full_names[] =
{
	"January", "February", "March", "April",
	"May", "June", "July", "August",
	"September", "October", "November", "December"
};
char *mon_short_names[] =
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
char *day_full_names[] =
{
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
char *day_short_names[] =
{
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static int phpday_tab[2][12] =
{
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

#define isleap(year) (((year%4) == 0 && (year%100)!=0) || (year%400)==0)
#define YEAR_BASE 1900

#define HAVE_TM_GMTOFF 1
#define HAVE_TM_ZONE 1

php_var date(php_var format, php_var timestamp=false) {
	//php vars
	php_var ret; //The return variable
	time_t the_time;
	struct tm *ta, tmbuf;
	char tmp_buff[32];
	int i, size = 0, length, h, beat, fd, wd, yd, wk;
	long tzone;

	// if a timestamp is provided, use it.  otherwise use the current time
	if (timestamp) {
		the_time = long(timestamp);
	} else {
		the_time = time(NULL);
	}

	ta = localtime_r(&the_time, &tmbuf);
	
	for (i = 0; i < format.container.length(); i++) {
		switch (format.container[i]) {
			case '\\':
				if(i < format.container.length()-1) {
					tmp_buff[0] = format.container[i+1];
					tmp_buff[1] = '\0';
					ret += tmp_buff;
					i++;
				}
				break;

			case 'U':		/* seconds since the epoch */
				sprintf(tmp_buff, "%ld", (long)the_time); /* SAFE */
				// printf("%ld", (long)the_time);
				ret += tmp_buff;
				break;

			case 'F':		/* month, textual, full */
				ret += mon_full_names[ta->tm_mon];
				break;

			case 'l':		/* day (of the week), textual, full */
				ret += day_full_names[ta->tm_wday];
				break;
			case 'Y':		/* year, numeric, 4 digits */
				sprintf(tmp_buff, "%d", ta->tm_year + YEAR_BASE);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'M':		/* month, textual, 3 letters */
				ret += mon_short_names[ta->tm_mon];
				break;
			case 'D':		/* day (of the week), textual, 3 letters */
				ret += day_short_names[ta->tm_wday];
				break;
			case 'z':		/* day (of the year) */
				sprintf(tmp_buff, "%d", ta->tm_yday);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'y':		/* year, numeric, 2 digits */
				sprintf(tmp_buff, "%02d", ((ta->tm_year)%100));  /* SAFE */
				ret += tmp_buff;
				break;
			case 'm':		/* month, numeric */
				sprintf(tmp_buff, "%02d", ta->tm_mon + 1);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'n':      /* month, numeric, no leading zeros */
				sprintf(tmp_buff, "%d", ta->tm_mon + 1);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'd':		/* day of the month, numeric */
				sprintf(tmp_buff, "%02d", ta->tm_mday);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'j':
				sprintf(tmp_buff, "%d", ta->tm_mday); /* SAFE */
				ret += tmp_buff;
				break;
			case 'H':		/* hour, numeric, 24 hour format */
				sprintf(tmp_buff, "%02d", ta->tm_hour);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'h':		/* hour, numeric, 12 hour format */
				h = ta->tm_hour % 12; if (h==0) h = 12;
				sprintf(tmp_buff, "%02d", h);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'G':      /* hour, numeric, 24 hour format, no leading zeros */
				sprintf(tmp_buff, "%d", ta->tm_hour);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'g':      /* hour, numeric, 12 hour format, no leading zeros */
				h = ta->tm_hour % 12; if (h==0) h = 12;
				sprintf(tmp_buff, "%d", h);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'i':		/* minutes, numeric */
				sprintf(tmp_buff, "%02d", ta->tm_min);  /* SAFE */
				ret += tmp_buff;
				break;
			case 's':		/* seconds, numeric */
				sprintf(tmp_buff, "%02d", ta->tm_sec);  /* SAFE */ 
				ret += tmp_buff;
				break;
			case 'A':		/* AM/PM */
				ret += (ta->tm_hour >= 12 ? "PM" : "AM");
				break;
			case 'a':		/* am/pm */
				ret += (ta->tm_hour >= 12 ? "pm" : "am");
				break;
			case 'S':		/* standard english suffix, e.g. 2nd/3rd for the day of the month */
				if (ta->tm_mday >= 10 && ta->tm_mday <= 19) {
					ret += "th";
				} else {
					switch (ta->tm_mday % 10) {
						case 1:
							ret += "st";
							break;
						case 2:
							ret += "nd";
							break;
						case 3:
							ret += "rd";
							break;
						default:
							ret += "th";
							break;
					}
				}
				break;

			case 't':		/* days in current month */
				sprintf(tmp_buff, "%2d", phpday_tab[isleap((ta->tm_year+YEAR_BASE))][ta->tm_mon] );
				ret += tmp_buff;
				break;
			case 'w':		/* day of the week, numeric EXTENSION */
				sprintf(tmp_buff, "%01d", ta->tm_wday);  /* SAFE */
				ret += tmp_buff;
				break;
			case 'O':		/* GMT offset in [+-]HHMM format */
#if HAVE_TM_GMTOFF				
				sprintf(tmp_buff, "%c%02d%02d", (ta->tm_gmtoff < 0) ? '-' : '+', abs(ta->tm_gmtoff / 3600), abs( (ta->tm_gmtoff % 3600) / 60 ));
#else
				sprintf(tmp_buff, "%c%02d%02d", ((ta->tm_isdst ? tzone - 3600:tzone)>0)?'-':'+', abs((ta->tm_isdst ? tzone - 3600 : tzone) / 3600), abs(((ta->tm_isdst ? tzone - 3600 : tzone) % 3600) / 60));
#endif
				ret += tmp_buff;
				break;
			case 'Z':		/* timezone offset in seconds */
#if HAVE_TM_GMTOFF
				sprintf(tmp_buff, "%ld", ta->tm_gmtoff);
#else
				sprintf(tmp_buff, "%ld", ta->tm_isdst ? -(tzone- 3600) : -tzone);
#endif
				ret += tmp_buff;
				break;
			case 'L':		/* boolean for leapyear */
				sprintf(tmp_buff, "%d", (isleap((ta->tm_year+YEAR_BASE)) ? 1 : 0 ) );
				ret += tmp_buff;
				break;
			case 'T':		/* timezone name */
#if HAVE_TM_ZONE
				ret += ta->tm_zone;
#elif HAVE_TZNAME
				ret += (ta->tm_isdst ? tname[1] : tname[0]);
#endif
				break;
			case 'B':	/* Swatch Beat a.k.a. Internet Time */
				beat =  (((((long)the_time)-(((long)the_time) -
					((((long)the_time) % 86400) + 3600))) * 10) / 864);
				while (beat < 0) {
					beat += 1000;
				}
				beat = beat % 1000;
				sprintf(tmp_buff, "%03d", beat); /* SAFE */
				ret += tmp_buff;
				break;
			case 'I':
				sprintf(tmp_buff, "%d", ta->tm_isdst);
				ret += tmp_buff;
				break;
			case 'r':
#if HAVE_TM_GMTOFF				
				sprintf(tmp_buff, "%3s, %2d %3s %04d %02d:%02d:%02d %c%02d%02d", 
					day_short_names[ta->tm_wday],
					ta->tm_mday,
					mon_short_names[ta->tm_mon],
					ta->tm_year + YEAR_BASE,
					ta->tm_hour,
					ta->tm_min,
					ta->tm_sec,
					(ta->tm_gmtoff < 0) ? '-' : '+',
					abs(ta->tm_gmtoff / 3600),
					abs( (ta->tm_gmtoff % 3600) / 60 )
				);
#else
				sprintf(tmp_buff, "%3s, %2d %3s %04d %02d:%02d:%02d %c%02d%02d", 
					day_short_names[ta->tm_wday],
					ta->tm_mday,
					mon_short_names[ta->tm_mon],
					ta->tm_year + YEAR_BASE,
					ta->tm_hour,
					ta->tm_min,
					ta->tm_sec,
					((ta->tm_isdst ? tzone - 3600 : tzone) > 0) ? '-' : '+',
					abs((ta->tm_isdst ? tzone - 3600 : tzone) / 3600),
					abs( ((ta->tm_isdst ? tzone - 3600 : tzone) % 3600) / 60 )
				);
#endif
				ret += tmp_buff;
				break;
			case 'W':		/* ISO-8601 week number of year, weeks starting on Monday */
				wd = ta->tm_wday==0 ? 6 : ta->tm_wday-1;/* weekday */
				yd = ta->tm_yday + 1;					/* days since January 1st */

				fd = (7 + wd - yd % 7+ 1) % 7;			/* weekday (1st January) */	

				/* week is a last year week (52 or 53) */
				if ((yd <= 7 - fd) && fd > 3){			
					wk = (fd == 4 || (fd == 5 && isleap((ta->tm_year + YEAR_BASE - 1)))) ? 53 : 52;
				}
				/* week is a next year week (1) */
				else if (isleap((ta->tm_year+YEAR_BASE)) + 365 - yd < 3 - wd){
					wk = 1;
				}
				/* normal week */
				else {
					wk = (yd + 6 - wd + fd) / 7 - (fd > 3);
				}

				sprintf(tmp_buff, "%d", wk);  /* SAFE */
				ret += tmp_buff;
				break;

			default:
				tmp_buff[0] = format.container[i];
				tmp_buff[1] = '\0';
				ret += tmp_buff;
				break;
		}
	}

	return ret;
}