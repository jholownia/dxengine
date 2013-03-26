/*
================================
 common.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

/*
================================
 
 Uncopyable

 Disables copy and assignment constructors which are silently created
 by the compiler. We don't need them in most cases here. Using this 
 class will give link time error if we try to use copy or assign.
 See Meyers "Effective C++" item 6 for more details.

================================
*/
class Uncopyable
{
protected:
    Uncopyable() { }
    ~Uncopyable() { }

private:
    Uncopyable(const Uncopyable&);
    const Uncopyable& operator=(const Uncopyable&);
};