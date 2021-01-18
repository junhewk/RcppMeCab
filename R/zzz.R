#' @noRd
#' @param libname libname
#' @param pkgname pkgname
.onLoad <- function(libname, pkgname) {
  library.dynam(
    pkgname,
    pkgname,
    lib.loc = .libPaths(),
    DLLpath = system.file("libs", package = pkgname)
  )
}

#' @noRd
#' @param libpath libpath
.onUnload <- function(libpath) {
  library.dynam.unload("RcppMeCab", libpath)
}
