#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

/* 
 *  A function which takes in a filename and returns the contents as a string, 
 *  with all non-alphanumeric (i.e., not A-Z, a-z, or 0-9) removed and characters
 *  converted to uppercase.
 */
char *read_string(const char *filename){
	char *plain_text = malloc(10);
	FILE *file_pointer_text;
    file_pointer_text = fopen(filename, "r");
    if (file_pointer_text == NULL)
    {
        printf("ERROR");
        exit(1);
    }

    int index = 0;
    char current_char;
    int size_line = 10;

    while((current_char = getc(file_pointer_text)) != EOF)
    {
		if (isalnum(current_char))
		{
			plain_text[index++] = toupper(current_char);
		}

		if (index == size_line)
		{
			size_line += 10;
			plain_text = realloc(plain_text, size_line);
		}
    }
	plain_text[index++] = '\0';

    fclose(file_pointer_text);

	return plain_text;
}

/* 
 *  A function which takes in a filename and encrypts the contents using the
 *  key string which is obtained from the "key_filename" file. The encrypted 
 *  message should be returned using the "result" buffer, which you should 
 *  assume has not been initialised or any memory allocated to it.
 */
void encrypt_columnar(const char *message_filename, const char *key_filename, char **result){
	char *string = read_string(message_filename);
    char *key = read_string(key_filename);

    int size_of_rows;
    int size_of_cols;
    int index;
	int x;
	int y;
	int row;
	char temp;

	size_of_cols = strlen(key);

	if (strlen(string) % size_of_cols == 0)
	{
		size_of_rows = strlen(string) / size_of_cols;
	}
	else
	{
		size_of_rows = (strlen(string) + size_of_cols - 1) / size_of_cols;
	}

	char *alphabetical = malloc(size_of_cols);
	strcpy(alphabetical, key);
	for (x = 0; alphabetical[x]; x++)
	{
		for (y = 0; alphabetical[y]; y++)
		{
			if (alphabetical[x] < alphabetical[y])
			{
				temp = alphabetical[x];
				alphabetical[x] = alphabetical[y];
				alphabetical[y] = temp;
			}
		}	
	}

	char *encrypted_string = malloc((size_of_cols * size_of_rows) + 1);
	strcpy(encrypted_string, string);
	index = strlen(string);
			
	if (strlen(string) < (size_of_cols * size_of_rows))
	{
		while (index < (size_of_cols * size_of_rows))
		{
			encrypted_string[index++] = 'X';
		}
	}
	encrypted_string[index++] = '\0';

	/* Iterates through alphabetically sorted key and swaps letters of the
	corresponding letters in string */
	char *temp_swap = malloc(sizeof(key) + 1);
	strcpy(temp_swap, key);
	index = 0;
	for (x = 0; x < size_of_cols; x++)
	{
		for (y = x; y < size_of_cols; y++)
		{
			if (alphabetical[x] == temp_swap[y])
			{
				if (x != y)
				{
					temp = temp_swap[y];
					temp_swap[y] = temp_swap[x];
					temp_swap[x] = temp;

					for (row = 0; row < (size_of_rows * size_of_cols); row += size_of_cols)
					{
						temp = encrypted_string[x + row];
						encrypted_string[x + row] = encrypted_string[y + row];
						encrypted_string[y + row] = temp;
					}
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
	*result = malloc(strlen(encrypted_string) + 1);
	strcpy(*result, encrypted_string);

	free(temp_swap);
	free(key);
	free(alphabetical);
	free(encrypted_string);
	free(string);
}

/* 
 *  A function which takes in a string and decrypts the contents using the
 *  key string which is obtained from the "key_filename" file. The decrypted 
 *  message should be returned using the "result" buffer, which you should 
 *  assume has not been initialised or any memory allocated to it. The function 
 *  should return true if decryption was successful, false if not.
 */
int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
    char *key = read_string(key_filename);
	char *string = read_string(message_filename);

	int size_of_cols;
	int size_of_rows;
	int x;
	int y;
	char temp;
	int index = 0;
	int swapped;
	int check_swapped;
	char *already_swapped = calloc(0, 1);
	int row;

	size_of_cols = strlen(key);
	size_of_rows = strlen(string) / size_of_cols;

	// Decryption is not possible
	if (strlen(string) % size_of_cols != 0)
	{
		return 0;
	}

	char *alphabetical = malloc(size_of_cols);
	strcpy(alphabetical, key);
	for (x = 0; alphabetical[x]; x++)
	{
		for (y = 0; alphabetical[y]; y++)
		{
			if (alphabetical[x] < alphabetical[y])
			{
				temp = alphabetical[x];
				alphabetical[x] = alphabetical[y];
				alphabetical[y] = temp;
			}
		}	
	}

	// Re-swaps letters that need swapping
	char *decrypted_string = malloc((size_of_cols * size_of_rows) + 1);
	strcpy(decrypted_string, string);
	for (x = 0; x < size_of_cols; x++)
	{
		swapped = 0;
		for (y = 0; y < size_of_cols; y++)
		{
			if (alphabetical[x] == key[y])
			{
				for (check_swapped = 0; check_swapped < strlen(already_swapped) + 1; check_swapped++)
				{
					if (already_swapped[check_swapped] == key[y])
					{
						swapped = 1;
						break;
					}
				}

				if (swapped == 0)
				{
					if (x != y)
					{
						already_swapped = realloc(already_swapped, strlen(already_swapped) + 1);
						already_swapped[index++] = alphabetical[y];
						for (row = 0; row < (size_of_rows * size_of_cols); row += size_of_cols)
						{
							temp = decrypted_string[x + row];
							decrypted_string[x + row] = decrypted_string[y + row];
							decrypted_string[y + row] = temp;
						}
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	*result = malloc(strlen(decrypted_string) + 1);
	strcpy(*result, decrypted_string);

	free(decrypted_string);
	free(alphabetical);
	free(string);
	free(key);

	return 1;
}

int main()
{
	const char *text_file = "C:/Users/Luke/Documents/message_test.txt";
	const char *another_file = "C:/Users/Luke/Documents/decrypt_test.txt";
    const char *key_file = "C:/Users/Luke/Documents/key_test.txt";
	char *result;
	char *decrypt;
	int successful;

	encrypt_columnar(text_file, key_file, &result);
	successful = decrypt_columnar(another_file, key_file, &decrypt);

	printf("%s \n", result);
	printf("%d \n", successful);
	if (successful)
	{
		printf("%s \n", decrypt);
	}
	else
	{
		printf("ERROR");
	}

	free(result);
	free(decrypt);

	return 0;
}