## Test environments

* local macOS 15.7.7 (arm64), R 4.6.1
* Ubuntu 24.04 LTS (local), R 4.3.3
* GitHub Actions: Ubuntu (latest), macOS (latest), Windows (latest), R release
* Tested with Japanese (IPAdic), Korean (mecab-ko-dic), and Chinese
  (mecab-jieba) dictionaries

## R CMD check results

0 errors | 1 warning | 1 note

### Local-only findings

* **Top-level files warning**: A complete local check needs the
  `checkbashisms` script, which is not installed on this macOS machine.

* **CRAN incoming feasibility note**: The previous CRAN update was one day
  earlier. Version 0.0.1.8 is a necessary backend correction for RmecabKo.

The installed package size is reported as 326 Mb (INFO). The package bundles
the required mecab-ko-dic system dictionary so users do not need a separate
dictionary installation.

## Resubmission context

Version 0.0.1.8 fixes the Korean build profile exposed by RmecabKo's Debian
incoming check. A system MeCab 0.996 installation is no longer treated as a
compatible Korean engine. The package instead builds the pinned mecab-ko 0.999
engine and always provisions mecab-ko-dic. The Korean integration tests now
fail if the engine or dictionary is unavailable rather than silently skipping.

## Changes since last CRAN release

This is a major update from the last CRAN version (0.0.1.2):

* MeCab is built from source when a compatible system engine is unavailable.
* Dictionaries are automatically downloaded and bundled during install.
* New `download_dic()` / `set_dic()` / `list_dic()` functions for
  managing multiple MeCab dictionaries entirely within R.
* New `lang` parameter on `pos()` and `posParallel()` for per-call
  dictionary selection (e.g. `pos("text", lang = "ja")`). This does not
  switch the install-time MeCab engine.
* New `dict_index()` function to compile user dictionaries from R.
* New `dictionary_info()` function reporting metadata for loaded dictionaries.
* mecab-ko 0.999 support (Korean).
* First-class Mandarin Chinese build profile using mecab-jieba 0.1.1.
* Windows build now compiles MeCab from source instead of using
  pre-built DLLs.

## Reverse dependencies

None. This package has no reverse dependencies on CRAN.
