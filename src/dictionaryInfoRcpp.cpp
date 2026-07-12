#include <Rcpp.h>
#include <string>
#include <vector>
#include "../inst/include/mecab.h"

using namespace Rcpp;

// [[Rcpp::export]]
DataFrame dictionaryInfoRcpp(std::string sys_dic, std::string user_dic) {
  std::vector<std::string> arguments = {"mecab"};
  if (!sys_dic.empty()) {
    arguments.push_back("-r");
    arguments.push_back(sys_dic + "/dicrc");
    arguments.push_back("-d");
    arguments.push_back(sys_dic);
  }
  if (!user_dic.empty()) {
    arguments.push_back("-u");
    arguments.push_back(user_dic);
  }

  std::vector<char*> argv;
  for (auto& argument : arguments) {
    argv.push_back(&argument[0]);
  }
  argv.push_back(nullptr);

  MeCab::Model* model = MeCab::createModel(
    static_cast<int>(argv.size() - 1), argv.data()
  );
  if (!model) {
    stop("Unable to load the requested MeCab dictionary: %s",
         MeCab::getLastError());
  }

  CharacterVector filename;
  CharacterVector charset;
  CharacterVector type;
  NumericVector size;
  IntegerVector left_size;
  IntegerVector right_size;
  IntegerVector version;

  for (const MeCab::DictionaryInfo* info = model->dictionary_info();
       info != nullptr; info = info->next) {
    String filename_value = info->filename ? info->filename : "";
    String charset_value = info->charset ? info->charset : "";
    filename_value.set_encoding(CE_UTF8);
    charset_value.set_encoding(CE_UTF8);

    filename.push_back(filename_value);
    charset.push_back(charset_value);
    switch (info->type) {
    case MECAB_SYS_DIC:
      type.push_back("system");
      break;
    case MECAB_USR_DIC:
      type.push_back("user");
      break;
    case MECAB_UNK_DIC:
      type.push_back("unknown");
      break;
    default:
      type.push_back("other");
    }
    size.push_back(static_cast<double>(info->size));
    left_size.push_back(info->lsize);
    right_size.push_back(info->rsize);
    version.push_back(info->version);
  }

  delete model;

  return DataFrame::create(
    Named("filename") = filename,
    Named("charset") = charset,
    Named("type") = type,
    Named("size") = size,
    Named("left_size") = left_size,
    Named("right_size") = right_size,
    Named("version") = version
  );
}
