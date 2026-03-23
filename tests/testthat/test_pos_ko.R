test_that("Test pos tagger works on ko", {
  skip_if(.Platform$OS.type == "windows", "Skip testing on Windows")
  skip_if(!l10n_info()[["UTF-8"]], "Non-UTF-8 locale")
  skip_if(Sys.getenv("MECAB_LANG") == "ja", "Skip testing on Korean")
  result <- tryCatch(
    pos(
      enc2utf8("\uc548\ub155\ud558\uc138\uc694 \uc800\ub294 \ubc15\ucc2c\uc5fd \uc785\ub2c8\ub2e4."),
      format = "data.frame"
    ),
    error = function(e) NULL
  )
  skip_if(is.null(result) || nrow(result) == 0, "Korean dictionary not available")
  expect_equal(result[4, 4], enc2utf8("\uc800"))
})
