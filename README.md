# RcppMeCab [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

This package, RcppMeCab, is a `Rcpp` wrapper for the part-of-speech morphological analyzer `MeCab`. It is based on `RmecabKo`, but supports native utf-8 encoding in C++ code and CJK MeCab library (in testing).

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

`RcppMeCab` in Windows supports Korean yet. Install [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) and [mecab-ko-dic-msvc](https://github.com/Pusnow/mecab-ko-dic-msvc) up to your 32-bit or 64-bit Windows version in `C:\mecab`. I think it'll work in Japanese too if [mecab binary](https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7WElGUGt6ejlpVXc) is installed in `C:\mecab`, but I can't test it because I can't use Japanese version Windows. Welcome always for the contribution of Japanese user.

```
# install.packages("devtools")
options(devtools.install.args = "--no-multiarch" # To avoid an error on installation; would be solved in later version
devtools::install_github("junhewk/RcppMecab")
```

## Usage

This package has `pos` function only.

```
pos(sentence, dict)
```

+ sentence: a text for analyzing
+ dict: a directory in which `mecabrc` is located, default value is ""

## Author

Junhewk Kim (junhewk.kim@gmail.com)

