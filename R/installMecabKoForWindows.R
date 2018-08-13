#' install mecab-ko for windows
#'
#' @param path The path to download a file and unzip. The default is "c:/mecab" and Default path is strongly recommended.
#' @param release "x86" or "x84". The default is "auto" but you can select if you already know.
#' @param cleanup Remove download file after unzip. The default is TRUE but you can remain file cleanup = FALSE.
#' @return None.
#'
#' @examples
#' \dontrun{
#' installMecabKoForWindows()
#' installMecabKoForWindows(path = "C://mecab")
#' }
#'
#' @export
installMecabKoForWindows <- function(path = "c://mecab",
                                     release = "auto",
                                     cleanup = TRUE){
  installForWindows(github_repo = "Pusnow/mecab-ko-msvc",
                    path = path,
                    release = release,
                    cleanup = cleanup)
}

#' install mecab-ko-dic for windows
#'
#' @param path the path to download a file and unzip. The default is "c:/mecab" and Default path is strongly recommended.
#' @param cleanup remove download file after unzip. The default is TRUE but you can remain file cleanup = FALSE.
#' @return None.
#'
#' @examples
#' \dontrun{
#' installMecabKoDicForWindows()
#' installMecabKoDicForWindows(path = "C://mecab")
#' }
#'
#' @export
installMecabKoDicForWindows <- function(path = "c://mecab",
                                        cleanup = TRUE){
  installForWindows(github_repo = "Pusnow/mecab-ko-dic-msvc",
                    path = path,
                    cleanup = cleanup)
}

#' @importFrom jsonlite fromJSON
installForWindows <-
  function(github_repo = "",
           path = "c:\\mecab",
           release = "auto",
           cleanup = TRUE) {
    if (Sys.info()["sysname"] != "Windows") {
      stop("This function only for windows os.")
    }
    path <-
      normalizePath(path, winslash = .Platform$file.sep, mustWork = F)
    if (!dir.exists(path)) {
      message(paste0("Create ", path))
      dir.create(path)
    }
    downloadLinks <-
      jsonlite::fromJSON(paste0(
        "https://api.github.com/repos/",
        github_repo,
        "/releases/latest"
      ))
    if (length(downloadLinks$assets$browser_download_url) != 1) {
      downloadLink <- getLink(downloadLinks, release)
    } else {
      downloadLink <- downloadLinks$assets$browser_download_url
    }
    fileName <- strsplit(downloadLink, "/")[[1]]
    fileName <- fileName[length(fileName)]
    filePath <- file.path(path, fileName)
    download.file(downloadLink, destfile = filePath)
    unzip(zipfile = filePath, exdir = path)
    if (cleanup) {
      invisible(file.remove(filePath))
      message("Cleanup download file.")
    }
    project <- strsplit(github_repo, "/")[[1]][2]
    message(paste0(project, " installation for windows is done."))
    message(paste0("Please check path : ", path))
  }

getLink <- function(downloadLinks, release) {
  switch(release,
         auto = {
           dli <- grep("x64", downloadLinks$assets$browser_download_url)
           if (Sys.info()["release"] == ">= 8 x64") {
             downloadLink <- downloadLinks$assets$browser_download_url[dli]
           } else {
             downloadLink <- downloadLinks$assets$browser_download_url[-dli]
           }
         },
         x64 = {
           downloadLink <-
             grep("x64", downloadLinks$assets$browser_download_url, value = T)
         },
         x86 = {
           dli <- grep("x64", downloadLinks$assets$browser_download_url)
           downloadLink <-
             downloadLinks$assets$browser_download_url[-dli]
         })
  return(downloadLink)
}


#system2("powershell", args=c("-file", "C:\\directoryName\\coolScript.ps1"))
