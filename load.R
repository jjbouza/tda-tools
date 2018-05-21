library('devtools')
library('Rcpp')

setwd('.')
Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
compileAttributes()
build()
install()
