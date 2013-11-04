dnl $Id$
dnl config.m4 for extension wms_helper

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(wms_helper, for wms_helper support,
dnl Make sure that the comment is aligned:
dnl [  --with-wms_helper             Include wms_helper support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(wms_helper, whether to enable wms_helper support,
[  --enable-wms_helper           Enable wms_helper support])

if test "$PHP_WMS_HELPER" = "yes"; then
  PHP_ADD_LIBPATH(/usr/lib)
  AC_DEFINE(HAVE_WMS_HELPERLIB, 1, [Whether you have WMS_HELPERLIB])
  


  dnl # --with-wms_helper -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/wms_helper.h"  # you most likely want to change this
  dnl if test -r $PHP_WMS_HELPER/$SEARCH_FOR; then # path given as parameter
  dnl   WMS_HELPER_DIR=$PHP_WMS_HELPER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for wms_helper files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WMS_HELPER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WMS_HELPER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the wms_helper distribution])
  dnl fi

  dnl # --with-wms_helper -> add include path
  dnl PHP_ADD_INCLUDE($WMS_HELPER_DIR/include)

  dnl # --with-wms_helper -> check for lib and symbol presence
  dnl LIBNAME=wms_helper # you may want to change this
  dnl LIBSYMBOL=wms_helper # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WMS_HELPER_DIR/lib, WMS_HELPER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WMS_HELPERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong wms_helper lib version or lib not found])
  dnl ],[
  dnl   -L$WMS_HELPER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WMS_HELPER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(wms_helper, wms_helper.c, $ext_shared)
fi
