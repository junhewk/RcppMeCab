# RcppMeCab

[![License](https://img.shields.io/badge/license-GPL-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl.html)
![R](https://img.shields.io/github/r-package/v/junhewk/RcppMeCab)
[![CRAN](http://www.r-pkg.org/badges/version/RcppMeCab)](https://cran.r-project.org/package=RcppMeCab)
[![Downloads](http://cranlogs.r-pkg.org/badges/RcppMeCab?color=brightgreen)](http://www.r-pkg.org/pkg/RcppMeCab)

RcppMeCab은 형태소 분석기인 `MeCab`을 R 환경에서 매끄럽게 사용할 수 있도록 한 패키지입니다. `Rcpp` 패키지를 사용하여 C++에서 직접 연산을 수행하므로 현재 R에서 제공되는 어떤 형태소 분석기보다 빠르며, 현존하는 `Python`과 `R` 라이브러리 중 가장 빠르고 효율적으로 형태소 분석을 수행할 수 있도록 라이브러리를 구성하였습니다. 제공되는 분산 처리 함수를 활용하여 다중 스레드 처리 또한 제공하므로, 대량의 자료를 빠르게 분석하기 원하시는 경우 적합한 패키지입니다.

## MeCab 백엔드

RcppMeCab은 설치 시 MeCab을 소스에서 빌드합니다. `MECAB_LANG` 환경 변수로 MeCab 변형을 선택합니다:

| `MECAB_LANG` | 백엔드 | 버전 | 소스 |
|---|---|---|---|
| `ko` (기본값) | [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) | 0.999 | Pusnow/mecab-ko-msvc |
| `ja` | [MeCab](http://taku910.github.io/mecab/) | 0.996 | taku910/mecab |

한국어 사용 시 [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc) 0.999 버전을 사용합니다. 기존 mecab-ko 0.9.2 대비 한국어 띄어쓰기 처리가 개선되었습니다.

리눅스, macOS에서 MeCab이 이미 시스템에 설치되어 있는 경우 (`mecab-config`으로 감지), `MECAB_LANG`과 무관하게 시스템 설치본을 사용합니다.

## 설치

### 모든 플랫폼 (윈도, 리눅스, macOS)

RcppMeCab은 MeCab이 시스템에 없으면 자동으로 소스를 다운로드하여 빌드합니다. 별도의 MeCab 설치가 필요하지 않습니다.

```r
# install.packages("devtools")
devtools::install_github("junhewk/RcppMeCab")
```

### 사전

패키지 설치 시 MeCab 사전이 **자동으로 다운로드 및 설치**됩니다:

+ **한국어** (`MECAB_LANG=ko`, 기본값): [mecab-ko-dic](https://github.com/Pusnow/mecab-ko-msvc/releases) (mecab-ko-msvc 릴리즈에서 사전 다운로드)
+ **일본어** (`MECAB_LANG=ja`): [IPAdic](http://taku910.github.io/mecab/) (설치 시 소스에서 컴파일)

사전은 패키지의 `dic/` 디렉토리에 저장되며 자동으로 사용됩니다. 별도의 사전 설치가 필요하지 않습니다.

### 추가 사전 다운로드

설치 후 `download_dic()`으로 다른 언어의 사전을 다운로드할 수 있습니다. 시스템 수준의 MeCab 설치가 필요하지 않으며, 사전 컴파일은 R 내에서 처리됩니다.

```r
download_dic("ja") # 일본어 IPAdic 다운로드 및 컴파일
download_dic("ko") # 한국어 mecab-ko-dic 다운로드
```

사전은 사용자 데이터 디렉토리(`tools::R_user_dir("RcppMeCab", "data")`)에 저장되며 R 세션 간에 유지됩니다.

`list_dic()`으로 설치된 사전 목록을 확인할 수 있습니다:

```r
list_dic()
#>      lang         name                              path active
#> 1 bundled      bundled /path/to/RcppMeCab/dic              TRUE
#> 2      ja       ipadic ~/.local/share/R/RcppMeCab/ja      FALSE
#> 3      ko mecab-ko-dic ~/.local/share/R/RcppMeCab/ko      FALSE
```

## 사용법

`RcppMeCab` 패키지는 `pos`, `posParallel`, `dict_index`, `download_dic`, `set_dic`, `list_dic` 함수를 제공합니다.

```r
pos("안녕하세요.")
pos("안녕하세요.", join = FALSE)          # 형태소만 출력 (품사는 attribute)
pos("안녕하세요.", format = "data.frame") # 데이터프레임으로 출력
posParallel(c("안녕하세요", "반갑습니다.", "많은 이용 부탁드립니다")) # 멀티스레딩
```

### 언어 전환

`lang` 매개변수로 언어별 사전을 선택할 수 있습니다:

```r
pos("東京は日本の首都です。", lang = "ja")
pos("안녕하세요", lang = "ko")
```

또는 `set_dic()`으로 기본 사전을 설정합니다:

```r
set_dic("ja")
pos("東京は日本の首都です。") # 일본어 사전 사용
set_dic("ko")
pos("안녕하세요")              # 한국어 사전 사용
set_dic("bundled")             # 빌드 시 포함된 사전으로 복원
```

직접 사전 경로를 지정할 수도 있습니다:

```r
pos("텍스트", sys_dic = "/path/to/custom-dic")
options(mecabSysDic = "/path/to/custom-dic")
```

### 함수 옵션

+ `join`: `FALSE`로 설정하면 형태소만 출력, 품사는 attribute. 기본값 `TRUE`는 "형태소/품사" 형태.
+ `format`: `"list"` (기본) 또는 `"data.frame"`
+ `lang`: 언어 코드 (`"ja"` 또는 `"ko"`). `download_dic()`으로 설치한 사전을 선택합니다. 지정 시 `sys_dic`보다 우선합니다.
+ `sys_dic`: 시스템 사전 디렉토리 경로. `options(mecabSysDic = "경로")`로 기본값 설정 가능.
+ `user_dic`: `dict_index()`로 컴파일한 사용자 사전 경로.

`sys_dic`과 `user_dic`에는 반드시 완전한 경로를 입력하셔야 합니다 (`~/` 약식 표현 불가).

## 사용자 사전 제작

RcppMeCab은 `dict_index()` 함수를 제공하여 R에서 직접 사용자 사전을 컴파일할 수 있습니다. 별도의 `mecab-dict-index` 커맨드라인 도구가 필요하지 않습니다.

`.csv` 파일(예, `sample.csv`)을 만들어 원하는 사용자 사전을 제작합니다. 사용자 사전의 구조는 다음과 같습니다.

[CSV 파일 구조](https://docs.google.com/spreadsheets/d/1-9blXKjtjeKZqsf4NzHeYJCrr49-nXeRF6D80udfcwY/edit#gid=1718487366)

+ 고유명사: `대우,,,,NNP,*,F,대우,*,*,*,*`
+ 인명: `까비,,,,NNP,인명,F,까비,*,*,*,*`
+ 지명: `세종,,,,NNP,지명,T,세종,*,*,*,*`

위에서 추가하고 싶은 어휘의 형태에 따라 첫 칸과 여덟 번째 칸의 어휘를 바꿉니다. 아홉 번째 칸은 종성 유무입니다(종성으로 끝나면 T, 아니면 F).

명사(NNP)가 아닌 다른 품사도 가능합니다. 품사 변경의 경우 [품사태그표](https://docs.google.com/spreadsheet/ccc?key=0ApcJghR6UMXxdEdURGY2YzIwb3dSZ290RFpSaUkzZ0E&usp=sharing#gid=4)를 참조해주십시오.

csv 파일을 저장한 후, R에서 다음과 같이 컴파일합니다.

```r
dict_index(
  dic_csv = "sample.csv",
  out_dic = "userdic.dic",
  dic_dir = "c:/mecab/mecab-ko-dic"  # 시스템 사전 디렉토리 경로
)

# 컴파일된 사전을 사용하여 형태소 분석:
pos("안녕하세요.", user_dic = "c:/mecab/userdic.dic")
```

리눅스, 맥에서도 동일하게 사용할 수 있습니다.

```r
dict_index(
  dic_csv = "~/sample.csv",
  out_dic = "~/userdic.dic",
  dic_dir = "/usr/local/lib/mecab/dic/mecab-ko-dic"
)

pos("안녕하세요.", user_dic = "/usr/local/lib/mecab/dic/mecab-ko-dic/user-dic/userdic.dic")
```

반드시 완전한 디렉토리 경로를 입력하셔야 합니다.
