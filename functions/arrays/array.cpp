#include <stdarg.h>
php_var array(int key = 0, ...)
{
	va_list ap;
	va_start(ap, key);
	php_var arr;
	arr.to_array();
	int i = 0;
	for(int i = 0; i < key; ++i)
	{
		php_var *temp = (php_var *) va_arg(ap, php_var *);
		php_var key2;
		if(temp == NULL)
		{
			bool found = false;
			for(;;)
			{
				for(int z = 0; z < arr.keys.size(); ++z)
				{
					if(arr.keys[z] == (php_var) i)
						found = true;
				}
				if(found)
					++i;
				else
					break;
			}
			key2 = i;
		}
		else
			key2 = *temp;
		php_var val = *((php_var *) va_arg(ap, php_var *));
		arr.keys.push_back(key2);
		arr.data.push_back(val);
	};
	va_end(ap);
	return arr;
}
