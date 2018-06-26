## Test environments
* local OS X install, R 3.5.0
* local Windows 10 install, R 3.5.0

## R CMD check results

0 errors | 0 warnings | 4 note

* This is a patch.
  1. Minor glitch in 'posLoopRcpp' function is fixed.
  2. Install environmental variable "MECAB_LANG" gets "ja", not "jp".
  3. Minor glitch in 'sys_dic' arg is fixed.
  4. A format of the result is revised; 'posRcpp' function will return a list, and both functions ('pos' and 'posParallel') can return a data frame.
* This package depends on an installed third-party library.
* in Mac OSX:
  checking for GNU extensions in Makefiles ... NOTE
  GNU make is a SystemRequirements.
* in Windows 10 (I'm working on this; it will be resolved later):
  checking compiled code ... NOTE
  File 'RcppMeCab/libs/i386/libmecab.dll':
    Found no calls to: 'R_registerRoutines', 'R_useDynamicSymbols'
  File 'RcppMeCab/libs/x64/libmecab.dll':
    Found no calls to: 'R_registerRoutines', 'R_useDynamicSymbols'
  
## Reverse dependencies

This is still a new release (first release was one week before), so there are no reverse dependencies.

---

* FAILURE SUMMARY
