// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../inst/include/mecab.h"

using namespace Rcpp;

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
List posLoopRcpp(std::vector< std::string > text, std::string sys_dic, std::string user_dic) {

  // lattice model
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
  MeCab::Model * model = MeCab::createModel(argv.size() - 1 , argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  MeCab::Tagger * tagger = model->createTagger();
  MeCab::Lattice * lattice = model->createLattice();

  for (it = text.begin(); it != text.end(); ++it) {

    StringVector parsed_string;
    StringVector parsed_tagset;

    // Gets tagged result in string
    lattice->set_sentence((*it).c_str());
    tagger->parse(lattice);
    const MeCab::Node * node = lattice->bos_node();

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

  delete lattice;
  delete tagger;
  delete model;

  return result;
}

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
List posLoopJoinRcpp(std::vector< std::string > text, std::string sys_dic, std::string user_dic) {

  // lattice model
  std::vector< std::string >::iterator it;
  String parsed_morph;
  List result;

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab model
  MeCab::Model * model = MeCab::createModel(argv.size() - 1 , argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  MeCab::Tagger * tagger = model->createTagger();
  MeCab::Lattice * lattice = model->createLattice();

  for (it = text.begin(); it != text.end(); ++it) {

    StringVector parsed_string;

    // Gets tagged result in string
    lattice->set_sentence((*it).c_str());
    tagger->parse(lattice);
    const MeCab::Node * node = lattice->bos_node();

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

  delete lattice;
  delete tagger;
  delete model;

  return result;
}

// [[Rcpp::interfaces(r, cpp)]]
// [[Rcpp::export]]
DataFrame posDFRcpp(StringVector text, std::string sys_dic, std::string user_dic) {

  // lattice model
  StringVector::iterator it;

  IntegerVector doc_id;
  IntegerVector sentence_id;
  IntegerVector token_id;
  CharacterVector token;
  CharacterVector pos;
  CharacterVector subtype;
  CharacterVector analytic;

  String token_t;
  String pos_t;
  String subtype_t;
  String analytic_t;

  int doc_number = 1;
  int sentence_number = 1;
  int token_number = 1;

  std::vector<std::string> arguments = {"--dicdir", sys_dic, "--userdic", user_dic};

  std::vector<char*> argv;
  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  // Create MeCab model
  MeCab::Model * model = MeCab::createModel(argv.size() - 1 , argv.data());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  MeCab::Tagger * tagger = model->createTagger();
  MeCab::Lattice * lattice = model->createLattice();

  for (it = text.begin(); it != text.end(); ++it) {

    // Gets tagged result in string
    lattice->set_sentence(as<const char*>(*it));
    tagger->parse(lattice);
    const MeCab::Node * node = lattice->bos_node();

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

        // append doc_id as int
        doc_id.push_back(doc_number);
      }
    }
    sentence_number = 1;
    token_number = 1;
    doc_number++;
  }

  // doc_id to factor
  doc_id.attr("class") = "factor";
  doc_id.attr("levels") = text;

  // gc
  delete lattice;
  delete tagger;
  delete model;

  return DataFrame::create(Named("doc_id")=doc_id, Named("sentence_id")=sentence_id, Named("token_id")=token_id, Named("token")=token, Named("pos")=pos, Named("subtype")=subtype, Named("analytic")=analytic);
}
