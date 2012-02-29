#include <cstdio>
#include "document.hpp"

namespace Sass {
  using namespace Prelexer;

  Document::Document(char* _path, char* _source) {
    path = _path;
    if (!_source) {
      std::FILE *f;
      // TO DO: CHECK f AGAINST NULL/0
      f = std::fopen(path, "rb");
      std::fseek(f, 0, SEEK_END);
      int len = std::ftell(f);
      std::rewind(f);
      // TO DO: WRAP THE new[] IN A TRY/CATCH BLOCK
      source = new char[len + 1];
      std::fread(source, sizeof(char), len, f);
      source[len] = '\0';
      std::fclose(f);
    }
    else {
      source = _source;
    }
    position = source;
    line_number = 1;
    last_munch_succeeded = false;
  }
  Document::~Document() {
    delete [] source;
  }
  
  void Document::parse_stylesheet() {
    printf("ABOUT TO MUNCH LEADING SPACES\n");
    try_munching<optional_spaces>();
    while (*position) {
      printf("LOOPING OVER STATEMENTS\n");
      statements.push_back(parse_statement());
      try_munching<optional_spaces>();
    }
  }
  
  Node Document::parse_statement() {
    if (try_munching<block_comment>()) {
      printf("MUNCHING A COMMENT\n");
      return Node(Node::comment, top);
    }
    else return Node();
  }
    
}