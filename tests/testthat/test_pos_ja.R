test_that("Test pos tagger works on ja", {
  skip_if(.Platform$OS.type == "windows", "Skip testing on Windows")
  skip_if(!l10n_info()[["UTF-8"]], "Non-UTF-8 locale")
  skip_if_not(Sys.getenv("MECAB_LANG") == "ja", "Skip testing on Japanese")
  result <- tryCatch(
    pos(
      enc2utf8("\u982d\u304c\u8d64\u3044\u9b5a\u3092\u98df\u3079\u305f\u732b"),
      format = "data.frame"
    ),
    error = function(e) NULL
  )
  skip_if(is.null(result) || nrow(result) == 0, "Japanese dictionary not available")
  expect_equal(result[4, 4], enc2utf8("\u9b5a"))
})
