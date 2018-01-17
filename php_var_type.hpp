#ifndef __php_var_type
#define __php_var_type

#define PHP_NULL 0
#define PHP_STRING 1
#define PHP_INT 2
#define PHP_FLOAT 3
#define PHP_BOOL 4
#define PHP_ARRAY 5
#define PHP_RESOURCE 6
#define PHP_OBJECT 7

class php_var_type {
public:
	unsigned int is_null : 1;
	unsigned int is_string : 1;
	unsigned int is_int : 1;
	unsigned int is_float : 1;
	unsigned int is_bool : 1;
	unsigned int is_array : 1;
	unsigned int is_resource : 1;
	unsigned int is_object : 1;
	
	php_var_type() {
		is_null = 0;
		is_string = 0;
		is_int = 0;
		is_float = 0;
		is_bool = 0;
		is_array = 0;
		is_resource = 0;
		is_object = 0;
	}
	
	operator int() {
		if(is_null) return PHP_NULL;
		if(is_string) return PHP_STRING;
		if(is_int) return PHP_INT;
		if(is_float) return PHP_FLOAT;
		if(is_bool) return PHP_BOOL;
		if(is_array) return PHP_ARRAY;
		if(is_resource) return PHP_RESOURCE;
		if(is_object) return PHP_OBJECT;
		return -1;
	}
	
	operator char*() {
		if(is_null) return "NULL";
		if(is_string) return "String";
		if(is_int) return "Integer";
		if(is_float) return "Float";
		if(is_bool) return "Boolean";
		if(is_array) return "Array";
		if(is_resource) return "Resource";
		if(is_object) return "Object";
		return "";
	}
	
	php_var_type &operator= (const int rhs) {
		*this = php_var_type(); // reset
		switch( rhs ) {
			case PHP_NULL: is_null = true; break;
			case PHP_STRING: is_string = true; break;
			case PHP_INT: is_int = true; break;
			case PHP_FLOAT: is_float = true; break;
			case PHP_BOOL: is_bool = true; break;
			case PHP_ARRAY: is_array = true; break;
			case PHP_RESOURCE: is_resource = true; break;
			case PHP_OBJECT: is_object = true; break;
		}
		return *this;
	}
	
	bool operator== (const int rhs) {
		switch( rhs ) {
			case PHP_NULL: if(is_null) return true;
			case PHP_STRING: if(is_string) return true;
			case PHP_INT: if(is_int) return true;
			case PHP_FLOAT: if(is_float) return true;
			case PHP_BOOL: if(is_bool) return true;
			case PHP_ARRAY: if(is_array) return true;
			case PHP_RESOURCE: if(is_resource) return true;
			case PHP_OBJECT: if(is_object) return true;
		}
		return false;
	}
};

#endif
