add_library(parser
              SparqlParser.h SparqlParser.cpp
              ParsedQuery.h ParsedQuery.cpp
              ParseException.h
              TsvParser.h TsvParser.cpp
              NTriplesParser.h NTriplesParser.cpp
              TurtleParser.h TurtleParser.cpp
              Tokenizer.h Tokenizer.cpp
              ContextFileParser.cpp ContextFileParser.h
              ParallelParseBuffer.h)
target_link_libraries(parser re2 -lbz2)
