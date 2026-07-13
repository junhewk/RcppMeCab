// Portable console entry point for the temporary Windows dictionary compiler.
// The upstream executable source includes a Visual Studio WinMain wrapper that
// is not compatible with the MinGW toolchain used by Rtools.
#include "mecab.h"

int main(int argc, char **argv) {
  return mecab_dict_index(argc, argv);
}
