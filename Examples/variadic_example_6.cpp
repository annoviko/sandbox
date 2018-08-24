#include <sstream>
#include <string>
#include <iostream>


class MessageBuilder
{
public:
  MessageBuilder()  = delete;

private:
  template<typename TypeArgument>
  static void AppendArgument(std::stringstream & stream, const TypeArgument & argument)
  {
    stream << argument;
  }

  template <typename TypeArgument, typename ... TypeParam>
  static void AppendArgument(std::stringstream & stream, const TypeArgument & argument, const TypeParam & ... param)
  {
    AppendArgument(stream, argument);
    AppendArgument(stream, param ...);
  }

public:
  template <typename ... TypeParam>
  static std::string Create(const TypeParam & ... param)
  {
    std::stringstream stream;
    AppendArgument(stream, param ...);
    return stream.str();
  }
};



int main(int argc, char * argv[])
{
    std::cout << MessageBuilder::Create("Send '", 2, "' pieces and '", 10, "' patches to someone") << std::endl;
    std::cout << MessageBuilder::Create("Hello World") << std::endl;
    return 0;
}
