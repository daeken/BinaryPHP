#include "php_var.hpp"

char* intstring(long i)
{
	char *buf = new char[11];
	sprintf(buf, "%i", i);
	return buf;
}
char* doublestring(double i)
{
	char *buf = new char[16];
	sprintf(buf, "%d", i);
	return buf;
}

php_var::php_var() // Constructor
{
	container = "";
	type = PHP_NULL; // Make the var, but make it null.
}
php_var::php_var(const char* str)
{
	container = str;
	type = PHP_STRING;
}
php_var::php_var(double i)
{
	float_container = i;
	type = PHP_FLOAT;
}
php_var::php_var(int i)
{
	int_container = i;
	type = PHP_INT;
}
php_var::php_var(unsigned int i)
{
	int_container = i;
	type = PHP_INT;
}
php_var::php_var(long i)
{
	int_container = i;
	type = PHP_INT;
}
php_var::php_var(const php_var &temp)
{
	type = temp.type;
	container = temp.container;
  int_container = temp.int_container;
  float_container = temp.float_container;
	keys = temp.keys;
	data = temp.data;
	res = temp.res;
}
php_var::php_var(char * str)
{
	container = str;
	type = PHP_STRING;
}
php_var::php_var(string str)
{
	container = str;
	type = PHP_STRING;
}
php_var::php_var(bool b)
{
	if(b)
		int_container = 1;
	else
		int_container = 0;
	type = PHP_BOOL;
}
php_var::php_var(void *bleh)
{
  obj = bleh;
  type = PHP_OBJECT;
}
php_var::operator const char*()
{
	if(type.is_array || type.is_object || type.is_resource )
		return (char *)type;
  switch(type)
  {
  case PHP_INT:
    return intstring(getint());
  case PHP_FLOAT:
    return doublestring(getfloat());
  case PHP_STRING:
    return getstring().c_str();
  }
}
php_var::operator string()
{
	if(type.is_array || type.is_object || type.is_resource )
		return string((char *)type);
	switch(type)
  {
  case PHP_INT:
    return intstring(getint());
  case PHP_FLOAT:
    return doublestring(getfloat());
  case PHP_STRING:
    return getstring();
  }
}
php_var::operator bool()
{
	if( !type.is_bool || (type.is_bool && int_container == 0))
		return true;
	return false;
}
php_var::operator double()
{
  return getfloat();
}
php_var::operator float()
{
  return getfloat();
}
php_var::operator int()
{
  return getint();
}
php_var::operator unsigned int()
{
  return getint();
}
php_var::operator long()
{
  return getlong();
}
php_var &php_var::operator[](int subscript)
{
	return (*this)[(php_var)subscript];
}
php_var &php_var::operator[](unsigned int subscript)
{
	return (*this)[(php_var)subscript];
}
php_var &php_var::operator[](const char* subscript)
{
	return (*this)[(php_var)subscript];
}
php_var &php_var::operator[](char* subscript)
{
	return (*this)[(php_var)subscript];
}
php_var &php_var::operator[](string subscript)
{
	return (*this)[(php_var)subscript];
}
php_var &php_var::operator[](php_var subscript)
{
	if(type.is_string)
	{
		throw php_var("can't handle array access to strings");
	}
	else if(type.is_array)
	{
		php_var key = subscript;
		unsigned int i = 0;
		for(i = 0;i < keys.size(); ++i)
		{
			if(key.container.compare(keys[i].container) == 0)
				return data[i];
		}
		php_var temp;
		keys.push_back(key);
		data.push_back(temp);
		return data[i];
	}
}
bool php_var::operator<(int i)
{
	if(getint() < i)
		return true;
	return false;
}
bool php_var::operator>(int i)
{
	if(getint() > i)
		return true;
	return false;
}
bool php_var::operator<(php_var i)
{
	if(getint() < i.getint())
		return true;
	return false;
}
bool php_var::operator>(php_var i)
{
	if(getint() > i.getint())
		return true;
	return false;
}
bool php_var::operator!=(const char* cmp)
{
	if(container.compare(cmp))
		return true;
	return false;
}
bool php_var::operator!=(int i)
{
	if(getint() == i)
		return false;
	return true;
}
bool php_var::operator!=(php_var var)
{
  switch(type)
  {
  case PHP_INT:
    return var.getint() != getint();
    break;
  case PHP_FLOAT:
    return var.getfloat() != getfloat();
    break;
  case PHP_BOOL:
    return var.getbool() != getbool();
    break;
  case PHP_STRING:
    return container.compare(var.getstring()) != 0;
    break;
  }
	return false;
}
bool php_var::operator==(const char* cmp)
{
  switch(type)
  {
  case PHP_INT:
    return strcmp(cmp, intstring(getint())) == 0;
    break;
  case PHP_FLOAT:
    return strcmp(cmp, doublestring(getfloat())) == 0;
    break;
  case PHP_BOOL:
    return strcmp(cmp, intstring(getint())) == 0;
    break;
  case PHP_STRING:
    return container.compare(cmp) == 0;
    break;
  }
	return false;
}
bool php_var::operator==(int i)
{
  switch(type)
  {
  case PHP_INT:
    return (i == getint());
    break;
  case PHP_FLOAT:
    return (i == getfloat());
    break;
  case PHP_BOOL:
    return (i == getbool());
    break;
  case PHP_STRING:
    return container.compare(intstring(i)) == 0;
    break;
  }
	return false;
	if(atol(container.c_str()) == i)
		return true;
	return false;
}
bool php_var::operator==(php_var var)
{
  switch(type)
  {
  case PHP_INT:
    return (var.getint() == getint());
    break;
  case PHP_FLOAT:
    return (var.getfloat() == getfloat());
    break;
  case PHP_BOOL:
    return (var.getbool() == getbool());
    break;
  case PHP_STRING:
    return container.compare(var.getstring()) == 0;
    break;
  }
	return false;
}
int php_var::operator++(int i)
{
	int ret = getint();
	*this = ret + 1;
	return ret;
}
int php_var::operator++()
{
	return (*this = getint() + 1);
}
int php_var::operator--(int i)
{
	int ret = getint();
	*this = ret - 1;
	return ret;
}
int php_var::operator--()
{
	return (*this = getint() - 1);
}
php_var &php_var::operator=(const php_var &temp)
{
	type = temp.type;
	container = temp.container;
  int_container = temp.int_container;
  float_container = temp.float_container;
	keys = temp.keys;
	data = temp.data;
	res = temp.res;
	return *this;
}
php_var &php_var::operator=(int i)
{
	int_container = i;
	type = PHP_INT;
	return *this;
}
php_var &php_var::operator=(unsigned int i)
{
	int_container = i;
	type = PHP_INT;
	return *this;
}
php_var &php_var::operator=(long i)
{
	int_container = i;
	type = PHP_INT;
	return *this;
}
php_var &php_var::operator=(double d)
{
	float_container = d;
	type = PHP_FLOAT;
	return *this;
}
php_var &php_var::operator=(char *str)
{
	container = str;
	type = PHP_STRING;
	return *this;
}
php_var &php_var::operator=(string str)
{
	container = str;
	type = PHP_STRING;
	return *this;
}
php_var &php_var::operator=(bool b)
{
	if(b)
		int_container = 1;
	else
		int_container = 0;
	type = PHP_BOOL;
	return *this;
}
void php_var::operator+=(int inc)
{
  switch(type)
  {
  case PHP_INT:
    int_container = getint() + inc;
    break;
  case PHP_FLOAT:
    float_container = getfloat() + inc;
    break;
  case PHP_STRING:
    container += intstring(inc);
    break;
  }
}
void php_var::operator+=(php_var inc)
{
  switch(type)
  {
  case PHP_INT:
    switch(inc.type)
    {
    case PHP_INT:
      int_container += inc.int_container;
      break;
    case PHP_FLOAT:
      int_container += (int) inc.float_container;
      break;
    case PHP_STRING:
      container = (string) intstring(int_container) + inc.container;
      break;
    }
    break;
  case PHP_FLOAT:
    switch(inc.type)
    {
    case PHP_INT:
      float_container += inc.int_container;
      break;
    case PHP_FLOAT:
      float_container += inc.float_container;
      break;
    case PHP_STRING:
      container = (string) doublestring(int_container) + inc.container;
      break;
    }
    break;
  case PHP_STRING:
    switch(inc.type)
    {
    case PHP_INT:
      container += intstring(inc.int_container);
      break;
    case PHP_FLOAT:
      container += doublestring(inc.float_container);
      break;
    case PHP_STRING:
      container += inc.container;
      break;
    }
    break;
  }
}
void php_var::operator+=(string str)
{
	switch(type)
  {
  case PHP_INT:
    container = (string) intstring(int_container) + str;
    break;
  case PHP_FLOAT:
    container = (string) doublestring(float_container) + str;
    break;
  case PHP_STRING:
    container += str;
    break;
  }
}
void php_var::operator+=(const char* str)
{
	switch(type)
  {
  case PHP_INT:
    container = (string) intstring(int_container) + str;
    break;
  case PHP_FLOAT:
    container = (string) doublestring(float_container) + str;
    break;
  case PHP_STRING:
    container += str;
    break;
  }
}
void php_var::operator+=(char* str)
{
	switch(type)
  {
  case PHP_INT:
    container = (string) intstring(int_container) + str;
    break;
  case PHP_FLOAT:
    container = (string) doublestring(float_container) + str;
    break;
  case PHP_STRING:
    container += str;
    break;
  }
}
void php_var::operator-=(int dec)
{
  switch(type)
  {
  case PHP_INT:
    int_container = getint() - dec;
    break;
  case PHP_FLOAT:
    float_container = getfloat() - dec;
    break;
  }
}
void php_var::operator-=(php_var inc)
{
  switch(type)
  {
  case PHP_INT:
    switch(inc.type)
    {
    case PHP_INT:
      int_container -= inc.int_container;
      break;
    case PHP_FLOAT:
      int_container -= (int) inc.float_container;
      break;
    case PHP_STRING:
      container = (string) intstring(int_container) + inc.container;
      break;
    }
    break;
  case PHP_FLOAT:
    switch(inc.type)
    {
    case PHP_INT:
      float_container -= inc.int_container;
      break;
    case PHP_FLOAT:
      float_container -= inc.float_container;
      break;
    case PHP_STRING:
      container = (string) doublestring(int_container) + inc.container;
      break;
    }
    break;
  }
}
void php_var::to_array()
{
	type = PHP_ARRAY;
	if(container.length() > 0) {
		(*this)[0] = container;
		container = "";
	}
}
template<typename T> inline T * OBJ(php_var obj) { return (reinterpret_cast<T *>(obj.res)); }
ostream &operator<<( ostream &out, const php_var &var )
{
	if(var.type.is_array)
  {
		out << "Array";
    return out;
  }
  php_var *php = (php_var *) &var;
	switch(php->type)
  {
  case PHP_BOOL:
  case PHP_INT:
		out << intstring(php->int_container);
    break;
  case PHP_FLOAT:
    out << doublestring(php->float_container);
    break;
  case PHP_STRING:
    out << php->container;
    break;
  }
	return out;
}
bool operator<(int i, php_var v)
{
	return(v > i);
}
bool operator>(int i, php_var v)
{
	return(v < i);
}
php_var operator-(php_var l, php_var r)
{
	return (php_var)((int) l - (int) r);
}
float operator/(php_var &l, php_var &r)
{
	return (float) *l / (float) *r;
}
php_var operator*(php_var l, php_var r)
{
	return (php_var)((long) l - (long) r);
}

