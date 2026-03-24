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

### Dictionary

A MeCab dictionary is **automatically downloaded and installed** during package installation:

+ **Korean** (`MECAB_LANG=ko`, default): [mecab-ko-dic](https://github.com/Pusnow/mecab-ko-msvc/releases) (pre-compiled, from mecab-ko-msvc releases)
+ **Japanese** (`MECAB_LANG=ja`): [IPAdic](http://taku910.github.io/mecab/) (compiled from source during installation)

The bundled dictionary is stored in the package's `dic/` directory and used automatically — no manual dictionary setup is required.

### Downloading additional dictionaries

You can download and install dictionaries for other languages after installation using `download_dic()`. No system-level MeCab installation is required — dictionary compilation is handled entirely within R.

```r
download_dic("ja") # download and compile Japanese IPAdic
download_dic("ko") # download Korean mecab-ko-dic
```

Dictionaries are stored in the user data directory (`tools::R_user_dir("RcppMeCab", "data")`) and persist across R sessions.

Use `list_dic()` to see all installed dictionaries:

```r
list_dic()
#>      lang         name                              path active
#> 1 bundled      bundled /path/to/RcppMeCab/dic              TRUE
#> 2      ja       ipadic ~/.local/share/R/RcppMeCab/ja      FALSE
#> 3      ko mecab-ko-dic ~/.local/share/R/RcppMeCab/ko      FALSE
```

## Usage

This package has `pos` and `posParallel` functions.

```r
pos(sentence)                        # returns a list
pos(sentence, join = FALSE)          # morphemes only (tags as vector names)
pos(sentence, format = "data.frame") # returns a data frame
pos(sentence, user_dic = "path")     # with a compiled user dictionary
posParallel(sentence)                # parallelized, faster for large inputs
```

### Switching languages

Use the `lang` parameter to select a dictionary by language:

```r
pos("東京は日本の首都です。", lang = "ja")
pos("안녕하세요", lang = "ko")
```

Or set a default with `set_dic()`:

```r
set_dic("ja")
pos("東京は日本の首都です。") # uses Japanese dictionary
set_dic("ko")
pos("안녕하세요")              # uses Korean dictionary
set_dic("bundled")             # switch back to the build-time dictionary
```

You can also specify a custom dictionary path directly:

```r
pos("text", sys_dic = "/path/to/custom-dic")
options(mecabSysDic = "/path/to/custom-dic")
```

### Parameters

+ `sentence`: text to analyze
+ `join`: if `TRUE` (default), output is `morpheme/tag`; if `FALSE`, output is `morpheme` with tag as attribute
+ `format`: `"list"` (default) or `"data.frame"`
+ `lang`: language code (`"ja"` or `"ko"`) to select a dictionary installed via `download_dic()`. Overrides `sys_dic` when specified.
+ `sys_dic`: directory containing `dicrc`, `sys.dic`, etc. Set a default with `options(mecabSysDic = "/path/to/dic")`
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
