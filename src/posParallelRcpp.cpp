// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(BH, RcppParallel)]]

#include <Rcpp.h>
#include <RcppParallel.h>
#include <boost/algorithm/string.hpp>
#include "mecab.h"

using namespace Rcpp;

struct TextParseJoin
{
  TextParseJoin(const std::vector<std::string>* sentences, std::vector< std::vector < std::string > >& result, mecab_model_t* model)
    : sentences_(sentences), result_(result), model_(model)
  {}

  void operator()(const tbb::blocked_range<size_t>& range) const
  {
    mecab_t* tagger = mecab_model_new_tagger(model_);
    mecab_lattice_t* lattice = mecab_model_new_lattice(model_);
    const mecab_node_t* node;

    for (size_t i = range.begin(); i < range.end(); ++i) {
      std::vector< std::string > parsed;
      mecab_lattice_set_sentence(lattice, (*sentences_)[i].c_str());
      mecab_parse_lattice(tagger, lattice);
      node = mecab_lattice_get_bos_node(lattice);

      for (; node; node = node->next) {
        if (node->stat == MECAB_BOS_NODE)
          ;
        else if (node->stat == MECAB_EOS_NODE)
          ;
        else {
          std::string parsed_morph = std::string(node->surface).substr(0, node->length);
          std::vector<std::string> features;
          boost::split(features, node->feature, boost::is_any_of(","));
          parsed.push_back(parsed_morph + "/" + features[0]);
        }
      }

      result_[i] = parsed; // mutex is not needed
    }

    mecab_lattice_destroy(lattice);
    mecab_destroy(tagger);
  }

  const std::vector<std::string>* sentences_;
  std::vector< std::vector < std::string > >& result_;
  mecab_model_t* model_;
};

struct TextParseDF
{
  TextParseDF(const std::vector<std::string>* sentences, std::vector< std::vector < std::string > >& result, mecab_model_t* model)
    : sentences_(sentences), result_(result), model_(model)
  {}

  void operator()(const tbb::blocked_range<size_t>& range) const
  {
    mecab_t* tagger = mecab_model_new_tagger(model_);
    mecab_lattice_t* lattice = mecab_model_new_lattice(model_);
    const mecab_node_t* node;

    for (size_t i = range.begin(); i < range.end(); ++i) {
      std::vector< std::string > parsed;

      mecab_lattice_set_sentence(lattice, (*sentences_)[i].c_str());
      mecab_parse_lattice(tagger, lattice);
      node = mecab_lattice_get_bos_node(lattice);

      for (; node; node = node->next) {
        if (node->stat == MECAB_BOS_NODE)
          ;
        else if (node->stat == MECAB_EOS_NODE)
          ;
        else {
          std::string parsed_morph = std::string(node->surface).substr(0, node->length);
          std::vector<std::string> features;
          boost::split(features, node->feature, boost::is_any_of(","));
          parsed.push_back(parsed_morph);
          parsed.push_back(features[0]);
          parsed.push_back(features[1]);
        }
      }

      result_[i] = parsed; // mutex is not needed
    }

    mecab_lattice_destroy(lattice);
    mecab_destroy(tagger);
  }

  const std::vector<std::string>* sentences_;
  std::vector< std::vector < std::string > >& result_;
  mecab_model_t* model_;
};

struct TextParse
{
  TextParse(const std::vector<std::string>* sentences, std::vector< std::vector < std::string > >& result, mecab_model_t* model)
    : sentences_(sentences), result_(result), model_(model)
  {}

  void operator()(const tbb::blocked_range<size_t>& range) const
  {
    mecab_t* tagger = mecab_model_new_tagger(model_);
    mecab_lattice_t* lattice = mecab_model_new_lattice(model_);
    const mecab_node_t* node;

    for (size_t i = range.begin(); i < range.end(); ++i) {
      std::vector< std::string > parsed;

      mecab_lattice_set_sentence(lattice, (*sentences_)[i].c_str());
      mecab_parse_lattice(tagger, lattice);
      node = mecab_lattice_get_bos_node(lattice);

      for (; node; node = node->next) {
        if (node->stat == MECAB_BOS_NODE)
          ;
        else if (node->stat == MECAB_EOS_NODE)
          ;
        else {
          std::string parsed_morph = std::string(node->surface).substr(0, node->length);
          // add join mechanism
          std::vector<std::string> features;
          boost::split(features, node->feature, boost::is_any_of(","));
          parsed.push_back(parsed_morph);
          parsed.push_back(features[0]);
        }
      }

      result_[i] = parsed; // mutex is not needed
    }

    mecab_lattice_destroy(lattice);
    mecab_destroy(tagger);
  }

