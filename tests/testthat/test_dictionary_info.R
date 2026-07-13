test_that("dictionary_info reports the loaded system dictionary", {
  skip_if(Sys.info()[["sysname"]] == "Windows" &&
            identical(Sys.getenv("MECAB_LANG"), "ja"),
          "Japanese dictionary is not bundled on Windows")
  info <- dictionary_info()

  expect_s3_class(info, "data.frame")
  expect_named(
    info,
    c("filename", "charset", "type", "size", "left_size", "right_size", "version")
  )
  expect_gte(nrow(info), 1L)
  expect_true(any(info$type == "system"))
  expect_true(all(nzchar(info$filename)))
})

test_that("dictionary_info validates arguments", {
  expect_error(dictionary_info(NA_character_), "sys_dic")
  expect_error(dictionary_info(user_dic = character()), "user_dic")
})
