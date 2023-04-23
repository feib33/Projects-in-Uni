# include "certcheck.h"

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

	// scan each line from input_file and check the certificate is valid or not
	// and save it in a linked list
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
		// reset the new node
		new_node = (certfile_t*)malloc(sizeof(certfile_t));
	}
	// write values in output_file 
	write_output(head);
	fclose(input_file);
	free(line);
	free(end_node);
	free(new_node);
	return 0;
}

// write csv file that contains three columns - file path for the certificate, url, 1 or 0
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

// check whether hostname match common name or subject alternative name
int check_match(char *hostname, char *name){
	int wildcard = '*';
	char dot = '.';
	// name doesnot contain wildcard
	if (strchr(name, wildcard) == NULL){
		if (strcmp(hostname, name) == 0 || strstr(name, hostname) != NULL){
			return 1;
		} else {
			return 0;
		}
	} else {
		char *hostname_rest = strchr(hostname, dot);
		char *name_rest = strchr(name, dot);
		if(strcmp(hostname_rest, name_rest) == 0){
			return 1;
		} else {
			return 0;
		}
	}
}

// domain name validation (including SAN extension) and wildcards
int cn_san_validation(char *url, X509 *cert){
	int common_name_loc = -1;
	char *common_name_str = NULL;
	int cn_match, san_match;
	X509_NAME *subName = X509_get_subject_name(cert);

	// get common name and check it with url
	common_name_loc = X509_NAME_get_index_by_NID(subName, NID_commonName, -1);
	X509_NAME_ENTRY *common_name_entry = X509_NAME_get_entry(subName, common_name_loc);
	ASN1_STRING *common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
	common_name_str = (char *) ASN1_STRING_data(common_name_asn1);
	cn_match = check_match(url, common_name_str);

	// get subject alternative name if it is in current certificate
	// and check it with url
	int san_index = X509_get_ext_by_NID(cert, NID_subject_alt_name, -1);
	if (san_index != -1){
		X509_EXTENSION *ex = X509_get_ext(cert, san_index);
		ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
		char buff[1024];
		OBJ_obj2txt(buff, 1024, obj, 0);
		BUF_MEM *bptr = NULL;
		char *buf = NULL;

		BIO *bio = BIO_new(BIO_s_mem());
		if (!X509V3_EXT_print(bio, ex, 0, 0)){
			fprintf(stderr, "Error in reading extensions");
		}
		BIO_flush(bio);
		BIO_get_mem_ptr(bio, &bptr);
		buf = (char *)malloc((bptr->length + 1) * sizeof(char));
		memcpy(buf, bptr->data, bptr->length);
		buf[bptr->length] = '\0';
	
		// match url with each DNS
		char *comma = ", ";
		char *san;
		san = strtok(buf, comma);
		while( san != NULL ) {
			san_match = check_match(url, san);
			if(san_match == 1){
				break;
			}
		}
		san = strtok(NULL, comma);
		free(buf);
		BIO_free_all(bio);
	} else {
		san_match = 0;
	}


	if (cn_match){
		return 1;
	} else if (san_match){
		return 1;
	} else {
		return 0;
	}
}

// validates basic constraint
int bc_validation(X509 *cert){
	// get basic constraint from stack of extensions
	int bc_index = X509_get_ext_by_NID(cert, NID_basic_constraints, -1);
	X509_EXTENSION *ex = X509_get_ext(cert, bc_index);
	ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
	char buff[1024];
	OBJ_obj2txt(buff, 1024, obj, 0);
	BUF_MEM *bptr = NULL;
	char *buf = NULL;
	BIO *bio = BIO_new(BIO_s_mem());
	if (!X509V3_EXT_print(bio, ex, 0, 0))
	{
		fprintf(stderr, "Error in reading extensions");
	}
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bptr);
	buf = (char *)malloc((bptr->length + 1) * sizeof(char));
	memcpy(buf, bptr->data, bptr->length);
	buf[bptr->length] = '\0';

    // false when it marked as a CA
	if (strcmp(buf, "CA:FALSE") == 0){
		return 1;
	} else {
		return 0;
	}
	free(buf);
	BIO_free_all(bio);
}

// validation of dates - Not Before and Not After dates
int date_validation(X509 *cert){
	int day, sec, notAfter, notBefore;

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

	// return true when the date is valid
	if (!(notBefore && notAfter)){
		return 0;
	} else {
		return 1;
	}
}

// make sure that minimum key length is not less than 256 bytes(2048 bits)
int rsa_len_validation(X509 *cert){
	EVP_PKEY *pkey = X509_get_pubkey(cert);
	RSA *rsa = EVP_PKEY_get1_RSA(pkey);
	int rsa_size = RSA_size(rsa);
	if(rsa_size < 256){
		return 0;
	} else {
		return 1;
	}
	EVP_PKEY_free(pkey);
}

// validates key usage
int key_usage_validation(X509 *cert){
	// get key usage from stack of extensions
	int ku_index = X509_get_ext_by_NID(cert, NID_ext_key_usage, -1);
	X509_EXTENSION *ex = X509_get_ext(cert, ku_index);
	ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
	char buff[1024];
	OBJ_obj2txt(buff, 1024, obj, 0);
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
	if (strstr(buf, "TLS Web Server Authentication") != NULL){
		return 1;
	} else {
		return 0;
	}
	free(buf);
	BIO_free_all(bio);
}

// check whether certificate is valid 
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

	// domain validation
	int pass_date_validation = date_validation(cert);
	int pass_cn_san_validation = cn_san_validation(url, cert);
	int pass_rsa_len_validation = rsa_len_validation(cert);
	int pass_bc_validation = bc_validation(cert);
	int pass_key_usage_validation = key_usage_validation(cert);

	// certificate is valid if it pass all detection
	if(pass_date_validation && pass_cn_san_validation && pass_rsa_len_validation 
		&& pass_bc_validation &&pass_key_usage_validation){
		return 1;
	} else {
		return 0;
	}
	X509_free(cert);
	BIO_free_all(certificate_bio);
}