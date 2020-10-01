if (Sys.getenv("MECAB_LANG") != "ja") {
  test_that("Test pos tagger works", {
    expect_equal(
      pos(
        enc2utf8("\uc548\ub155\ud558\uc138\uc694 \uc800\ub294 \ubc15\ucc2c\uc5fd \uc785\ub2c8\ub2e4."),
        format = "data.frame"
      )[4, 3],
      enc2utf8("\uc138\uc694")
    )
  })
} else {
  test_that("Skipped the test on Korean", {
    expect_true(TRUE)
  })
}
