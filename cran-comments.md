## Test environments
* local OS X install, R 3.5.0
* win-builder (devel and release)

## R CMD check results

0 errors | 0 warnings | 6 note

* This is a new release.
* This package depends on an installed third-party library.
* checking CRAN incoming feasibility ... NOTE
Maintainer: 'Junhewk Kim <junhewk.kim@gmail.com>'
* checking installed package size ... NOTE
  installed size is  5.3Mb
  sub-directories of 1Mb or more:
    libs   5.2Mb
* checking for GNU extensions in Makefiles ... NOTE
GNU make is a SystemRequirements.
* checking compiled code ... NOTE
File 'RcppMeCab/libs/i386/libmecab.dll':
  Found no calls to: 'R_registerRoutines', 'R_useDynamicSymbols'
File 'RcppMeCab/libs/x64/libmecab.dll':
  Found no calls to: 'R_registerRoutines', 'R_useDynamicSymbols'

For the last matter and other mistakes, I got several comments from Dr. Ligge (Thanks so much about comments):

1. you have  not declared all authors, contributors and coipyright
holders in Authors@R.
	Resolution: I added Taku Kudo (the developer of the MeCab library, see https://github.com/taku910/mecab/blob/master/mecab/AUTHORS) in Author and Author@R.
2. You have not declared SystemRequirements as detailed in Writing R
Extensions. Example: MeCab
	Resolution: I added 'MeCab' in SystemRequirements.
3. Do not use directed quotes in the Title and Description field of the
DSCRIPTION file. Raher use undirected quotes.
	Resolution: I changes directed quotes to undirected quotes.
4. License: GPL-3 is also possible for the MeCab binaries we are then
shipping in the binary verion of your package without shiping sources???
	Resolution: MeCab uses GPL (see https://github.com/taku910/mecab/blob/master/mecab/GPL). I changes the package's license to GPL.
5. Please provide working examples so that your packages gets actually
executed in the checks.
	Resolution: In my opinion, examples won't work in the CRAN win-builder. This package requires pre-installed MeCab and MeCab dictionary libraries. MeCab is also added to SystemRequirements.
6. You have C code in your package, hence why not register routines?
	Resolution: I tried several times to register routines, but MeCab library is compiled by MSVC and reject the R dll. (Actually, I don't know how to deal this matter.)

## Reverse dependencies

This is a new release, so there are no reverse dependencies.

---

* FAILURE SUMMARY
