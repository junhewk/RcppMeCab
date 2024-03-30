// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../inst/include/mecab.h"

using namespace Rcpp;

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
CharacterVector posRcpp(std::string text, std::string sys_dic, std::string user_dic) {

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab tagger
  MeCab::Tagger *tagger = MeCab::createTagger(argv.size() - 1, argv.data());
  if (!tagger) {
    Rcerr << "model is NULL" << std::endl;
    delete tagger;
    return R_NilValue;
  }

  // Create Node object
  const MeCab::Node* node = tagger->parseToNode(text.c_str());

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

  delete tagger;

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
  MeCab::Tagger *tagger = MeCab::createTagger(argv.size() - 1, argv.data());
  if (!tagger) {
    Rcerr << "model is NULL" << std::endl;
    delete tagger;
    return R_NilValue;
  }

  // Create Node object
  const MeCab::Node* node = tagger->parseToNode(text.c_str());

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

  delete tagger;

  return parsed_string;
}
