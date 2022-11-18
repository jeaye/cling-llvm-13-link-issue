#include <iostream>
#include <array>

#include <cling/Interpreter/Interpreter.h>
#include <cling/Interpreter/Value.h>

#include "memory_pool.hpp"

int main(int const argc, char const **argv)
{
  std::array<char const*, 2> cling_args{ argv[0], "-std=c++17" };
  cling::Interpreter jit(cling_args.size(), cling_args.data(), LLVMDIR);

  jit.AddIncludePath("include");
  jit.process("#include \"memory_pool.hpp\"");
  jit.process("struct bar : pool_item_base<bar>{ }; bar b;");
}
