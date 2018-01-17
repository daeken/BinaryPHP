php_var gethostbyname( php_var host ) {
	struct hostent *host_entry;
	struct in_addr address;
	if(!(host_entry = gethostbyname( (const char*)host )) || !*(host_entry->h_addr_list)) {
		return host;
	}
	memcpy(&address.s_addr, *(host_entry->h_addr_list), sizeof(address.s_addr));
	return php_var(inet_ntoa(address));
}