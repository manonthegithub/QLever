// Copyright 2011, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold <buchholb>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "engine/Server.h"
#include "util/ReadableNumberFact.h"

using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;

#define EMPH_ON "\033[1m"
#define EMPH_OFF "\033[22m"

// Available options.
struct option options[] = {{"help", no_argument, NULL, 'h'},
                           {"index", required_argument, NULL, 'i'},
                           {"worker-threads", required_argument, NULL, 'j'},
                           {"on-disk-literals", no_argument, NULL, 'l'},
                           {"port", required_argument, NULL, 'p'},
                           {"no-patterns", no_argument, NULL, 'P'},
                           {"text", no_argument, NULL, 't'},
                           {NULL, 0, NULL, 0}};

void printUsage(char* execName) {
  std::ios coutState(nullptr);
  coutState.copyfmt(cout);
  cout << std::setfill(' ') << std::left;

  cout << "Usage: " << execName << " -p <PORT> -i <index> [OPTIONS]" << endl
       << endl;
  cout << "Options" << endl;
  cout << "  " << std::setw(20) << "h, help" << std::setw(1) << "    "
       << "Show this help and exit." << endl;
  cout << "  " << std::setw(20) << "i, index" << std::setw(1) << "    "
       << "The location of the index files." << endl;
  cout << "  " << std::setw(20) << "p, port" << std::setw(1) << "    "
       << "The port on which to run the web interface." << endl;
  cout << "  " << std::setw(20) << "no-patterns" << std::setw(1) << "    "
       << "Disable the use of patterns. This disables ql:has-predicate."
       << endl;
  cout << "  " << std::setw(20) << "t, text" << std::setw(1) << "    "
       << "Enables the usage of text." << endl;
  cout << "  " << std::setw(20) << "j, worker-threads" << std::setw(1) << "    "
       << "Sets the number of worker threads to use" << endl;
  cout.copyfmt(coutState);
}

// Main function.
int main(int argc, char** argv) {
  char* locale = setlocale(LC_CTYPE, "");

  std::locale loc;
  ad_utility::ReadableNumberFacet facet(1);
  std::locale locWithNumberGrouping(loc, &facet);
  ad_utility::Log::imbue(locWithNumberGrouping);

  // Init variables that may or may not be
  // filled / set depending on the options.
  string index = "";
  bool text = false;
  int port = -1;
  int numThreads = 1;
  bool usePatterns = true;

  optind = 1;
  // Process command line arguments.
  while (true) {
    int c = getopt_long(argc, argv, "i:p:j:tauhml", options, NULL);
    if (c == -1) break;
    switch (c) {
      case 'i':
        index = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'P':
        usePatterns = false;
        break;
      case 't':
        text = true;
        break;
      case 'j':
        numThreads = atoi(optarg);
        break;
      case 'h':
        printUsage(argv[0]);
        exit(0);
        break;
      case 'l':
        std::cout << "Warning: the -l flag (onDiskLiterals) is deprecated and "
                     "will be ignored for ServerMain. The correct setting for "
                     "this flag is read directly from the index\n";
        break;
      default:
        cout << endl
             << "! ERROR in processing options (getopt returned '" << c
             << "' = 0x" << std::setbase(16) << static_cast<int>(c) << ")"
             << endl
             << endl;
        printUsage(argv[0]);
        exit(1);
    }
  }

  if (index.size() == 0 || port == -1) {
    if (index.size() == 0) {
      cerr << "ERROR: No index specified, but an index is required." << endl;
    }
    if (port == -1) {
      cerr << "ERROR: No port specified, but the port is required." << endl;
    }
    printUsage(argv[0]);
    exit(1);
  }

  cout << endl
       << EMPH_ON << "ServerMain, version " << __DATE__ << " " << __TIME__
       << EMPH_OFF << endl
       << endl;
  cout << "Set locale LC_CTYPE to: " << locale << endl;

  try {
    Server server(port, numThreads);
    server.initialize(index, text, usePatterns);
    server.run();
  } catch (const std::exception& e) {
    // This code should never be reached as all exceptions should be handled
    // within server.run()
    LOG(ERROR) << e.what() << std::endl;
    return 1;
  }
  // This should also never be reached as the server threads are not supposed
  // to terminate.
  return 2;
}
