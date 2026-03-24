#' @noRd
## usethis namespace: start
#' @importFrom Rcpp sourceCpp
#' @useDynLib RcppMeCab, .registration=TRUE
## usethis namespace: end
NULL

#' @noRd
.onLoad <- function(libname, pkgname) {
  # Set bundled dictionary as default if no mecabSysDic option is set
  if (is.null(getOption("mecabSysDic"))) {
    dic_dir <- system.file("dic", package = pkgname)
    if (nzchar(dic_dir) && file.exists(file.path(dic_dir, "sys.dic"))) {
      options(mecabSysDic = dic_dir)
    }
  }
}

#' @noRd
#' @param libpath libpath
.onUnload <- function(libpath) {
  library.dynam.unload("RcppMeCab", libpath)
}
