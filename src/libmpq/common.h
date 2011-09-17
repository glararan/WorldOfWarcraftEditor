#ifndef _COMMON_H
#define _COMMON_H

/* function to return the hash to a given string. */
uint32_t libmpq__hash_string(
	const char	*key,
	uint32_t	offset
);

/* function to encrypt a block. */
int32_t libmpq__encrypt_block(
	uint32_t	*in_buf,
	uint32_t	in_size,
	uint32_t	seed
);

/* function to decrypt a block. */
int32_t libmpq__decrypt_block(
	uint32_t	*in_buf,
	uint32_t	in_size,
	uint32_t	seed
);

/* function to detect decryption key. */
int32_t libmpq__decrypt_key(
	uint8_t		*in_buf,
	uint32_t	in_size,
	uint32_t	block_size,
	uint32_t	*key
);

/* function to decompress or explode block from archive. */
int32_t libmpq__decompress_block(
	uint8_t		*in_buf,
	uint32_t	in_size,
	uint8_t		*out_buf,
	uint32_t	out_size,
	uint32_t	compression_type
);

#endif