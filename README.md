# cpp-httplib

a single header only ( for better or worse ) client and server http(s) library.

problably gets you pwned ( no offense to the author! ).

```cpp
//  derived from: client.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <httplib/httplib.hxx>

#include <iostream>

#define CA_CERT_FILE "./ca-bundle.crt"

int main(int argc, char* argv[]) {
  using namespace std;

  if(argc != 2) {
    cerr << "usage: driver <ca-bundle.crt>" << endl;
    return EXIT_FAILURE;
  }

  auto const* bundle = argv[1];

  httplib::SSLClient cli("google.com");
  cli.set_ca_cert_path(bundle);
  cli.enable_server_certificate_verification(true);

  auto res = cli.Get("/");
  if (res) {
    cout << "status = " << res->status << endl;
    cout << "Content-Type = " << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
    cout << "request failed" << endl;
    auto result = cli.get_openssl_verify_result();
    if (result) {
      cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
    }
  }

  return EXIT_SUCCESS;
}
```

## packaging

as you can see in the above example users of the library have to include
`<httplib/httplib.hxx>`. the `build2` library automatically promotes
`-DCPPHTTPLIB_OPENSSL_SUPPORT` and `-DCPPHTTPLIB_ZLIB_SUPPORT`.

## dependencies

  - openssl ( libcrypto, libssl )
  - libz

`build2` packages are used for the deps but you can always override this for
system deps usage like this:

```shell
$ bdep init -C @clang10 cc \
  config.cxx=clang++10 \
  config.cxx.poptions="-DNDEBUG" \
  config.cxx.coptions="-Wall -Wextra -Ofast -flto -stdlib=libc++" -- \
  '?sys:libcrypto/*' '?sys:libssl/*' '?sys:libz/*'
```

## TODO

  - [ ] package all / more examples
  - [ ] provide `testscript`s
  - [ ] provide a fuzzing target for clang's `-fsanitize=fuzzer`
