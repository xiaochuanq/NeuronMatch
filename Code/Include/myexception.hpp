/*
 * myexception.h
 *
 *  Created on: Mar 8, 2011
 *      Author: xiaochuanq
 */
#ifndef MYEXCPETION_H
#define MYEXCPETION_H
#include <typeinfo> // for exception
#include <string>

struct MyException : public std::exception
{
private:
   std::string s;
public:
   MyException(std::string ss) : s(ss) {}
   virtual ~MyException() throw() {};
   const char* what() const throw() { return s.c_str(); }
};

#endif
