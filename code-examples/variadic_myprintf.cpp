#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>


void myprintf(const char * p_message) {
  fwrite(p_message, sizeof(char), strlen(p_message), stdout);
}


void print_argument(const char * p_value) {
  myprintf(p_value);
}


template <typename TypeValue>
void print_argument(const char * p_type, TypeValue p_value) {
  //char buffer[128];
  //sprintf(buffer, p_type, p_value);
  std::stringstream stream;
  stream << p_value;
  fwrite(stream.str().c_str(), sizeof(char), stream.str().length(), stdout);
}


template <typename TypeValue>
int print_message(int p_pos, const char * p_message, const TypeValue & p_value) {
  const size_t message_length = strlen(p_message);

  bool escape = false;
  bool processed = false;

  for (; p_pos < message_length; p_pos++) {
    if (p_message[p_pos] == '\\') {
      escape = true;
      continue;
    }

    if (p_message[p_pos] == '%' && escape != true) {
      if (processed) {
        return p_pos;
      }

      if (p_pos < message_length) {
        p_pos++;

        char type_value[3];
        type_value[0] = '%';
        type_value[1] = p_message[p_pos];
        type_value[2] = '\0';

        print_argument(type_value, p_value);

        processed = true;
      }
    }
    else {
      putc(p_message[p_pos], stdout);
    }
  }

  return p_pos;
}


template <typename TypeValue, typename ... TypeParam>
int print_message(int p_pos, const char * p_message, const TypeValue & p_value, const TypeParam & ... p_param) {
  int position = print_message(p_pos, p_message, p_value);
  print_message(position, p_message, p_param...);
  return 0;
}


template <typename ... TypeParam>
void myprintf(const char * p_message, const TypeParam & ... p_param) {
  print_message(0, p_message, p_param...);
}


int main(int argc, char *argv[]) {
  myprintf("Hello '%d' '%f'!", 10, 1.4);
  getchar();
}