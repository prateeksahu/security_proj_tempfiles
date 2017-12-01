#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char *text = (char *)malloc(16*sizeof(char));
	text = "Hello World\n";

	unsigned char *key = (unsigned char *)malloc(16*sizeof(char));
	key = "abcdefghijklmnop";

	printf("%s\n%s", key, text);

	AES_KEY *expandedEKeys = malloc(1*sizeof(AES_KEY));

	int esuccess = AES_set_encrypt_key(key, 128, expandedEKeys);

	printf("%s\n", expandedEKeys->rd_key);

	char *cipher = (char *)malloc(16*sizeof(char));

	AES_encrypt(text, cipher, expandedEKeys);

	printf("%s\n", cipher);

	AES_KEY *expandedDKeys = malloc(1*sizeof(AES_KEY));

	int dsuccess = AES_set_decrypt_key(key, 128, expandedDKeys);

	printf("%s\n", expandedDKeys->rd_key);

	char *ouput = (char *)malloc(16*sizeof(char));

	AES_decrypt(cipher, ouput, expandedDKeys);

	printf("%s", ouput);
}