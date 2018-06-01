// [[Rcpp::depends(RcppParallel)]]
// [[Rcpp::plugins(cpp11)]]

#include <Rcpp.h>
#include <RcppParallel.h>
#include <mecab.h>

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
          std::string feature_result = node->feature;
          parsed.push_back(parsed_morph + "/" + feature_result.substr(0, feature_result.find(",")));
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
          std::string feature_result = node->feature;
          parsed.push_back(parsed_morph);
          parsed.push_back(feature_result.substr(0, feature_result.find(",")));
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
List posParallelJoinRcpp( std::vector<std::string> text, std::string dict ) {

  std::vector< std::vector < std::string > > results(text.size());
  List result;

  mecab_model_t* model = mecab_model_new2(dict.c_str());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  // parallel argorithm with Intell TBB
  // RcppParallel doesn't get CharacterVector as input and output
  auto func = TextParseJoin(&text, results, model);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, text.size()), func);

  mecab_model_destroy(model);

  // explicit type conversion
  for (int k = 0; k < results.size(); ++k) {
    CharacterVector resultString;
    for (int l = 0; l < results[k].size(); ++l) {
      Rcpp::String morph_copy; // type conversion to Rcpp::String and set encoding
      morph_copy = results[k][l];
      morph_copy.set_encoding(CE_UTF8);
      resultString.push_back(morph_copy);
    }
    result.push_back(resultString);
  }

  return result;
}

// [[Rcpp::export]]
List posParallelRcpp( std::vector<std::string> text, std::string dict ) {

  std::vector< std::vector < std::string > > results(text.size());
  List result;

  mecab_model_t* model = mecab_model_new2(dict.c_str());
  if (!model) {
    Rcerr << "model is NULL" << std::endl;
    return R_NilValue;
  }

  // parallel argorithm with Intell TBB
  // RcppParallel doesn't get CharacterVector as input and output
  auto func = TextParse(&text, results, model);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, text.size()), func);

  mecab_model_destroy(model);

  // explicit type conversion
  for (int k = 0; k < results.size(); ++k) {
    CharacterVector resultString;
    CharacterVector resultTag;
    for (int l = 0; l < results[k].size(); l += 2) {
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

  return result;
}
