// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../inst/include/mecab.h"

using namespace Rcpp;

#define CHECK(eval) if (! eval) {                  \
Rcpp::Rcerr << mecab_strerror(tagger) << std::endl; \
mecab_destroy(tagger);                              \
return R_NilValue; }

// [[Rcpp::interfaces(r, cpp)]]
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

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab model
  model = mecab_model_new(argv.size() - 1, argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  CHECK(tagger);
  lattice = mecab_model_new_lattice(model);
  CHECK(lattice);

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

  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}

// [[Rcpp::interfaces(r, cpp)]]
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

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab model
  model = mecab_model_new(argv.size() - 1, argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  CHECK(tagger);
  lattice = mecab_model_new_lattice(model);
  CHECK(lattice);

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

  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return result;
}

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
DataFrame posDFRcpp(StringVector text, std::string sys_dic, std::string user_dic) {

  // lattice model

  mecab_model_t* model;
  mecab_t* tagger;
  mecab_lattice_t* lattice;
  const mecab_node_t* node;
  StringVector::iterator it;

  StringVector doc_id;
  IntegerVector sentence_id;
  IntegerVector token_id;
  StringVector token;
  StringVector pos;
  StringVector subtype;
  StringVector analytic;

  String doc_id_t;
  String token_t;
  String pos_t;
  String subtype_t;
  String analytic_t;

  int doc_number = 0;
  int sentence_number = 1;
  int token_number = 1;
  StringVector text_names;
  bool b = text.hasAttribute("name");
  if (b == TRUE) {
    text_names = text.names();
  }

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab model
  model = mecab_model_new(argv.size() - 1, argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  tagger = mecab_model_new_tagger(model);
  CHECK(tagger);
  lattice = mecab_model_new_lattice(model);
  CHECK(lattice);

  for (it = text.begin(); it != text.end(); ++it) {

    mecab_lattice_set_sentence(lattice, as<const char*>(*it));
    mecab_parse_lattice(tagger, lattice);
    node = mecab_lattice_get_bos_node(lattice);

    for (; node; node = node->next) {
      if (node->stat == MECAB_BOS_NODE)
        ;
      else if (node->stat == MECAB_EOS_NODE)
        ;
      else {
        std::vector<std::string> features;
        boost::split(features, node->feature, boost::is_any_of(","));

        token_t = std::string(node->surface).substr(0, node->length);
        pos_t = features[0];
        subtype_t = features[1];
        // For parsing unk-feature when using Japanese MeCab and IPA-dict.
        if (features.size() > 7) {
          analytic_t = features[7];
        } else {
          analytic_t = "";
        }

        if (subtype_t == "*") {
          subtype_t = "";
        }

        if (analytic_t == "*") {
          analytic_t = "";
        }

        token_t.set_encoding(CE_UTF8);
        pos_t.set_encoding(CE_UTF8);
        subtype_t.set_encoding(CE_UTF8);
        analytic_t.set_encoding(CE_UTF8);

        // append token, pos, and subtype
        token.push_back(token_t);
        pos.push_back(pos_t);
        subtype.push_back(subtype_t);
        analytic.push_back(analytic_t);

        token_id.push_back(token_number);
        token_number++;

        // append sentence_id and token_id
        sentence_id.push_back(sentence_number);
        if (token_t == "." or token_t == "。") {
          sentence_number++;
          token_number = 1;
        }

        // append doc_id
        if (b == TRUE) {
          doc_id_t = text_names[doc_number];
          doc_id_t.set_encoding(CE_UTF8);
          doc_id.push_back(doc_id_t);
        } else {
          doc_id.push_back(std::to_string(doc_number + 1));
        }
      }
    }
    sentence_number = 1;
    token_number = 1;
    doc_number++;
  }

  // gc
  mecab_destroy(tagger);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return DataFrame::create(_["doc_id"]=doc_id, _["sentence_id"]=sentence_id, _["token_id"]=token_id, _["token"]=token, _["pos"]=pos, _["subtype"]=subtype, _["analytic"]=analytic);
}
