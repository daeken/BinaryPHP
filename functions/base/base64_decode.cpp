unsigned char *php_base64_decode(const unsigned char *str, int length)
{
	const unsigned char *current = str;
	int ch, i = 0, j = 0, k;
	/* this sucks for threaded environments */
	unsigned char *result;
	
	result = new unsigned char[(length + 1)];
	if (result == NULL) {
		return NULL;
	}

	/* run through the whole string, converting as we go */
	while ((ch = *current++) != '\0' && length-- > 0) {
		if (ch == base64_pad) break;

	    /* When Base64 gets POSTed, all pluses are interpreted as spaces.
		   This line changes them back.  It's not exactly the Base64 spec,
		   but it is completely compatible with it (the spec says that
		   spaces are invalid).  This will also save many people considerable
		   headache.  - Turadg Aleahmad <turadg@wise.berkeley.edu>
	    */

		if (ch == ' ') ch = '+'; 

		ch = base64_reverse_table[ch];
		if (ch < 0) continue;

		switch(i % 4) {
		case 0:
			result[j] = ch << 2;
			break;
		case 1:
			result[j++] |= ch >> 4;
			result[j] = (ch & 0x0f) << 4;
			break;
		case 2:
			result[j++] |= ch >>2;
			result[j] = (ch & 0x03) << 6;
			break;
		case 3:
			result[j++] |= ch;
			break;
		}
		i++;
	}

	k = j;
	/* mop things up if we ended on a boundary */
	if (ch == base64_pad) {
		switch(i % 4) {
		case 0:
		case 1:
			delete result;
			return NULL;
		case 2:
			k++;
		case 3:
			result[k++] = 0;
		}
	}
	result[j] = '\0';
	return result;
}

php_var base64_decode(php_var str)
{
	unsigned char *result;
	php_var str_len;
	str_len = strlen(str);
	result = php_base64_decode(str, str_len);
	if( result != NULL )
	{
		return result;
	}	else	{
		return false;
	}

}