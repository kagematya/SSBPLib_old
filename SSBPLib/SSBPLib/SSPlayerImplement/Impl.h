#pragma once

//SS5Player.cppの中身を移しただけ
#include <string>
#include <vector>
#include <stdarg.h>

#pragma warning(disable : 4996)

namespace ss{

	
// printf 形式のフォーマット
static std::string Format(const char* format, ...){

	static std::vector<char> tmp(1000);

	va_list args, source;
	va_start(args, format);
	va_copy( source , args );

	while (1)
	{
		va_copy( args , source );
		//Windows
		if (_vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
		{
			tmp.resize(tmp.size() * 2);
		}
		else
		{
			break;
		}
	}
	tmp.push_back('\0');
	std::string ret = &(tmp[0]);
	va_end(args);
	return ret;
}



} //namespace ss
