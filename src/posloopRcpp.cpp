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
  std::string f;
  StringVector parsed_string;
  StringVector parsed_tagset;
  List result;

  char arg0[] = "-d";
  char *arg1 = new char[sys_dic.length() + 1];
  strcpy(arg1, sys_dic.c_str());
  char arg2[] = "-u";
  char *arg3 = new char[user_dic.length() + 1];
  strcpy(arg3, user_dic.c_str());
  char* argv_model[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], NULL };
  int argc_model = (int)(sizeof(argv_model) / sizeof(argv_model[0])) - 1;

  // Create MeCab model
  model = mecab_model_new(argc_model, argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  lattice = mecab_model_new_lattice(model);

  for (it = text.begin(); it != text.end(); ++it) {

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

  delete[] arg1;
  delete[] arg3;

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
  std::string f;
  StringVector parsed_string;
  List result;

  char arg0[] = "-d";
  char *arg1 = new char[sys_dic.length() + 1];
  strcpy(arg1, sys_dic.c_str());
  char arg2[] = "-u";
  char *arg3 = new char[user_dic.length() + 1];
  strcpy(arg3, user_dic.c_str());
  char* argv_model[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], NULL };
  int argc_model = (int)(sizeof(argv_model) / sizeof(argv_model[0])) - 1;

  // Create MeCab model
  model = mecab_model_new(argc_model, argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  lattice = mecab_model_new_lattice(model);

  for (it = text.begin(); it != text.end(); ++it) {

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

  delete[] arg1;
  delete[] arg3;

  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}
