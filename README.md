# RcppMeCab

[![License](https://img.shields.io/badge/license-GPL-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl.html)
![R](https://img.shields.io/github/r-package/v/junhewk/RcppMeCab)
[![CRAN](http://www.r-pkg.org/badges/version/RcppMeCab)](https://cran.r-project.org/package=RcppMeCab)
[![Downloads](http://cranlogs.r-pkg.org/badges/RcppMeCab?color=brightgreen)](http://www.r-pkg.org/pkg/RcppMeCab)
[![R-CMD-check](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml/badge.svg)](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml)

This package, RcppMeCab, is a `Rcpp` wrapper for the part-of-speech morphological analyzer `MeCab`. It supports native utf-8 encoding in C++ code and CJK (Chinese, Japanese, and Korean) MeCab library. This package fully utilizes the power `Rcpp` brings `R` computation to analyze texts faster.

__Please see [this](README_kr.md) for easy installation and usage examples in Korean.__

## MeCab backends

RcppMeCab builds MeCab from source at install time. The MeCab variant is selected by the `MECAB_LANG` environment variable:

| `MECAB_LANG` | Backend | Version | Source |
|---|---|---|---|
| `ko` (default) | [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) | 0.999 | Pusnow/mecab-ko-msvc |
| `ja` | [MeCab](http://taku910.github.io/mecab/) | 0.996 | taku910/mecab |

On Linux and macOS, if MeCab is already installed system-wide (detected via `mecab-config`), RcppMeCab uses the system installation regardless of `MECAB_LANG`.

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

### Language selection

Set `MECAB_LANG` before installation to choose the MeCab variant:

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
+ **Korean**: Install [mecab-ko-dic](https://github.com/Pusnow/mecab-ko-msvc/releases) (available as `mecab-ko-dic.zip`/`mecab-ko-dic.tar.gz` from mecab-ko-msvc releases)
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
+ `user_dic`: path to a user dictionary compiled by `dict_index()`

Note: provide full paths for `sys_dic` and `user_dic` (no tilde `~/` expansion).

## Compiling a user dictionary

RcppMeCab provides the `dict_index()` function to compile user dictionaries directly from R, without needing the `mecab-dict-index` command-line tool.

Prepare your entries as a CSV file ([Japanese format](http://taku910.github.io/mecab/dic.html), [Korean format](https://bitbucket.org/eunjeon/mecab-ko-dic/src/master/final/user-dic/README.md)), then compile:

```r
dict_index(
  dic_csv = "entries.csv",
  out_dic = "userdic.dic",
  dic_dir = "/path/to/mecab-dic"
)

# Then use the compiled dictionary:
pos("some text", user_dic = "userdic.dic")
```

## Author

Junhewk Kim (junhewk.kim@gmail.com)

## Contributor

Kato Akiru
