# RcppMeCab [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

This package, RcppMeCab, is a `Rcpp` wrapper for the part-of-speech morphological analyzer `MeCab`. It supports native utf-8 encoding in C++ code and CJK (Chinese, Japanese, and Korean) MeCab library. This package fully utilizes the power `Rcpp` brings `R` computation to analyze texts faster.

## Installation

### Mac OSX, Linux

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
pos(sentence, dict)
pos(sentence, join, dict) # for yield morphemes only (tags will be given on the vector names)
posParallel(sentence, dict) # parallelized version uses more memory
```

+ sentence: a text for analyzing
+ dict: a directory in which `mecabrc` or `dicrc` file is located, default value is ""
+ join: If it gets TRUE, output form is (morpheme/tag). If it gets FALSE, output form is (morpheme) + tag in attribute.

## TODOs

+ Test multilanguage support
+ Provide other useful functions
+ Provide multilanguage manuals for international support

## Author

Junhewk Kim (junhewk.kim@gmail.com)
