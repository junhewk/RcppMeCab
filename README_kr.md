# RcppMeCab

[![License](https://img.shields.io/badge/license-GPL-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl.html)
![R](https://img.shields.io/github/r-package/v/junhewk/RcppMeCab)
[![CRAN](http://www.r-pkg.org/badges/version/RcppMeCab)](https://cran.r-project.org/package=RcppMeCab)
[![Downloads](http://cranlogs.r-pkg.org/badges/RcppMeCab?color=brightgreen)](http://www.r-pkg.org/pkg/RcppMeCab)

RcppMeCab은 형태소 분석기인 `MeCab`을 R 환경에서 매끄럽게 사용할 수 있도록 한 패키지입니다. `Rcpp` 패키지를 사용하여 C++에서 직접 연산을 수행하므로 현재 R에서 제공되는 어떤 형태소 분석기보다 빠르며, 현존하는 `Python`과 `R` 라이브러리 중 가장 빠르고 효율적으로 형태소 분석을 수행할 수 있도록 라이브러리를 구성하였습니다. 제공되는 분산 처리 함수를 활용하여 다중 스레드 처리 또한 제공하므로, 대량의 자료를 빠르게 분석하기 원하시는 경우(예, 실시간으로 형태소 분석 처리를 통해 결과값을 제공해야 할 때) 적합한 패키지입니다. 또한, 원하시는 경우 일본어와 중국어도 분석할 수 있습니다(단, 동시에 분석할 수는 없고 `mecab`과 사전의 별도 설치 과정을 필요로 합니다).

## 0.0.1.3-3의 변경점

+ `pos()`에 단일 스트링을 넣으면 리스트가 아닌 캐릭터 벡터가 출력됩니다. 다른 라이브러리에서 tokenizer로 활용할 수 있도록 변경하였습니다.
+ `pos()`와 `posParallel()`은 이제 input 값을 vector name에 보존하지 않습니다. R way에 맞지 않는 표현이자 메모리 사용량을 줄이기 위하여 단순 list 또는 character vector로 결괏값을 되돌리도록 설정하였습니다.
+ 코드와 설명의 오류를 보정하였습니다. 특히, `sys_dic`과 `user_dic`에 full directory path를 입력해야 하는 점을 유념하여 주십시오.

## 설치

### 윈도

한국어 설명 페이지이므로 한국어 환경에서 사용하시는 것을 기준으로 설명합니다.

```
install.packages(devtools) # 현재 CRAN 버전은 Rcpp 지원이 늦어 버전업을 하지 못하고 있습니다. Github 버전을 설치하기 위해 devtools 패키지를 설치합니다.
library(devtools)
install_github("junhewk/RcppMeCab")
```

다음, `MeCab` 프로그램과 사전을 설치하셔야 합니다.

MeCab 프로그램: [mecab-ko-0.9.2-msvc-4](https://github.com/Pusnow/mecab-ko-msvc/releases/download/release-0.9.2-msvc-4/mecab-ko-msvc-x64.zip)

MeCab 사전: [mecab-ko-dic-2.1.1-20180720-msvc-2](https://github.com/Pusnow/mecab-ko-dic-msvc/releases/download/mecab-ko-dic-2.1.1-20180720-msvc-2/mecab-ko-dic-msvc.zip)

두 압축파일을 받아 `C:\mecab`에 압축해제합니다. 이때, 서브디렉토리를 생성하지 않고 바로 압축을 풉니다. `C:\mecab`에 `mecabrc` 파일과 `mecab-ko-dic` 디렉토리가 있어야 합니다.

#### 버전 관련 공지

현재의 `mecab-ko-msvc` 버전(0.999)는 안타깝게도 R이 지원하지 않습니다. 0.9.2 이하 버전으로 설치하여 주십시오.

### 리눅스, 맥

`MeCab`을 먼저 설치합니다. 다음 설치 페이지 [은전한닢 프로젝트](https://bitbucket.org/eunjeon/mecab-ko/src/master/README.md) 를 참조 부탁드립니다.

다음, `RcppMeCab`을 설치합니다.

```
install.packages(devtools) # 현재 CRAN 버전은 Rcpp 지원이 늦어 버전업을 하지 못하고 있습니다. Github 버전을 설치하기 위해 devtools 패키지를 설치합니다.
library(devtools)
install_github("junhewk/RcppMeCab")
```

#### 버전 관련 공지

현재의 `mecab-ko-msvc` 버전(0.999)는 안타깝게도 R이 지원하지 않습니다. `mecab-ko` 기존 버전으로 설치하셔야 합니다. `mecab-ko-dic`은 최신 버전으로 다운로드하신 다음, 해당 디렉토리를 `sys_dic`으로 지정하시면 지원합니다.

## 사용법

`RcppMeCab` 패키지는 두 개의 함수 `pos`와 `posParallel`을 제공합니다. `pos`는 기본 형태소 분석 함수이고, `posParallel`은 멀티스레딩을 지원하는 분석 함수입니다.

```
pos("안녕하세요.") # 리눅스, 맥의 경우
pos(enc2utf8("안녕하세요.")) # 윈도의 경우 입력을 UTF-8으로 선언해주셔야 합니다. 많은 분이 iconv 함수를 이용하는데, enc2utf8이 직관적이고 빠릅니다.
posParallel(c("안녕하세요", "반갑습니다.", "많은 이용 부탁드립니다")) # 리눅스, 맥에서 멀티스레딩
posParallel(enc2utf8(c("안녕하세요", "반갑습니다.", "많은 이용 부탁드립니다"))) # 윈도에서 멀티스레딩
```

함수에 네 가지 옵션을 추가할 수 있습니다.

+ join: `join = FALSE`로 설정하면, 분석된 형태소를 출력하고 품사는 특성(attribute)으로 붙습니다(`pos("안녕하세요.", join = FALSE)`). 기본은 `join = TRUE`로, "형태소/품사" 형태로 결과값을 반환합니다.
+ format: 출력 형식을 지정합니다. 기본형은 리스트입니다. 형식을 data.frame으로 설정하실 수 있습니다.(`pos("안녕하세요.", format = "data.frame"`)
+ sys_dic: `mecabrc` 파일과 `mecab-ko-dic` 디렉토리가 위치한 곳을 설정해 주실 수 있습니다.(`pos("안녕하세요.", sys_dic = "c:/mecab-dict"`) # 반드시 완전 경로를 입력하셔야 합니다. 리눅스의 ~/ 와 같은 약식 표현은 `mecab`이 인식하지 못합니다.
+ user_dic: 사용자 사전을 `.dic`의 형태로 만들어서 사용하실 수 있습니다(R 외부에서 작업해야 합니다). 사용자 사전 제작법은 아래를 참조해 주십시오.(`pos("안녕하세요.", user_dic = "c:/mecab/user.dic"`)

## 사용자 사전 제작

### 윈도

`c:\mecab`에 `mecab-dict-index.exe` 파일이 있습니다. 이 파일을 사용하여 제작한 `.csv` 파일을 컴파일해야 합니다. `mecab-ko-dic`의 `model_file`이 필요하므로, 위에서 설명한 위치로 `mecab`과 `mecab-ko-dic`을 설치하는 것을 권장합니다.

`.csv` 파일(예, `sample.csv`)을 만들어 원하는 사용자 사전을 제작합니다. 사용자 사전의 구조는 다음과 같습니다.

[CSV 파일 구조](https://docs.google.com/spreadsheets/d/1-9blXKjtjeKZqsf4NzHeYJCrr49-nXeRF6D80udfcwY/edit#gid=1718487366)

+ 고유명사
`대우,,,,NNP,*,F,대우,*,*,*,*`

+ 인명
`까비,,,,NNP,인명,F,까비,*,*,*,*`

+ 지명
`세종,,,,NNP,지명,T,세종,*,*,*,*`

위에서 추가하고 싶은 어휘의 형태에 따라 첫 칸과 여덟 번째 칸의 어휘를 바꿉니다. 아홉 번째 칸은 종성 유무입니다(종성으로 끝나면 T, 아니면 F).

명사(NNP)가 아닌 다른 품사도 가능합니다. 품사 변경의 경우 [품사태그표](https://docs.google.com/spreadsheet/ccc?key=0ApcJghR6UMXxdEdURGY2YzIwb3dSZ290RFpSaUkzZ0E&usp=sharing#gid=4)를 참조해주십시오.

csv 파일을 `c:\mecab`에 저장한 후, 다음 명령어를 커맨드 창(cmd)에서 실행합니다.

```
mecab-dict-index.exe -m c:\mecab\mecab-ko-dic\model.bin -d c:\mecab\mecab-ko-dic -u userdic.dic -f utf8 -t utf8 c:\mecab\sample.csv
```

위 명령어의 경우 `c:\mecab`에 `userdic.dic` 파일로 사용자 사전을 제작합니다. 다른 이름으로 저장하실 경우 `userdic.dic`을 변경하십시오. 다음, `pos` 함수의 `user_dic`에 위에 만든 사용자 사전을 넣으면 됩니다.(`pos("안녕하세요.", user_dic = "c:/mecab/userdic.dic")`)

### 리눅스, 맥

`mecab`과 `mecab-ko-dic`이 설치되어 있음을 가정합니다. 

`.csv` 파일(예, `sample.csv`)을 만들어 원하는 사용자 사전을 제작합니다. 사용자 사전의 구조는 다음과 같습니다.

[CSV 파일 구조](https://docs.google.com/spreadsheets/d/1-9blXKjtjeKZqsf4NzHeYJCrr49-nXeRF6D80udfcwY/edit#gid=1718487366)

+ 고유명사
`대우,,,,NNP,*,F,대우,*,*,*,*`

+ 인명
`까비,,,,NNP,인명,F,까비,*,*,*,*`

+ 지명
`세종,,,,NNP,지명,T,세종,*,*,*,*`

위에서 추가하고 싶은 어휘의 형태에 따라 첫 칸과 여덟 번째 칸의 어휘를 바꿉니다. 아홉 번째 칸은 종성 유무입니다(종성으로 끝나면 T, 아니면 F).

명사(NNP)가 아닌 다른 품사도 가능합니다. 품사 변경의 경우 [품사태그표](https://docs.google.com/spreadsheet/ccc?key=0ApcJghR6UMXxdEdURGY2YzIwb3dSZ290RFpSaUkzZ0E&usp=sharing#gid=4)를 참조해주십시오.

csv 파일을 원하는 곳에 저장한 후, 다음 명령어를 터미널에서 실행합니다.

```
$ /usr/local/libexec/mecab/mecab-dict-index -m /usr/local/lib/mecab/dic/mecab-ko-dic/model.bin -d /usr/local/lib/mecab/dic/mecab-ko-dic/ -u ~/userdic.dic -f utf8 -t utf8 ~/sample.csv
```

위 명령어의 경우 홈 디렉토리에 `userdic.dic` 파일로 사용자 사전을 제작합니다. 다른 이름으로 저장하실 경우 `userdic.dic`을 변경하십시오. 다음, `pos` 함수의 `user_dic`에 위에 만든 사용자 사전을 넣으면 됩니다.(`pos("안녕하세요.", user_dic = "/usr/local/lib/mecab/dic/mecab-ko-dic/user-dic/userdic.dic")`) # 마찬가지로 반드시 완전한 디렉토리 경로를 입력하셔야 합니다.

## 알려진 문제

`Exception, list()` 오류

현재 `RcppMeCab`은 오류 메시지를 정확하게 출력하지 않습니다. 이 경우, 실행에 오류가 나서 빈 리스트를 결과로 반환한 것입니다. 단, 해당 오류의 경우 `mecab-ko-dic`을 불러올 수 없는 경우가 대부분입니다. 윈도의 경우, `c:\mecab`에 `mecab`과 `mecab-ko-dic`이 제대로 압축 해제되어 있는지 확인해 주십시오.
