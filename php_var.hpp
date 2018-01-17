#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

#ifndef __php_var
#define __php_var

#include "php_var_type.hpp"

using namespace std;

char* intstring(long i);
char* doublestring(double i);

class php_var
{
public:
  php_var(); // Constructor
  php_var(const char* str);
  php_var(double i);
  php_var(int i);
  php_var(unsigned int i);
  php_var(long i);
  php_var(const php_var &temp);
  php_var(char * str);
  php_var(string str);
  php_var(bool b);
  php_var(void *foo);

  operator const char*();
  operator string();
  operator bool();
  operator double();
  operator float();
  operator int();
  operator unsigned int();
  operator long();
  
  php_var &operator[](int subscript);
  php_var &operator[](unsigned int subscript);
  php_var &operator[](const char* subscript);
  php_var &operator[](char* subscript);
  php_var &operator[](string subscript);
  php_var &operator[](php_var subscript);
  
  bool operator<(int i);
  bool operator>(int i);
  bool operator<(php_var i);
  bool operator>(php_var i);
  bool operator!=(const char* cmp);
  bool operator!=(int i);
  bool operator!=(php_var var);
  bool operator==(const char* cmp);
  bool operator==(int i);
  bool operator==(php_var var);
  
  int operator++(int i);
  int operator++();
  int operator--(int i);
  int operator--();
  
  php_var &operator=(const php_var &temp);
  php_var &operator=(int i);
  php_var &operator=(unsigned int i);
  php_var &operator=(long i);
  php_var &operator=(double d);
  php_var &operator=(char *str);
  php_var &operator=(string str);
  php_var &operator=(bool b);
  
  void operator+=(int inc);
  void operator+=(php_var inc);
  void operator+=(string str);
  void operator+=(const char* str);
  void operator+=(char* str);
  void operator-=(int dec);
  void operator-=(php_var dec);

  friend ostream &operator<<( ostream &out, const php_var &var );
  void to_array();
  int getint();
  float getfloat();
  bool getbool();
  long getlong();
  string getstring();
  
  string container; // Contains value.
  long int_container;
  float float_container;
  vector<php_var> keys;
  vector<php_var> data;
  void *res;
  void *obj;
  int res_type;
  php_var_type type; // Contains current type.
};

bool operator<(int i, php_var v);
bool operator>(int i, php_var v);
php_var operator-(php_var l, php_var r);
float operator/(php_var &l, php_var &r);
php_var operator*(php_var l, php_var r);

#define OBJECT(var, type) ((type) *) (var).obj
#endif
