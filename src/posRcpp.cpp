#include <Rcpp.h>
#include <string>
#include "mecab.h"
using namespace Rcpp;

#define CHECK(eval) if (! eval) {                                   \
Rcpp::Rcerr << "Exception: " << mecab_strerror(mecab) << std::endl; \
mecab_destroy(mecab);                                               \
return R_NilValue; }

// [[Rcpp::export]]
StringVector posRcpp(StringVector text, StringVector dict) {
  // basic MeCab tagger

  std::string input = as<std::string>(text);

  mecab_t* mecab;
  const mecab_node_t* node;

  // Create MeCab object
  mecab = mecab_new2(as<const char *>(dict));
  CHECK(mecab);

  // Create Node object
  node = mecab_sparse_tonode(mecab, input.c_str());
  CHECK(node);

  StringVector result;

  for (; node; node = node->next) {
    if (node->stat == MECAB_BOS_NODE)
      ;
    else if (node->stat == MECAB_EOS_NODE)
      ;
    else {
      String parsed_morph = std::string(node->surface).substr(0, node->length);
      parsed_morph.push_back("/");
      std::string feature_result = node->feature;
      parsed_morph.push_back(feature_result.substr(0, feature_result.find(",")));
      parsed_morph.set_encoding(CE_UTF8); // Set Encoding with Rcpp module
      result.push_back(parsed_morph);
    }
  }

  mecab_destroy(mecab);
  return result;
}
