#include "php_var.hpp"

#ifndef __arg_list
#define __arg_list

class arg_list {
private:
	struct arg_item {
		int index;
		php_var *value;
		arg_item *next;
		
		arg_item() { index = 0; value = NULL; next = NULL; }
	};
	
	int length;
	arg_item *first;
	arg_item *current;
	arg_item *last;

public:
	arg_list();
	~arg_list();
	
	void add( php_var* );		// Add an argument
	const php_var* cur();		// Return current arg (does *not* iterate)
	const php_var* fetch();		// Return current arg, and iterate
	int pos();			// Current offset
	int len();			// Number of items
	void start();			// Set current arg to the first in the list
	void skip( int = 1 );		// Skip args
	void end();			// Set current arg to the last in the list
};

//define FUNCTION(name) php_var name(arg_list args)
//define RETURN(var) return php_var(var)

#endif
