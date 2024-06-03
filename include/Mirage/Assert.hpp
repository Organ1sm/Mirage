#pragma once

#include <cassert>
#include <iostream>

#ifndef MIRAGE_ASSERT
#define MIRAGE_ASSERT(x, expr) assert(((void)(expr), (x)))
#endif

#ifndef MIRAGE_LOG
#define MIRAGE_LOG(...) std::cout << __VA_ARGS__ << std::endl;
#endif