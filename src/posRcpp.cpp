// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "mecab.h"

using namespace Rcpp;

#define CHECK(eval) if (! eval) {                                   \
Rcpp::Rcerr << "Exception: " << mecab_strerror(mecab) << std::endl; \
mecab_destroy(mecab);                                               \
return R_NilValue; }

// [[Rcpp::export]]
StringVector posRcpp(StringVector text, std::string sys_dic, std::string user_dic) {
  // basic MeCab tagger

  std::string input = as<std::string>(text);

  mecab_t* mecab;
  const mecab_node_t* node;

  char arg0[] = "-d";
  char *arg1 = new char[sys_dic.length() + 1];
  strcpy(arg1, sys_dic.c_str());
  char arg2[] = "-u";
  char *arg3 = new char[user_dic.length() + 1];
  strcpy(arg3, user_dic.c_str());
  char* argv_model[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], NULL };
  int argc_model = (int)(sizeof(argv_model) / sizeof(argv_model[0])) - 1;

  // Create MeCab object
  mecab = mecab_new(argc_model, argv_model);
  CHECK(mecab);

  // Create Node object
  node = mecab_sparse_tonode(mecab, input.c_str());
  CHECK(node);

  StringVector result;
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
      result.push_back(parsed_morph);
      tags.push_back(parsed_tag);
    }
  }

  result.names() = tags;

  delete[] arg1;
  delete[] arg3;
  mecab_destroy(mecab);
  return result;
}

// [[Rcpp::export]]
StringVector posJoinRcpp(StringVector text, std::string sys_dic, std::string user_dic) {
  // basic MeCab tagger

  std::string input = as<std::string>(text);

  mecab_t* mecab;
  const mecab_node_t* node;

  char arg0[] = "-d";
  char *arg1 = new char[sys_dic.length() + 1];
  strcpy(arg1, sys_dic.c_str());
  char arg2[] = "-u";
  char *arg3 = new char[user_dic.length() + 1];
  strcpy(arg3, user_dic.c_str());
  char* argv_model[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], NULL };
  int argc_model = (int)(sizeof(argv_model) / sizeof(argv_model[0])) - 1;

  // Create MeCab object
  mecab = mecab_new(argc_model, argv_model);
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
      std::vector<std::string> features;
      boost::split(features, node->feature, boost::is_any_of(","));
      parsed_morph.push_back(features[0]);
      parsed_morph.set_encoding(CE_UTF8); // Set Encoding with Rcpp module
      result.push_back(parsed_morph);
    }
  }

  delete[] arg1;
  delete[] arg3;
  mecab_destroy(mecab);
  return result;
}
