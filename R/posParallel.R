#' parallel version of part-of-speech tagger
#'
#' \code{posParallel} returns part-of-speech (POS) tagged morpheme of the sentence.
#'
#' This is a parallelized version of MeCab part-of-speech tagger. The function gets a
#' character vector of any length and runs a loop inside C++ with Intel TBB to provide faster
#' processing.
#'
#' Parallelizing over a character vector is not supported by \code{RcppParallel}.
#' Thus, this function makes duplicates of the input and the output.
#' Therefore, if your data volume is large, use \code{pos} or divide the vector to
#' several sub-vectors.
#'
#' You can add a user dictionary to \code{user_dic}. It should be compiled by
#' \code{mecab-dict-index}. You can find an explatation about compiling a user
#' dictionary in the \url{https://github.com/junhewk/RcppMeCab}.
#'
#' You can also set a system dictionary especially if you are using multiple
#' dictionaries (for example, using both IPA and Juman dictionary at the same time in Japanese)
#' in \code{sys_dic}. Using \code{options(mecabSysDic=)}, you can set your
#' prefered system dictionary to the R terminal.
#'
#' If you want to get a morpheme only, use \code{join = False} to put tag names on the attribute.
#' Basically, the function will return a list of character vectors with (morpheme)/(tag) elements.
#'
#' @param sentence A character vector of any length. For analyzing multiple sentences, put them in one character vector.
#' @param join A bool to decide the output format. The default value is TRUE. If FALSE, the function will return morphemes only, and tags put in the attribute. if \code{format="data.frame"}, then this will be ignored.
#' @param format A data type for the result. The default value is "list". You can set this to "data.frame" to get a result as data frame format.
#' @param sys_dic A location of system MeCab dictionary. The default value is "".
#' @param user_dic A location of user-specific MeCab dictionary. The default value is "".
#' @return A string vector of POS tagged morpheme will be returned in conjoined character
#'  vecter form. Element name of the list are original phrases
#'
#' @examples
#' \dontrun{
#' sentence <- c(#some UTF-8 texts)
#' posParallel(sentence)
#' posParallel(sentence, join = FALSE)
#' posParallel(sentence, format = "data.frame")
#' posParallel(sentence, user_dic = "~/user_dic.dic")
#' # System dictionary example: in case of using mecab-ipadic-NEologd
#' pos(sentence, sys_dic = "/usr/local/lib/mecab/dic/mecab-ipadic-neologd/")
#' }
#'
#' @export
posParallel <- function(sentence, join = TRUE, format = c("list", "data.frame"), sys_dic = "", user_dic = "") {
  if (typeof(sentence) != "character") {
    if (typeof(sentence) == "factor") {
      stop("The type of input sentence is a factor. Please typesetting it with as.character().")
    } else {
      stop("The function gets a character vector only.")
    }
  }

  format = match.arg(format)

  if (!is.null(getOption("mecabSysDic")) && !sys_dic == "") sys_dic = getOption("mecabSysDic")

  if (format == "data.frame") {
    result <- posParallelDFRcpp(sentence, sys_dic, user_dic)
  } else {
    if (join == TRUE) {
      result <- posParallelJoinRcpp(sentence, sys_dic, user_dic)
    } else {
      result <- posParallelRcpp(sentence, sys_dic, user_dic)
    }
  }

  return(result)
}
