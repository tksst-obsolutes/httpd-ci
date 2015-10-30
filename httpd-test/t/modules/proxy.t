use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;
use Apache::TestUtil;
use Apache::TestConfig ();

my $num_tests = 17;
if (have_min_apache_version('2.4.7')) {
    $num_tests++;
}
plan tests => $num_tests, need_module 'proxy';

Apache::TestRequest::module("proxy_http_reverse");
Apache::TestRequest::user_agent(requests_redirectable => 0);

my $r = GET("/reverse/");
ok t_cmp($r->code, 200, "reverse proxy to index.html");
ok t_cmp($r->content, qr/^welcome to /, "reverse proxied body");

if (have_cgi) {
    $r = GET("/reverse/modules/cgi/env.pl");
    ok t_cmp($r->code, 200, "reverse proxy to env.pl");
    ok t_cmp($r->content, qr/^APACHE_TEST_HOSTNAME = /, "reverse proxied env.pl response");
    
    $r = GET("/reverse/modules/cgi/env.pl?reverse-proxy");
    ok t_cmp($r->code, 200, "reverse proxy with query string");
    ok t_cmp($r->content, qr/QUERY_STRING = reverse-proxy\n/s, "reverse proxied query string OK");

    $r = GET("/reverse/modules/cgi/nph-dripfeed.pl");
    ok t_cmp($r->code, 200, "reverse proxy to dripfeed CGI");
    ok t_cmp($r->content, "abcdef", "reverse proxied to dripfeed CGI content OK");

    if (have_min_apache_version('2.1.0')) {
        $r = GET("/reverse/modules/cgi/nph-102.pl");
        ## Uncomment next 2 lines and comment out the subsequant 2 lines
        ## when LWP is fixed to work w/ 1xx
        ##ok t_cmp($r->code, 200, "reverse proxy to nph-102");
        ##ok t_cmp($r->content, "this is nph-stdout", "reverse proxy 102 response");
        ok t_cmp($r->code, 102, "reverse proxy to nph-102");
        ok t_cmp($r->content, "", "reverse proxy 102 response");
    } else {
        skip "skipping tests with httpd <2.1.0" foreach (1..2);
    }

} else {
    skip "skipping tests without CGI module" foreach (1..8);
}

if (have_min_apache_version('2.0.55')) {
    # trigger the "proxy decodes abs_path issue": with the bug present, the
    # proxy URI-decodes on the way through, so the origin server receives
    # an abs_path of "/reverse/nonesuch/file%", which it fails to parse and
    # returns a 400 response.
    $r = GET("/reverse/nonesuch/file%25");
    ok t_cmp($r->code, 404, "reverse proxy URI decoding issue, PR 15207");
} else {
    skip "skipping PR 15207 test with httpd < 2.0.55";
}

$r = GET("/reverse/notproxy/local.html");
ok t_cmp($r->code, 200, "ProxyPass not-proxied request");
my $c = $r->content;
chomp $c;
ok t_cmp($c, "hello world", "ProxyPass not-proxied content OK");

if (have_module('alias')) {
    $r = GET("/reverse/perm");
    ok t_cmp($r->code, 301, "reverse proxy of redirect");
    ok t_cmp($r->header("Location"), qr{http://[^/]*/reverse/alias}, "reverse proxy rewrote redirect");

    if (have_module('proxy_balancer')) {
        # More complex reverse mapping case with the balancer, PR 45434
        Apache::TestRequest::module("proxy_http_balancer");
        my $hostport = Apache::TestRequest::hostport();
        $r = GET("/pr45434/redirect-me");
        ok t_cmp($r->code, 301, "reverse proxy of redirect via balancer");
        ok t_cmp($r->header("Location"), "http://$hostport/pr45434/5.html", "reverse proxy via balancer rewrote redirect");
        Apache::TestRequest::module("proxy_http_reverse"); # flip back 
    } else {
        skip "skipping tests without mod_proxy_balancer" foreach (1..2);
    }

} else {
    skip "skipping tests without mod_alias" foreach (1..4);
}

if (have_min_apache_version('2.4.7')) {
    my $pid = fork;
    if ($pid) {
        system './scripts/uds-test.pl';
        exit;
    }
    # give time for the system call to take effect
    sleep 2;
    $r = GET("/uds/");
    ok t_cmp($r->code, 200, "ProxyPass UDS path");
}

