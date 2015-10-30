#!/bin/sh

installroot=${PWD}/prefix

(
	cd ${installroot}/conf &&
	echo "ServerName localhost:80" >> httpd.conf &&
	perl -pe 's/^\s*#\s*(LoadModule )/$1/g'  < httpd.conf | 
	perl -pe 's/^\s*LoadModule foo_module.+$//g' | 
	perl -pe 's/^(LoadModule mpm_(prefork|worker|simple|motorz)_module )/#$1/g' > httpd.conf-new &&
	mv httpd.conf-new httpd.conf
)

rv=$?

if [ $rv -ne 0 ];then
	exit $rv
fi


(
	cd httpd-test &&
	perl Makefile.PL -apxs ${installroot}/bin/apxs &&
	t/TEST
)
