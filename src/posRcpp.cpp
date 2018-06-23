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
List posRcpp(std::string text, std::string sys_dic, std::string user_dic) {

  // basic MeCab tagger

  mecab_t* mecab;
  const mecab_node_t* node;

  std::vector<std::string> args;
  args.push_back("mecab");
  if (sys_dic != "") {
    args.push_back("-d");
    args.push_back(sys_dic);
  }
  if (user_dic != "") {
    args.push_back("-u");
    args.push_back(user_dic);
  }

  char** argv_model = new char*[args.size()];
  for(size_t i = 0; i < args.size(); ++i) {
    argv_model[i] = new char[args[i].size() + 1];
    std::strcpy(argv_model[i], args[i].c_str());
  }

  // Create MeCab object
  mecab = mecab_new(args.size(), argv_model);
  CHECK(mecab);

  // Create Node object
  node = mecab_sparse_tonode(mecab, text.c_str());
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
  result.push_back(parsed_string);

  String result_name;
  result_name.push_back(text);
  result_name.set_encoding(CE_UTF8);
  result.names() = result_name;

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;
  mecab_destroy(mecab);

  return result;
}

// [[Rcpp::export]]
List posJoinRcpp(std::string text, std::string sys_dic, std::string user_dic) {

  // basic MeCab tagger
  mecab_t* mecab;
  const mecab_node_t* node;

  std::vector<std::string> args;
  args.push_back("mecab");
  if (sys_dic != "") {
    args.push_back("-d");
    args.push_back(sys_dic);
  }
  if (user_dic != "") {
    args.push_back("-u");
    args.push_back(user_dic);
  }

  char** argv_model = new char*[args.size()];
  for(size_t i = 0; i < args.size(); ++i) {
    argv_model[i] = new char[args[i].size() + 1];
    std::strcpy(argv_model[i], args[i].c_str());
  }

  // Create MeCab object
  mecab = mecab_new(args.size(), argv_model);
  CHECK(mecab);

  // Create Node object
  node = mecab_sparse_tonode(mecab, text.c_str());
  CHECK(node);

  List result;
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

  result.push_back(parsed_string);
  String result_name = text;
  result_name.set_encoding(CE_UTF8);
  result.names() = result_name;

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;
  mecab_destroy(mecab);

  return result;
}
