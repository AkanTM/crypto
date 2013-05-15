#include <stdio.h>
#include <gcrypt.h> 
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

char b64string[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

long base64_encode (to, from, len)
	char *to, *from;
	unsigned int len;
{
	char *fromp = from;
	char *top = to;
	unsigned char cbyte;
	unsigned char obyte;
	char end[3];

	for (; len >= 3; len -= 3) {
		cbyte = *fromp++;
		*top++ = b64string[(int)(cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30;		/* 0011 0000 */

		cbyte = *fromp++;
		obyte |= (cbyte >> 4);			/* 0000 1111 */
		*top++ = b64string[(int)obyte];
		obyte = (cbyte << 2) & 0x3C;		/* 0011 1100 */

		cbyte = *fromp++;
		obyte |= (cbyte >> 6);			/* 0000 0011 */
		*top++ = b64string[(int)obyte];
		*top++ = b64string[(int)(cbyte & 0x3F)];/* 0011 1111 */
	}

	if (len) {
		end[0] = *fromp++;
		if (--len) end[1] = *fromp++; else end[1] = 0;
		end[2] = 0;

		cbyte = end[0];
		*top++ = b64string[(int)(cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30;		/* 0011 0000 */

		cbyte = end[1];
		obyte |= (cbyte >> 4);
		*top++ = b64string[(int)obyte];
		obyte = (cbyte << 2) & 0x3C;		/* 0011 1100 */

		if (len) *top++ = b64string[(int)obyte];
		else *top++ = '=';
		*top++ = '=';
	}
	*top = 0;
	return top - to;
}




int main(int argc, char* argv[])
{
	char buffer[1025];
	char *base64;
	char *hash;
	int i;

	if (argc != 2)
	{
		printf("Usage: %s file\n", argv[0]);
		return 1;
	}



	FILE *myfile = fopen(argv[1], "rb");
	

	if ( myfile == NULL )
	{

		printf ("Couldnt read file %s\n", argv[1]);
		return 1;
	}


	


	if (!gcry_check_version(GCRYPT_VERSION)) 
		return 1;


	gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

	gcry_md_hd_t hd; 

	gcry_md_open(&hd, GCRY_MD_WHIRLPOOL, GCRY_MD_FLAG_SECURE);

	
	int len = 0;
	i=0;
	while ( (len = fread (buffer, 1, 1024, myfile)) ) 
	{
		i++;
		gcry_md_write(hd, (void *)buffer, len);
	}

	
	hash = calloc(1024, sizeof(char));
	//gcry_md_hash_buffer(GCRY_MD_WHIRLPOOL, hash, buffer, strlen(buffer));


	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_start = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;


 	hash =  gcry_md_read ( hd, GCRY_MD_WHIRLPOOL);


 	gettimeofday(&tv, NULL);

	double time_stop = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;


	
	base64=calloc(513, sizeof(char));

	base64_encode(base64, hash, strlen(hash));

	printf("%s\n", base64);
	printf("%lf\n", time_stop - time_start);

	// printf("%s\n", base64_encode( hash, sizeof(hash), &len));

	gcry_md_close(hd);

	struct rusage rusage;
	getrusage( RUSAGE_SELF, &rusage );

	printf("%li\n",  rusage.ru_maxrss * 1024L );

	return 0;
} 