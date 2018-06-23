// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "mecab.h"

using namespace Rcpp;

// [[Rcpp::export]]
List posLoopRcpp(std::vector< std::string > text, std::string sys_dic, std::string user_dic) {

  // lattice model

  mecab_model_t* model;
  mecab_t* tagger;
  mecab_lattice_t* lattice;
  const mecab_node_t* node;
  std::vector< std::string >::iterator it;
  String parsed_morph;
  String parsed_tag;
  List result;

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

  // Create MeCab model
  model = mecab_model_new(args.size(), argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  lattice = mecab_model_new_lattice(model);

  for (it = text.begin(); it != text.end(); ++it) {

    StringVector parsed_string;
    StringVector parsed_tagset;

    mecab_lattice_set_sentence(lattice, (*it).c_str());
    mecab_parse_lattice(tagger, lattice);
    node = mecab_lattice_get_bos_node(lattice);

    for (; node; node = node->next) {
      if (node->stat == MECAB_BOS_NODE)
        ;
      else if (node->stat == MECAB_EOS_NODE)
        ;
      else {
        parsed_morph = std::string(node->surface).substr(0, node->length);
        parsed_morph.set_encoding(CE_UTF8);

        std::vector<std::string> features;
        boost::split(features, node->feature, boost::is_any_of(","));
        String parsed_tag = features[0];
        parsed_tag.set_encoding(CE_UTF8);

        parsed_string.push_back(parsed_morph);
        parsed_tagset.push_back(parsed_tag);
      }
    }

    parsed_string.names() = parsed_tagset;
    result.push_back(parsed_string);

  }

  StringVector result_name(text.size());

  for (size_t h = 0; h < text.size(); ++h) {
    String character_name = text[h];
    character_name.set_encoding(CE_UTF8);
    result_name[h] = character_name;
  }

  result.names() = result_name;

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;
  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}

// [[Rcpp::export]]
List posLoopJoinRcpp(std::vector< std::string > text, std::string sys_dic, std::string user_dic) {

  // lattice model

  mecab_model_t* model;
  mecab_t* tagger;
  mecab_lattice_t* lattice;
  const mecab_node_t* node;
  std::vector< std::string >::iterator it;
  String parsed_morph;
  List result;

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

  // Create MeCab model
  model = mecab_model_new(args.size(), argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  lattice = mecab_model_new_lattice(model);

  for (it = text.begin(); it != text.end(); ++it) {

    StringVector parsed_string;

    mecab_lattice_set_sentence(lattice, (*it).c_str());
    mecab_parse_lattice(tagger, lattice);
    node = mecab_lattice_get_bos_node(lattice);

    for (; node; node = node->next) {
      if (node->stat == MECAB_BOS_NODE)
        ;
      else if (node->stat == MECAB_EOS_NODE)
        ;
      else {
        parsed_morph = std::string(node->surface).substr(0, node->length);
        parsed_morph.push_back("/");
        std::vector<std::string> features;
        boost::split(features, node->feature, boost::is_any_of(","));
        parsed_morph.push_back(features[0]);
        parsed_morph.set_encoding(CE_UTF8);
        parsed_string.push_back(parsed_morph);
      }
    }

    result.push_back(parsed_string);

  }

  StringVector result_name(text.size());

  for (size_t h = 0; h < text.size(); ++h) {
    String character_name = text[h];
    character_name.set_encoding(CE_UTF8);
    result_name[h] = character_name;
  }

  result.names() = result_name;

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;
  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}
