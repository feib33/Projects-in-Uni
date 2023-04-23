#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct certfile{
	char certificate[1024];
	char url[1024];
	int valid;
	struct certfile *next;
} certfile_t;

void write_output(certfile_t *head);
int check_certificate(char *certificate, char *url);
int check_match(char *hostname, char *name);
int cn_san_validation(char *url, X509 *cert);
int bc_validation(X509 *cert);
int date_validation(X509 *cert);
int rsa_len_validation(X509 *cert);
int key_usage_validation(X509 *cert);