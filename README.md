# RcppMeCab

[![License](https://img.shields.io/badge/license-GPL-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl.html)
![R](https://img.shields.io/github/r-package/v/junhewk/RcppMeCab)
[![CRAN](http://www.r-pkg.org/badges/version/RcppMeCab)](https://cran.r-project.org/package=RcppMeCab)
[![Downloads](http://cranlogs.r-pkg.org/badges/RcppMeCab?color=brightgreen)](http://www.r-pkg.org/pkg/RcppMeCab)
[![R-CMD-check](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml/badge.svg)](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml)

This package, RcppMeCab, is a `Rcpp` wrapper for the part-of-speech morphological analyzer `MeCab`. It supports native utf-8 encoding in C++ code and CJK (Chinese, Japanese, and Korean) MeCab library. This package fully utilizes the power `Rcpp` brings `R` computation to analyze texts faster.

__Please see [this](README_kr.md) for easy installation and usage examples in Korean.__

## Installation

### Linux, macOS, and Windows

RcppMeCab automatically downloads and builds MeCab from source if it is not already installed on your system. No manual MeCab installation is required.

```r
install.packages("RcppMeCab") # install from CRAN

# or install the development version
# install.packages("devtools")
devtools::install_github("junhewk/RcppMeCab")
```

If you already have MeCab installed (e.g. via `brew install mecab` on macOS, or `apt install libmecab-dev` on Linux), RcppMeCab will use your system installation.

### Language selection (Windows)

On Windows, set `MECAB_LANG` before installation to choose the MeCab language variant. The default is `ko` (Korean).

```r
# Korean (default)
install.packages("RcppMeCab", type = "source")

# Japanese
Sys.setenv(MECAB_LANG = "ja")
install.packages("RcppMeCab", type = "source")
```

### Dictionary setup

You need a MeCab dictionary for your target language:

+ **Japanese**: Install [MeCab](http://taku910.github.io/mecab/) and IPAdic, or on macOS: `brew install mecab mecab-ipadic`
+ **Korean**: Install [mecab-ko](https://bitbucket.org/eunjeon/mecab-ko) and [mecab-ko-dic](https://bitbucket.org/eunjeon/mecab-ko-dic). Mirrors of these files are also available on the [RcppMeCab releases page](https://github.com/junhewk/RcppMeCab/releases/tag/0.0.1.0) in case Bitbucket is unavailable. On Windows: install [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) and [mecab-ko-dic-msvc](https://github.com/Pusnow/mecab-ko-dic-msvc) in `C:\mecab`
+ **Chinese**: Install MeCab with [MeCab Chinese Dic](http://www.52nlp.cn/%E7%94%A8mecab%E6%89%93%E9%80%A0%E4%B8%80%E5%A5%97%E5%AE%9E%E7%94%A8%E7%9A%84%E4%B8%AD%E6%96%87%E5%88%86%E8%AF%8D%E7%B3%BB%E7%BB%9F%E4%B8%89%EF%BC%9Amecab-chinese)

## Usage

This package has `pos` and `posParallel` functions.

```r
pos(sentence)                        # returns a list
pos(sentence, join = FALSE)          # morphemes only (tags as vector names)
pos(sentence, format = "data.frame") # returns a data frame
pos(sentence, user_dic = "path")     # with a compiled user dictionary
posParallel(sentence)                # parallelized, faster for large inputs
```

### Parameters

+ `sentence`: text to analyze
+ `join`: if `TRUE` (default), output is `morpheme/tag`; if `FALSE`, output is `morpheme` with tag as attribute
+ `format`: `"list"` (default) or `"data.frame"`
+ `sys_dic`: directory containing `dicrc`, `model.bin`, etc. Set a default with `options(mecabSysDic = "/path/to/dic")`
+ `user_dic`: path to a user dictionary compiled by `mecab-dict-index`

Note: provide full paths for `sys_dic` and `user_dic` (no tilde `~/` expansion).

## Compiling a user dictionary

MeCab's `DictionaryCompiler` API calls `die()`, which would crash the R session, so it is not exposed through RcppMeCab. Use the `mecab-dict-index` command-line tool instead.

You need a `model_file` for automatic cost estimation:

+ Japanese: [model_file in ipadic](https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7bnc5aFZSTE9qNnM)
+ Korean: `model.bin` in [mecab-ko-dic](https://bitbucket.org/eunjeon/mecab-ko-dic)

Prepare your entries as a CSV file ([Japanese format](http://taku910.github.io/mecab/dic.html), [Korean format](https://bitbucket.org/eunjeon/mecab-ko-dic/src/master/final/user-dic/README.md)), then compile:

```sh
/usr/local/libexec/mecab/mecab-dict-index \
  -m /path/to/model.bin \
  -d /path/to/mecab-dic \
  -u userdic.dic \
  -f utf8 -t utf8 \
  entries.csv
```

On Windows, use `mecab-dict-index.exe` bundled with [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) or the MeCab binary installer.

## Author

Junhewk Kim (junhewk.kim@gmail.com)

## Contributor

Kato Akiru
