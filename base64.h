#ifndef BASE64H
#define BASE64H

#ifdef __BORLANDC__
#  pragma pack(push, 8)
#endif
	#include <string>
    #include <iostream>
#ifdef __BORLANDC__
#  pragma pack(pop)
#endif

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif // BASE64H
