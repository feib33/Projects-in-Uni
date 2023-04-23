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
int main(int argc, char const *argv[])
{

    FILE *input_file = fopen(argv[1],"r");

    if(input_file == NULL)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    char *line = calloc(1024, sizeof(char));
    int count = 0;
    certfile_t *head = NULL;
    certfile_t *end_node , *new_node;
    end_node = new_node = (certfile_t*)malloc(sizeof(certfile_t));

    while(fscanf(input_file, "%s\n ", line) != EOF){
        char *certificate = strtok(line, ",");
        char *url = strtok(NULL, ",");

        int valid = check_certificate(certificate, url);

        strcpy(new_node->certificate, certificate);
        strcpy(new_node->url, url);
        new_node->valid = valid;
        // create the head of linked list
        if(count == 0){
            new_node->next = NULL;
            end_node = new_node;
            head = new_node;
        } else {
            // add certfile into linked list
            new_node->next = NULL;
            end_node->next = new_node;
            end_node = new_node;
        }
        count++;
        printf("%d\n", count);
        // reset the new node
        new_node = (certfile_t*)malloc(sizeof(certfile_t));
    }
    write_output(head);
    fclose(input_file);
    free(line);
    free(end_node);
    free(new_node);
    return 0;
}


void write_output(certfile_t *head){
    char *filename = "output";
    FILE *output_file = fopen(filename, "w");
    if (output_file == NULL){
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    while(head != NULL){
        fprintf(output_file, "%s,%s,%d\n", head->certificate, head->url, head->valid);
        head = head->next;
    }
 
  fclose(output_file);
}

int check_match(char *hostname, char *name){
    // name doesnot contain wildcard
    int wildcard = '*';
    char dot = '.';
    if (strchr(name, wildcard) == NULL){
        if (strcmp(hostname, name) == 0 || strstr(name, hostname) != NULL){
            return 1;
        } else {
            return 0;
        }
    } else {
        char *name_rest = strchr(name, dot);
        char *hostname_rest = strchr(hostname, dot);
    
    
        printf("%s\n", name_rest);
        printf("%s\n", hostname_rest);
        if(strcmp(hostname_rest, name_rest) == 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

int check_certificate(char *certificate, char *url){

    BIO *certificate_bio = NULL;
    X509 *cert = NULL;
    X509_NAME *cert_issuer = NULL;
    X509_CINF *cert_inf = NULL;
    STACK_OF(X509_EXTENSION) * ext_list;

    //initialise openSSL
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    //create BIO object to read certificate
    certificate_bio = BIO_new(BIO_s_file());

    //Read certificate into BIO
    if (!(BIO_read_filename(certificate_bio, certificate)))
    {
        fprintf(stderr, "Error in reading cert BIO filename");
        exit(EXIT_FAILURE);
    }
    if (!(cert = PEM_read_bio_X509(certificate_bio, NULL, 0, NULL)))
    {
        fprintf(stderr, "Error in loading certificate");
        exit(EXIT_FAILURE);
    }

    //cert contains the x509 certificate and can be used to analyse the certificate
    

    // check the validation of dates
    int day, sec;
    int notAfter, notBefore;
    ASN1_TIME *from = X509_get_notBefore(cert);
    if (!ASN1_TIME_diff(&day, &sec, from, NULL)){
        fprintf(stderr, "Time format is invalid");
    }
    // cetificate is valid
    if (day >= 0 || sec >= 0){
        notBefore = 1;
    }
    // date is in the future
    if (day < 0 || sec < 0){
        notBefore = 0;
    }

    ASN1_TIME *to = X509_get_notAfter(cert);
    
    if (!ASN1_TIME_diff(&day, &sec, NULL, to)){
        fprintf(stderr, "Time format is invalid");
    }
    // certificate is valid
    if (day >= 0 || sec >= 0){
        notAfter = 1;
    }
    // certificate has expired - valid to date in the past
    if (day < 0 || sec < 0){
        notAfter = 0;
    } 

    if (!(notBefore && notAfter)){
        return 0;
    }


    // check minimum key length of 2048 bits for RSA
    EVP_PKEY *pkey = X509_get_pubkey(cert);
    RSA *rsa = EVP_PKEY_get1_RSA(pkey);
    int rsa_size = RSA_size(rsa);
    if(rsa_size < 256){
      return 0;
    }


    // domain validation
    int common_name_loc = -1;
    char *common_name_str = NULL;
    int cn_match, san_match;
    X509_NAME *subName = X509_get_subject_name(cert);

    common_name_loc = X509_NAME_get_index_by_NID(subName, NID_commonName, -1);
    X509_NAME_ENTRY *common_name_entry = X509_NAME_get_entry(subName, common_name_loc);
    ASN1_STRING *common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
    common_name_str = (char *) ASN1_STRING_data(common_name_asn1);
    cn_match = check_match(url, common_name_str);
    
    


    //List of extensions available at https://www.openssl.org/docs/man1.1.0/crypto/X509_REVOKED_get0_extensions.html
    //Need to check extension exists and is not null
    int san_index = X509_get_ext_by_NID(cert, NID_subject_alt_name, -1);
    if (san_index == -1){
        printf("No san extension found\n");
        san_match = 0;
        if (cn_match || san_match){
            return 1;
        } else {
            return 0;
        }
    }


    X509_EXTENSION *ex = X509_get_ext(cert, san_index);
    ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
    char buff[1024];
    OBJ_obj2txt(buff, 1024, obj, 0);
    san_match = check_match(url, buff);

    if (cn_match || san_match) {
        return 1;
    } else {
        return 0;
    }


    BUF_MEM *bptr = NULL;
    char *buf = NULL;

    BIO *bio = BIO_new(BIO_s_mem());
    if (!X509V3_EXT_print(bio, ex, 0, 0))
    {
        fprintf(stderr, "Error in reading extensions");
    }
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bptr);

    //bptr->data is not NULL terminated - add null character
    buf = (char *)malloc((bptr->length + 1) * sizeof(char));
    memcpy(buf, bptr->data, bptr->length);
    buf[bptr->length] = '\0';

    //Can print or parse value
    //printf("%s\n", buf);

    //*********************
    // End of Example code
    //*********************

    X509_free(cert);
    BIO_free_all(certificate_bio);
    BIO_free_all(bio);
    EVP_PKEY_free(pkey);
    free(buf);
    return 1;
}