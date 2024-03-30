// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../inst/include/mecab.h"

using namespace Rcpp;

#define CHECK(eval) if (! eval) { \
  Rcpp::Rcerr << mecab_strerror(mecab) << std::endl; \
  mecab_destroy(mecab); \
  return R_NilValue; }

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
CharacterVector posRcpp(std::string text, std::string sys_dic, std::string user_dic) {

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab tagger
  mecab_t * mecab = mecab_new(argv.size() - 1, argv.data());
  CHECK(mecab);

  // Create Node object
  const mecab_node_t * node = mecab_sparse_tonode(mecab, text.c_str());
  CHECK(node);

  List result;
  StringVector parsed_string;
  StringVector tags;

  for (; node; node = node->next) {
    if (node->stat == MECAB_BOS_NODE)
      ;
    else if (node->stat == MECAB_EOS_NODE)
      ;
    else {
      String parsed_morph = std::string(node->surface).substr(0, node->length);
      parsed_morph.set_encoding(CE_UTF8); // Set Encoding with Rcpp module
      std::vector<std::string> features;
      boost::split(features, node->feature, boost::is_any_of(","));
      String parsed_tag = features[0];
      parsed_tag.set_encoding(CE_UTF8);
      parsed_string.push_back(parsed_morph);
      tags.push_back(parsed_tag);
    }
  }

  parsed_string.names() = tags;

  mecab_destroy(mecab);
  return parsed_string;
}

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
StringVector posJoinRcpp(std::string text, std::string sys_dic, std::string user_dic) {

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab tagger
  mecab_t * mecab = mecab_new(argv.size() - 1, argv.data());
  CHECK(mecab);

  // Create Node object
  const mecab_node_t * node = mecab_sparse_tonode(mecab, text.c_str());
  CHECK(node);

  StringVector parsed_string;

  for (; node; node = node->next) {
    if (node->stat == MECAB_BOS_NODE)
      ;
    else if (node->stat == MECAB_EOS_NODE)
      ;
    else {
      String parsed_morph = std::string(node->surface).substr(0, node->length);
      parsed_morph.push_back("/");
      std::vector<std::string> features;
      boost::split(features, node->feature, boost::is_any_of(","));
      parsed_morph.push_back(features[0]);
      parsed_morph.set_encoding(CE_UTF8); // Set Encoding with Rcpp module
      parsed_string.push_back(parsed_morph);
    }
  }

  mecab_destroy(mecab);

  return parsed_string;
}
