// Copyright 2011, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)

#pragma once

#include <sys/timeb.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

#include "./StringUtils.h"

#ifndef LOGLEVEL
#define LOGLEVEL 3
#endif

#define LOG(x)      \
  if (x > LOGLEVEL) \
    ;               \
  else              \
    ad_utility::Log::getLog<x>()  // NOLINT
#define AD_POS_IN_CODE                                                     \
  '[' << ad_utility::getLastPartOfString(__FILE__, '/') << ':' << __LINE__ \
      << "] "  // NOLINT

#define TRACE 5
#define DEBUG 4
#define INFO 3
#define WARN 2
#define ERROR 1
#define FATAL 0

namespace ad_utility {

using std::string;
//! Helper class to get thousandth separators in a locale
class CommaNumPunct : public std::numpunct<char> {
 protected:
  virtual char do_thousands_sep() const { return ','; }

  virtual std::string do_grouping() const { return "\03"; }
};

const static std::locale commaLocale(std::locale(), new CommaNumPunct());

//! String representation of a double with precision and thousandth separators
inline string to_string(double in, size_t precision) {
  std::ostringstream buffer;
  buffer.imbue(commaLocale);
  buffer << std::setprecision(precision) << std::fixed << in;
  return buffer.str();
}

//! String representation of a long with thousandth separators
inline string to_string(long in) {
  std::ostringstream buffer;
  buffer.imbue(commaLocale);
  buffer << in;
  return buffer.str();
}

//! Log
class Log {
 public:
  template <unsigned char LEVEL>
  static std::ostream& getLog() {
    return std::cout << ad_utility::Log::getTimeStamp() << "\t- "
                     << ad_utility::Log::getLevel<LEVEL>();
  }

  static void imbue(const std::locale& locale) { std::cout.imbue(locale); }

  static string getTimeStamp() {
    struct timeb timebuffer;
    char timeline[26];

    ftime(&timebuffer);
    ctime_r(&timebuffer.time, timeline);
    timeline[19] = '.';
    timeline[20] = 0;

    std::ostringstream os;
    os << timeline;
    os.fill('0');
    os.width(3);
    os << timebuffer.millitm;
    return os.str();
  }

  template <unsigned char LEVEL>
  static string getLevel() {
    if (LEVEL == TRACE) {
      return "TRACE: ";
    }
    if (LEVEL == DEBUG) {
      return "DEBUG: ";
    }
    if (LEVEL == INFO) {
      return "INFO:  ";
    }
    if (LEVEL == WARN) {
      return "WARN:  ";
    }
    if (LEVEL == ERROR) {
      return "ERROR: ";
    }
    if (LEVEL == FATAL) {
      return "FATAL: ";
    }
  }
};
}  // namespace ad_utility
