unsigned char *php_base64_encode(const unsigned char *str, int length)
{
	const unsigned char *current = str;
	int i = 0;
	unsigned char *result = new unsigned char[((length + 3 - length % 3) * 4 / 3 + 1) * sizeof(char)];

	while (length > 2) { /* keep going until we have less than 24 bits */
		result[i++] = base64_table[current[0] >> 2];
		result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		result[i++] = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		result[i++] = base64_table[current[2] & 0x3f];

		current += 3;
		length -= 3; /* we just handle 3 octets of data */
	}

	/* now deal with the tail end of things */
	if (length != 0) {
		result[i++] = base64_table[current[0] >> 2];
		if (length > 1) {
			result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			result[i++] = base64_table[(current[1] & 0x0f) << 2];
			result[i++] = base64_pad;
		}
		else {
			result[i++] = base64_table[(current[0] & 0x03) << 4];
			result[i++] = base64_pad;
			result[i++] = base64_pad;
		}
	}
	result[i] = '\0';
	return result;
}

php_var base64_encode(php_var str)
{
	unsigned char *result;
	php_var str_len;
	str_len = strlen(str);
	result = php_base64_encode(str, str_len);
	if( result != NULL )
	{
		return result;
	} else	{
		return false;
	}
}