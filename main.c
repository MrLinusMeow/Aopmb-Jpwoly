#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

union name{
	struct num {
		uint64_t high;
		uint64_t low;
	} num;
	char c[8];
};

void shift(char* arg){
	#define SIZE 16
	char buffer[SIZE];
	union name filename;
	filename.num.low = random();
	filename.num.high = random();
	filename.c[0] = 'K';
	FILE* keyf = fopen(filename.c, "ab");
	
	for( int i = 0; arg[i] != '\0'; ++i){
		char num = '\0';
		if(isalpha(arg[i])){
			printf("input shift number for %c\n", arg[i]);
			char* digit = fgets(buffer, SIZE, stdin);
			arg[i] = toupper(arg[i]) - 'A';
			if(digit[0] == '\n'){
				digit[0] = digit[2];
				digit[2] = '\0';
			}
			if(digit[0] == '\0'){
				digit[0] = digit[1];
				digit[1] = '\0';
			}
			if(isdigit(digit[0])){
				num = digit[0] - '0';
				fputc(digit[0], keyf);
				if(isdigit(digit[1])){
					num *= 10;
					num += digit[1] - '0';
					fputc(digit[1], keyf);
				}else{ digit[1] = '\0'; }
			}else{ printf("Not a number: %x\n", digit[0]); abort(); }
		}else{ printf("Not a letter: %2x\n", arg[i]); abort(); }
		arg[i] = ((arg[i] + num) % 25) + 'A';
		num = '\0';
	}
	fputc( 0x20, keyf);
	fclose(keyf);
	printf("RESULT:\t%s\n", arg);
}

int main(int argc, char* argv[]){
	if(argc < 2){ 
		printf("Usage: %s plaintext1 [plaintext2] [...]\n", argv[0]);
		return 0;
	}
	for( int i = 1; argv[i] != NULL; ++i){
		shift(argv[i]);
	}
	return 0;
}