  const std::vector<std::string>* sentences_;
  std::vector< std::vector < std::string > >& result_;
  mecab_model_t* model_;
};

// [[Rcpp::export]]
List posParallelJoinRcpp( std::vector<std::string> text, std::string sys_dic, std::string user_dic ) {

  std::vector< std::vector < std::string > > results(text.size());
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
  mecab_model_t* model = mecab_model_new(args.size(), argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  // parallel argorithm with Intell TBB
  // RcppParallel doesn't get CharacterVector as input and output
  auto func = TextParseJoin(&text, results, model);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, text.size()), func);

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;

  mecab_model_destroy(model);

  // explicit type conversion
  for (size_t k = 0; k < results.size(); ++k) {
    CharacterVector resultString;
    for (size_t l = 0; l < results[k].size(); ++l) {
      Rcpp::String morph_copy; // type conversion to Rcpp::String and set encoding
      morph_copy = results[k][l];
      morph_copy.set_encoding(CE_UTF8);
      resultString.push_back(morph_copy);
    }
    result.push_back(resultString);
  }

  StringVector result_name(text.size());

  for (size_t h = 0; h < text.size(); ++h) {
    String character_name = text[h];
    character_name.set_encoding(CE_UTF8);
    result_name[h] = character_name;
  }

  result.names() = result_name;

  return result;
}

// [[Rcpp::export]]
DataFrame posParallelDFRcpp( StringVector text, std::string sys_dic, std::string user_dic ) {

  std::vector< std::vector < std::string > > results(text.size());
  std::vector< std::string > input = as<std::vector< std::string > >(text);

  StringVector doc_id;
  IntegerVector sentence_id;
  IntegerVector token_id;
  StringVector token;
  StringVector pos;
  StringVector subtype;

  String doc_id_t;
  String token_t;
  String pos_t;
  String subtype_t;

  int doc_number = 0;
  int sentence_number = 1;
  int token_number = 1;
  StringVector text_names;
  bool b = text.hasAttribute("name");

  if (b == TRUE) {
    text_names = text.names();
  }

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
  mecab_model_t* model = mecab_model_new(args.size(), argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  // parallel argorithm with Intell TBB
  // RcppParallel doesn't get CharacterVector as input and output
  auto func = TextParseDF(&input, results, model);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, input.size()), func);

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;

  mecab_model_destroy(model);

  // explicit type conversion
  for (size_t k = 0; k < results.size(); ++k) {
    for (size_t l = 0; l < results[k].size(); l += 3) {
      token_t = results[k][l];
      pos_t = results[k][l + 1];
      subtype_t = results[k][l + 2];

      if (subtype_t == "*") {
        subtype_t = "";
      }

      token_t.set_encoding(CE_UTF8);
      pos_t.set_encoding(CE_UTF8);
      subtype_t.set_encoding(CE_UTF8);

      token.push_back(token_t);
      pos.push_back(pos_t);
      subtype.push_back(subtype_t);

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
    sentence_number = 1;
    token_number = 1;
    doc_number++;
  }

  return DataFrame::create(_["doc_id"]=doc_id, _["sentence_id"]=sentence_id, _["token_id"]=token_id, _["token"]=token, _["pos"]=pos, _["subtype"]=subtype);
}

// [[Rcpp::export]]
List posParallelRcpp( std::vector<std::string> text, std::string sys_dic, std::string user_dic ) {

  std::vector< std::vector < std::string > > results(text.size());
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
  mecab_model_t* model = mecab_model_new(args.size(), argv_model);
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  // parallel argorithm with Intell TBB
  // RcppParallel doesn't get CharacterVector as input and output
  auto func = TextParse(&text, results, model);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, text.size()), func);

  for(size_t i = 0; i < args.size(); i++){
    delete [] argv_model[i];
  }
  delete [] argv_model;

  mecab_model_destroy(model);

  // explicit type conversion
  for (size_t k = 0; k < results.size(); ++k) {
    CharacterVector resultString;
    CharacterVector resultTag;
    for (size_t l = 0; l < results[k].size(); l += 2) {
      Rcpp::String morph_copy; // type conversion to Rcpp::String and set encoding
      Rcpp::String tags_copy;
      morph_copy = results[k][l];
      tags_copy = results[k][l + 1];
      morph_copy.set_encoding(CE_UTF8);
      tags_copy.set_encoding(CE_UTF8);
      resultString.push_back(morph_copy);
      resultTag.push_back(tags_copy);
    }
    resultString.names() = resultTag;
    result.push_back(resultString);
  }

  StringVector result_name(text.size());

  for (size_t h = 0; h < text.size(); ++h) {
    String character_name = text[h];
    character_name.set_encoding(CE_UTF8);
    result_name[h] = character_name;
  }

  result.names() = result_name;

  return result;
}
