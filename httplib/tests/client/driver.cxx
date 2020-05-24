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

  return 0;
}
