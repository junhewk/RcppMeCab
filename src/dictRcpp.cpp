#include <Rcpp.h>
#include <string>
#include "../inst/include/mecab.h"

using namespace Rcpp;

// [[Rcpp::export]]
int dictIndexRcpp(std::vector<std::string> args) {
  std::vector<char*> argv;
  for (auto& arg : args)
    argv.push_back(&arg[0]);
  argv.push_back(nullptr);

  int result = mecab_dict_index(static_cast<int>(argv.size() - 1), argv.data());

  if (result != 0) {
    stop("mecab_dict_index failed (return code: %d)", result);
  }

  return result;
}
