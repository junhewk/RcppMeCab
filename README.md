# RcppMeCab

[![R-CMD-check](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml/badge.svg?branch=master)](https://github.com/junhewk/RcppMeCab/actions/workflows/R-CMD-check.yml)
[![CRAN status](https://www.r-pkg.org/badges/version/RcppMeCab)](https://CRAN.R-project.org/package=RcppMeCab)
[![CRAN downloads](https://cranlogs.r-pkg.org/badges/RcppMeCab)](https://CRAN.R-project.org/package=RcppMeCab)
[![CRAN total downloads](https://cranlogs.r-pkg.org/badges/grand-total/RcppMeCab)](https://CRAN.R-project.org/package=RcppMeCab)
[![R version](https://img.shields.io/badge/R-%E2%89%A5%203.4.0-276DC3.svg)](https://www.r-project.org/)
[![License: GPL](https://img.shields.io/badge/license-GPL-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

RcppMeCab is an `Rcpp` wrapper for the MeCab part-of-speech morphological
analyzer. It provides UTF-8 installation profiles and dictionaries for
Japanese, Korean, and Mandarin Chinese text. Engine selection happens at
package installation; runtime dictionary selection does not replace that
engine. Rcpp and parallel processing provide efficient text analysis.

__Please see [this](https://github.com/junhewk/RcppMeCab/blob/master/README_kr.md) for easy installation and usage examples in Korean.__

## Build profiles and MeCab engines

When a system MeCab installation is not found, RcppMeCab builds an engine from
source. `MECAB_LANG` selects that source-build profile and its bundled
dictionary:

| `MECAB_LANG` | Engine | Version | Bundled dictionary |
|---|---|---|---|
| `ko` (default) | [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) | 0.999 | mecab-ko-dic |
| `ja` | [MeCab](https://taku910.github.io/mecab/) | 0.996 | IPAdic |
| `zh` | [MeCab](https://taku910.github.io/mecab/) | 0.996 | [mecab-jieba 0.1.1](https://github.com/lindera/mecab-jieba) |

MeCab is dictionary-driven, so Chinese uses the standard MeCab engine with a
Mandarin dictionary rather than a separate Chinese engine. The Chinese profile
supports simplified and traditional UTF-8 text.

For the `ja` and `ko` profiles on Linux and macOS, an existing system MeCab
installation (detected via `mecab-config`) takes precedence. The `zh` profile
always builds the pinned standard MeCab engine so its compiler and
mecab-jieba dictionary remain compatible. `MECAB_LANG` is an installation
setting; it does not switch engines in a running R session.

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

### Installation profile

Set `MECAB_LANG` before installation to choose the engine and bundled
dictionary profile. Existing system engines take precedence for `ja` and `ko`
as described above; `zh` always uses the pinned source build.

```r
# Korean (default)
install.packages("RcppMeCab", type = "source")

# Japanese
Sys.setenv(MECAB_LANG = "ja")
install.packages("RcppMeCab", type = "source")

# Chinese (Mandarin; simplified and traditional text)
Sys.setenv(MECAB_LANG = "zh")
install.packages("RcppMeCab", type = "source")
```

### Dictionary

When RcppMeCab builds its own engine, the matching dictionary is
**automatically downloaded and installed** during package installation:

+ **Korean** (`MECAB_LANG=ko`, default): [mecab-ko-dic](https://github.com/Pusnow/mecab-ko-msvc/releases) (pre-compiled, from mecab-ko-msvc releases)
+ **Japanese** (`MECAB_LANG=ja`): [IPAdic](http://taku910.github.io/mecab/) (compiled from source during installation)
+ **Chinese** (`MECAB_LANG=zh`): [mecab-jieba](https://github.com/lindera/mecab-jieba) (compiled from source during installation)

The bundled dictionary is stored in the package's `dic/` directory and used
automatically. The `ja` and `ko` profiles use a system-configured dictionary
when they reuse a system MeCab installation. The `zh` profile always compiles
and bundles mecab-jieba with its pinned engine.

### Downloading additional dictionaries

You can download and install dictionaries for other languages after installation using `download_dic()`. No system-level MeCab installation is required — dictionary compilation is handled entirely within R.

```r
download_dic("ja") # download and compile Japanese IPAdic
download_dic("ko") # download Korean mecab-ko-dic
download_dic("zh") # download and compile Chinese mecab-jieba
```

Dictionaries are stored in the user data directory (`tools::R_user_dir("RcppMeCab", "data")`) and persist across R sessions.

Use `list_dic()` to see all installed dictionaries:

```r
list_dic()
#>      lang         name                              path active
#> 1 bundled      bundled /path/to/RcppMeCab/dic              TRUE
#> 2      ja       ipadic ~/.local/share/R/RcppMeCab/ja      FALSE
#> 3      ko mecab-ko-dic ~/.local/share/R/RcppMeCab/ko      FALSE
#> 4      zh  mecab-jieba ~/.local/share/R/RcppMeCab/zh      FALSE
```

## Usage

This package has `pos` and `posParallel` functions.

```r
pos(sentence)                        # returns a list
pos(sentence, join = FALSE)          # morphemes only (tags as vector names)
pos(sentence, format = "data.frame") # returns a data frame
pos(sentence, user_dic = "path")     # with a compiled user dictionary
posParallel(sentence)                # parallelized, faster for large inputs
dictionary_info()                    # active dictionary metadata
```

### Selecting dictionaries

`lang` selects a dictionary previously installed with `download_dic()`. It
does not change the MeCab engine compiled or linked into RcppMeCab.

```r
download_dic("ja")
download_dic("zh")

pos("東京は日本の首都です。", lang = "ja")
pos("我是中国人。", lang = "zh")
```

The active default dictionary can also be changed:

```r
set_dic("ja")
pos("東京は日本の首都です。")
set_dic("zh")
pos("我是中国人。")
set_dic("bundled") # restore the build-time dictionary
```

Japanese and Chinese dictionaries use the standard MeCab engine and can be
selected within a standard MeCab installation. Supported Korean analysis
requires the mecab-ko engine. Loading `mecab-ko-dic` into standard MeCab does
not add mecab-ko's Korean-specific whitespace behavior. Conversely, loading a
Japanese or Chinese dictionary does not replace an installed mecab-ko engine.
Use a separate RcppMeCab installation or reinstall with the required engine
profile when moving between Korean and standard MeCab behavior.

Dictionary feature layouts are also language-specific. List output consistently
uses each token's primary POS tag. The historical `data.frame` columns
`subtype` and `analytic` follow the Japanese/Korean layout; they do not expose
all mecab-jieba fields. Pinyin, traditional/simplified forms, and definitions
do not yet have dedicated output columns.

A dictionary can also be selected by path:

```r
pos("text", sys_dic = "/path/to/custom-dic")
options(mecabSysDic = "/path/to/custom-dic")
```

`dictionary_info()` reports the filename, charset, type, size, context sizes,
and version of every dictionary loaded by MeCab. It reports dictionaries, not
the language or engine variant, and is useful for confirming which dictionary
is active.

### Parameters

+ `sentence`: text to analyze
+ `join`: if `TRUE` (default), output is `morpheme/tag`; if `FALSE`, output is `morpheme` with tag as attribute
+ `format`: `"list"` (default) or `"data.frame"`
+ `lang`: dictionary code (`"ja"`, `"ko"`, or `"zh"`) selecting a dictionary installed via `download_dic()`. It does not switch engines and overrides `sys_dic` when specified.
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

## Authors

Junhewk Kim (junhewk.kim@gmail.com), Taku Kudo

## Contributors

Akiru Kato, Patrick Schratz
