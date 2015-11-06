#!/bin/sh

if [ -z "$MAKE_CONCURRENCY" ];then
	MAKE_CONCURRENCY=2
fi

cd httpd &&
./buildconf &&
./configure \
--prefix=${PWD}/../installroot \
--enable-mpms-shared=all \
--enable-mods-shared="all ssl ldap cache proxy file_cache authnz_ldap charset_lite dav_lock disk_cache reallyall" \
--disable-ssl-ct \
--enable-maintainers-mode &&
make -j${MAKE_CONCURRENCY} &&
make install
