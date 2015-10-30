#!/bin/sh

cd httpd &&
./buildconf &&
./configure \
--prefix=${PWD}/../prefix \
--with-included-apr \
--with-ldap \
--enable-mpms-shared=all \
--enable-mods-shared="all ssl ldap cache proxy file_cache authnz_ldap charset_lite dav_lock disk_cache reallyall" \
--disable-ssl-ct \
--enable-maintainers-mode &&
make -j6 &&
make install
