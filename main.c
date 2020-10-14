#include "trivium.h"

void main()
{
	FILE *fp, *fd;
	struct trivium_context ctx;
	uint32_t byte, block = 10000;
	uint8_t *buf, *out, key[10], iv[10];

	int res, action = 1;

	char file_ini[] = "C:\\Users\\g\\Documents\\repos\\especialidad\\Cripto\\1.txt";
	char file_fin[] = "C:\\Users\\g\\Documents\\repos\\especialidad\\Cripto\\crypto";

	uint8_t value = 0;

	while((value = getchar()) != '0')
    {
        switch(value)
        {
			case 't': 
				buf = xmalloc(sizeof(uint8_t) * block);
				out = xmalloc(sizeof(uint8_t) * block);
	
				fp = open_file(file_ini, 1);
				fd = open_file(file_fin, 2);
				
				memset(key, 'k', sizeof(key));
				memset(iv, 'i', sizeof(iv));

				if(trivium_set_key_and_iv(&ctx, (uint8_t *)key, 10, iv, 2)) 
				{
					printf("Trivium context filling error!\n");
					exit(1);
				}
				
				while((byte = fread(buf, 1, block, fp)) > 0) 
				{
					if(action == 1)
						trivium_crypt(&ctx, buf, byte, out);
					
					fwrite(out, 1, byte, fd);
				}
				
				free(buf);
				free(out); 
				break;

			case 's':
				printf("asdfasdf");
				break;
		}
	}
}
