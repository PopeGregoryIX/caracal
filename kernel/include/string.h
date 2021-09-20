#ifndef __KSTRING_H
#define __KSTRING_H

#include <stdint.h>
#include <cpu.h>

class String
{
public:
    inline String(const char* s)    {*this = s;}
    String(const char* s, size_t len);
    inline String(uint64_t n)       {*this = n;}
    inline String(const String& s)  {*this = s;}
    inline String(const bool b)     {*this = b;}
    inline ~String()                {delete str_;}

    inline size_t length( void ) const  { return len_;}

    //  character array operator
    char operator[](size_t index) const;

    //  Assignment operators
    String& operator=(const char* s);
    String& operator=(uint64_t num);
    String& operator=(const String& s);
    String& operator=(const bool b);

    //  Comparison operators
    inline bool operator==(const char* s) const  {   return(compare(str_, s, (int)len_) == 0);    }
    inline bool operator==(const String& s) const {  return( *this == s.str_);  }
private:
    char* str_ = 0;
    size_t len_ = 0;
public:
    static int compare(const char* s1, const char* s2, int maxLength = 0);
	static char* copy(char* dest, const char* src);
	static char* itoa(uint64_t num, char* buffer);
	static size_t length(const char* s);
private:
	static uint8_t numBase_;
	static const char ltHexUpper_[16];
	static const char ltHexLower_[16];
};

#endif
