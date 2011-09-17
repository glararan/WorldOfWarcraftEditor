#ifndef _WAVE_H
#define _WAVE_H

/* buffer. */
typedef union
{
	uint16_t	*pw;
	uint8_t		*pb;
} byte_and_int16_t;

/* decompress a wave file, mono or stereo, 1500F230 offset. */
int32_t libmpq__do_decompress_wave(
	uint8_t		*out_buf,
	int32_t		out_length,
	uint8_t		*in_buf,
	int32_t		in_length,
	int32_t		channels
);

#endif