int php_var::getint()
{
  switch(type)
  {
  case PHP_INT:
    return int_container;
    break;
  case PHP_FLOAT:
    return (int) float_container;
    break;
  case PHP_BOOL:
    return (int_container == 0) ? 0 : 1;
    break;
  case PHP_STRING:
    return atoi(container.c_str());
  default:
    return 0;
    break;
  }
}
float php_var::getfloat()
{
  switch(type)
  {
  case PHP_INT:
    return (float) int_container;
    break;
  case PHP_FLOAT:
    return float_container;
    break;
  case PHP_BOOL:
    return (int_container == 0) ? 0 : 1;
    break;
  case PHP_STRING:
    return atof(container.c_str());
  default:
    return 0;
    break;
  }
}
bool php_var::getbool()
{
  if(getint() == 1)
    return true;
  return false;
}
long php_var::getlong()
{
  switch(type)
  {
  case PHP_INT:
    return int_container;
    break;
  case PHP_FLOAT:
    return (long) float_container;
    break;
  case PHP_BOOL:
    return (getbool()) ? 0 : 1;
    break;
  case PHP_STRING:
    return atol(container.c_str());
  default:
    return 0;
    break;
  }
}
string php_var::getstring()
{
  switch(type)
  {
  case PHP_INT:
    return intstring(int_container);
    break;
  case PHP_FLOAT:
    return doublestring(float_container);
    break;
  case PHP_BOOL:
    return (getbool()) ? "0" : "1";
    break;
  case PHP_STRING:
    return container;
    break;
  default:
    return "";
    break;
  }
}

#define is_identical(var, varb) (((var) == (varb) && (var).type == (varb).type) ? (php_var)true : (php_var)false)
