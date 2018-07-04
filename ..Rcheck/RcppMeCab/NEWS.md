# RcppMeCab 0.0.1.2

+ loop version of `pos` function is fixed (duplicated result)
+ `sys_dic` is now working properly
+ each function checks `getOption("mecabSysDic")` to get user preference of MeCab system dictionary
+ present input character vecters over the result list attributes (names)
+ a single character vector input in `pos()` will return a list
+ an option for result type is added: with arg `format="data.frame"`

# RcppMeCab 0.0.1.1

+ `posParallel` function is added to support parallelization
+ `join` parameter is added to yield a output of morphemes only
+ `RcppParallel` dependency
+ `user_dic` parameter is added to support user dictionary usage
+ Published on CRAN

# RcppMeCab 0.0.1.0

+ First release
+ Windows support is solved; further work should be done for multiarch installation
