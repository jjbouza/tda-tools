library('devtools')
library('Rcpp')


print("\n Installing current local version of tda-tools... \n")
setwd('..')
Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
compileAttributes()
build()
print("\n Installation done. \n")
install()
