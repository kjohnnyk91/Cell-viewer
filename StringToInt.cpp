#include <iostream>

#include "stringToInt.hpp"

int stringToInt(std::string str)
{
	int n = 0;
	int p = 1;
	
	for(std::string::reverse_iterator rit = str.rbegin(); rit != str.rend(); rit++)
	{
		if(((*rit >= '0') and (*rit <= '9')) or (*rit == '-'))
		{
			if(*rit != '-')
			{
				n += (*rit - '0')*p;
				p *= 10;
			}
		}
		else
		{			
			throw *rit;
		}
	}
	
	if(*(str.begin()) == '-')
	{
		n *= -1;
	}
	
	return n;
}