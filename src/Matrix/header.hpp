#pragma once
#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <list>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <sstream>
#include <ranges>
#include <memory>
#include <set>
#include <stdlib.h>
#include <immintrin.h>
#include <cstddef>
#include <cstdint>

#if __cplusplus > 199711L
#include <unordered_map>
#include <random>
#endif

#define pfunc __PRETTY_FUNCTION__
#define debug_log

#ifdef debug_log
#define func_log std::clog << __PRETTY_FUNCTION__ << std::endl
#define class_func_log std::clog << "[ " << this << " ] " << __PRETTY_FUNCTION__ << std::endl
#else
#define func_log
#define class_func_log
#endif

void caret(std::string msg)
{
	const char c = '~';
	std::cout << "\n" << std::string(60, c) << "\n"
	<< msg << "\n" << std::string(60, c) << "\n";
}

#endif
