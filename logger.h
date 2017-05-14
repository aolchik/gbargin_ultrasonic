
#define NONE  0
#define DEBUG 2

#ifndef Logger_H
#define Logger_H

class Logger {
public:
  Logger();
  void set_level(int);
  void disable();
  void debug(char []);
  void ident();
  void print_identation();
  void unident();

private:
  int level;
  int ident_level;
};

#endif
