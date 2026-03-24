## Test environments

* Ubuntu 24.04 LTS (local), R 4.3.3
* GitHub Actions: Ubuntu (latest), macOS (latest), Windows (latest), R release
* Tested with both Japanese (IPAdic) and Korean (mecab-ko-dic) dictionaries

## R CMD check results

0 errors | 0 warnings | 5 notes

### NOTEs

* **CRAN incoming feasibility**: Two URLs redirect from http to https
  (www.r-pkg.org and www.rcpp.org). Both respond with 200.

* **Installed package size (330 Mb)**: The package bundles a MeCab
  dictionary (~325 Mb) at install time so that users do not need to
  install MeCab or dictionaries separately. The dictionary is required
  for the package to function.

* **Future file timestamps**: "unable to verify current time" — local
  environment issue, not package-related.

* **DESCRIPTION meta-information**: Author field differs from Authors@R
  because Authors@R includes a contributor (Akiru Kato) not listed in
  the plain Author field. Will be harmonised in a future release.

* **Non-portable compilation flag** (`-mno-omit-leaf-frame-pointer`):
  This flag comes from the system R installation (Ubuntu 24.04), not
  from the package itself. The package does not set this flag.

## Changes since last CRAN release

This is a major update from the last CRAN version (0.0.1.2):

* MeCab is now built from source at install time on all platforms
  (Linux, macOS, Windows). No system-level MeCab installation required.
* Dictionaries are automatically downloaded and bundled during install.
* New `download_dic()` / `set_dic()` / `list_dic()` functions for
  managing multiple language dictionaries entirely within R.
* New `lang` parameter on `pos()` and `posParallel()` for per-call
  language selection (e.g. `pos("text", lang = "ja")`).
* New `dict_index()` function to compile user dictionaries from R.
* mecab-ko 0.999 support (Korean).
* Windows build now compiles MeCab from source instead of using
  pre-built DLLs.

## Reverse dependencies

None. This package has no reverse dependencies on CRAN.
