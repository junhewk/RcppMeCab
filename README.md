# RcppMeCab [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

This package, RcppMeCab, is a `Rcpp` wrapper for the part-of-speech morphological analyzer `MeCab`. It supports native utf-8 encoding in C++ code and CJK (Chinese, Japanese, and Korean) MeCab library. This package fully utilizes the power `Rcpp` brings `R` computation to analyze texts faster.

## Installation

### Linux and Mac OSX

First, install `MeCab` of your language-of-choice.

+ Japanese: `MeCab` from [github](http://taku910.github.io/mecab/)
+ Korean: `MeCab-Ko` from [Bitbucket repository](https://bitbucket.org/eunjeon/mecab-ko)
+ Chinese: `MeCab` and `MeCab Chinese Dic` from [MeCab-Chinese](http://www.52nlp.cn/%E7%94%A8mecab%E6%89%93%E9%80%A0%E4%B8%80%E5%A5%97%E5%AE%9E%E7%94%A8%E7%9A%84%E4%B8%AD%E6%96%87%E5%88%86%E8%AF%8D%E7%B3%BB%E7%BB%9F%E4%B8%89%EF%BC%9Amecab-chinese)

Second, you can install RcppMeCab from github with:

```
# install.packages("devtools")
devtools::install_github("junhewk/RcppMeCab")
```

### Windows

You should download DLLs applicable for your language.

+ [mecab_ko.zip](https://github.com/junhewk/RcppMeCab/releases/download/0.0.1/mecab_ko.zip) (for Korean)
+ [mecab_jp.zip](https://github.com/junhewk/RcppMeCab/releases/download/0.0.1/mecab_jp.zip) (for Japanese and other languages)

Uncompress DLLs in `~\mecab` (for example, `C:\Users\User\mecab`). And, install `RcppMeCab` package from Github.

```
# install.packages("devtools")
devtools::install_github("junhewk/RcppMecab")
```

For analyzing, you need a MeCab dictionary.

For Korean:

Install [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) and [mecab-ko-dic-msvc](https://github.com/Pusnow/mecab-ko-dic-msvc) up to your 32-bit or 64-bit Windows version in `C:\mecab`. Provide directory location to `RcppMeCab` function.

For Japanese:

Install [mecab binary](https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7WElGUGt6ejlpVXc). Provide directory location to `RcppMeCab` function. For example: `pos(sentence, "C:/PROGRA~2/mecab/dic/ipadic")`

## Usage

This package has `pos` and `posParallel` function.

```
pos(sentence)
pos(sentence, join = FALSE) # for yield morphemes only (tags will be given on the vector names)
pos(sentence, user_dic) # gets a compiled user dictionary 
posParallel(sentence, user_dic) # parallelized version uses more memory
```

+ sentence: a text for analyzing
+ sys_dic: a directory in which `dicrc` file is located, default value is ""
+ user_dic: a user dictionary file compiled by `mecab_dict_index`, default value is also ""
+ join: If it gets TRUE, output form is (morpheme/tag). If it gets FALSE, output form is (morpheme) + tag in attribute.

## Compiling User Dictionary

MeCab API has `DictionaryCompiler`, but it contains `die()`. Hence, calling it in Rcpp crashes down entire R session. This will not be included in `RcppMeCab` functions.

Please refer to [Mecab](http://taku910.github.io/mecab/dic.html) for Japanese.

### Unix and Mac OSX

You should have `model_file` if you want the library to estimate cost automatically. 

+ Japanese: [model_file in ipadic](https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7bnc5aFZSTE9qNnM)
+ Korean: `model.bin` in [mecab-ko-dic](https://bitbucket.org/eunjeon/mecab-ko-dic)

You need entire `mecab-ko-dic` source if you want to compile Korean user dictionary. User dictionary should also be prepared in CSV file. CSV structure is found in [Japanese](http://taku910.github.io/mecab/dic.html) and [Korean](https://bitbucket.org/eunjeon/mecab-ko-dic/src/e39e16059b8748c2663ab09195a08293c7063a28/final/user-dic/README.md?fileviewer=file-view-default).

Compile:

```
$ /usr/local/libexec/mecab/mecab-dict-index -m `model_file` -d `mecab_dic_location` -u `user_dictionary_file_name` -f `CSV file charset` -t `original dictionary charset` `target_csv

# example

$ /usr/local/libexec/mecab/mecab-dict-index -m /usr/local/lib/mecab/dic/mecab-ko-dic/model.bin -d ~/mecab-ko-dic-2.0.3-20170922 -u userdic.dic -f utf8 -t utf8 ~/person.csv
```

### Windows

+ Korean: `mecab-ko-msvc` has `mecab-dict-index.exe`.
+ Japanese: `MeCab` binary version has `mecab-dict-index.exe`.

You can use it in the same way Linux compiles the dictionary.

## TODOs

+ Test multilanguage support
+ Provide other useful functions
+ Provide multilanguage manuals for international support

## Author

Junhewk Kim (junhewk.kim@gmail.com)
