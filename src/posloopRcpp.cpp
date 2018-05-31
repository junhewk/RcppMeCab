#include <Rcpp.h>
#include <string>
#include "mecab.h"

using namespace Rcpp;

// [[Rcpp::export]]
List posLoopRcpp(std::vector< std::string > text, std::string dict) {

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

  // Create MeCab model
  model = mecab_model_new2( dict.c_str() );
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
        f = node->feature;
        parsed_morph = std::string(node->surface).substr(0, node->length);
        parsed_morph.set_encoding(CE_UTF8);
        parsed_morph.push_back("/");
        parsed_tag = f.substr(0, f.find(","));
        parsed_tag.set_encoding(CE_UTF8);
        parsed_morph.push_back(parsed_tag);
        parsed_string.push_back(parsed_morph);
      }
    }

    result.push_back(parsed_string);

  }

  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}
