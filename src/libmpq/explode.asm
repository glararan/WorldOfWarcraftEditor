﻿; Listing generated by Microsoft (R) Optimizing Compiler Version 16.00.40219.01 

	TITLE	C:\programovani\WoW Editor\Test\src\libmpq\explode.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRT
INCLUDELIB OLDNAMES

PUBLIC	_pkware_copyright
_DATA	SEGMENT
_pkware_copyright DB 'PKWARE Data Compression Library for Win32', 0dH, 0aH
	DB	'Copyright 1989-1995 PKWARE Inc.  All Rights Reserved', 0dH, 0aH
	DB	'Patent No. 5,051,745', 0dH, 0aH, 'PKWARE Data Compression Lib'
	DB	'rary Reg. U.S. Pat. and Tm. Off.', 0dH, 0aH, 'Version 1.11', 0dH
	DB	0aH, 00H
_DATA	ENDS
CONST	SEGMENT
_pkzip_dist_bits DB 02H
	DB	04H
	DB	04H
	DB	05H
	DB	05H
	DB	05H
	DB	05H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	06H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
	DB	08H
_pkzip_dist_code DB 03H
	DB	0dH
	DB	05H
	DB	019H
	DB	09H
	DB	011H
	DB	01H
	DB	03eH
	DB	01eH
	DB	02eH
	DB	0eH
	DB	036H
	DB	016H
	DB	026H
	DB	06H
	DB	03aH
	DB	01aH
	DB	02aH
	DB	0aH
	DB	032H
	DB	012H
	DB	022H
	DB	042H
	DB	02H
	DB	07cH
	DB	03cH
	DB	05cH
	DB	01cH
	DB	06cH
	DB	02cH
	DB	04cH
	DB	0cH
	DB	074H
	DB	034H
	DB	054H
	DB	014H
	DB	064H
	DB	024H
	DB	044H
	DB	04H
	DB	078H
	DB	038H
	DB	058H
	DB	018H
	DB	068H
	DB	028H
	DB	048H
	DB	08H
	DB	0f0H
	DB	070H
	DB	0b0H
	DB	030H
	DB	0d0H
	DB	050H
	DB	090H
	DB	010H
	DB	0e0H
	DB	060H
	DB	0a0H
	DB	020H
	DB	0c0H
	DB	040H
	DB	080H
	DB	00H
_pkzip_clen_bits DB 00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	01H
	DB	02H
	DB	03H
	DB	04H
	DB	05H
	DB	06H
	DB	07H
	DB	08H
_pkzip_len_base DW 00H
	DW	01H
	DW	02H
	DW	03H
	DW	04H
	DW	05H
	DW	06H
	DW	07H
	DW	08H
	DW	0aH
	DW	0eH
	DW	016H
	DW	026H
	DW	046H
	DW	086H
	DW	0106H
_pkzip_slen_bits DB 03H
	DB	02H
	DB	03H
	DB	03H
	DB	04H
	DB	04H
	DB	04H
	DB	05H
	DB	05H
	DB	05H
	DB	05H
	DB	06H
	DB	06H
	DB	06H
	DB	07H
	DB	07H
_pkzip_len_code DB 05H
	DB	03H
	DB	01H
	DB	06H
	DB	0aH
	DB	02H
	DB	0cH
	DB	014H
	DB	04H
	DB	018H
	DB	08H
	DB	030H
	DB	010H
	DB	020H
	DB	040H
	DB	00H
_pkzip_bits_asc DB 0bH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	08H
	DB	07H
	DB	0cH
	DB	0cH
	DB	07H
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0dH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	04H
	DB	0aH
	DB	08H
	DB	0cH
	DB	0aH
	DB	0cH
	DB	0aH
	DB	08H
	DB	07H
	DB	07H
	DB	08H
	DB	09H
	DB	07H
	DB	06H
	DB	07H
	DB	08H
	DB	07H
	DB	06H
	DB	07H
	DB	07H
	DB	07H
	DB	07H
	DB	08H
	DB	07H
	DB	07H
	DB	08H
	DB	08H
	DB	0cH
	DB	0bH
	DB	07H
	DB	09H
	DB	0bH
	DB	0cH
	DB	06H
	DB	07H
	DB	06H
	DB	06H
	DB	05H
	DB	07H
	DB	08H
	DB	08H
	DB	06H
	DB	0bH
	DB	09H
	DB	06H
	DB	07H
	DB	06H
	DB	06H
	DB	07H
	DB	0bH
	DB	06H
	DB	06H
	DB	06H
	DB	07H
	DB	09H
	DB	08H
	DB	09H
	DB	09H
	DB	0bH
	DB	08H
	DB	0bH
	DB	09H
	DB	0cH
	DB	08H
	DB	0cH
	DB	05H
	DB	06H
	DB	06H
	DB	06H
	DB	05H
	DB	06H
	DB	06H
	DB	06H
	DB	05H
	DB	0bH
	DB	07H
	DB	05H
	DB	06H
	DB	05H
	DB	05H
	DB	06H
	DB	0aH
	DB	05H
	DB	05H
	DB	05H
	DB	05H
	DB	08H
	DB	07H
	DB	08H
	DB	08H
	DB	0aH
	DB	0bH
	DB	0bH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0dH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0cH
	DB	0cH
	DB	0cH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
	DB	0dH
_pkzip_code_asc DW 0490H
	DW	0fe0H
	DW	07e0H
	DW	0be0H
	DW	03e0H
	DW	0de0H
	DW	05e0H
	DW	09e0H
	DW	01e0H
	DW	0b8H
	DW	062H
	DW	0ee0H
	DW	06e0H
	DW	022H
	DW	0ae0H
	DW	02e0H
	DW	0ce0H
	DW	04e0H
	DW	08e0H
	DW	0e0H
	DW	0f60H
	DW	0760H
	DW	0b60H
	DW	0360H
	DW	0d60H
	DW	0560H
	DW	01240H
	DW	0960H
	DW	0160H
	DW	0e60H
	DW	0660H
	DW	0a60H
	DW	0fH
	DW	0250H
	DW	038H
	DW	0260H
	DW	050H
	DW	0c60H
	DW	0390H
	DW	0d8H
	DW	042H
	DW	02H
	DW	058H
	DW	01b0H
	DW	07cH
	DW	029H
	DW	03cH
	DW	098H
	DW	05cH
	DW	09H
	DW	01cH
	DW	06cH
	DW	02cH
	DW	04cH
	DW	018H
	DW	0cH
	DW	074H
	DW	0e8H
	DW	068H
	DW	0460H
	DW	090H
	DW	034H
	DW	0b0H
	DW	0710H
	DW	0860H
	DW	031H
	DW	054H
	DW	011H
	DW	021H
	DW	017H
	DW	014H
	DW	0a8H
	DW	028H
	DW	01H
	DW	0310H
	DW	0130H
	DW	03eH
	DW	064H
	DW	01eH
	DW	02eH
	DW	024H
	DW	0510H
	DW	0eH
	DW	036H
	DW	016H
	DW	044H
	DW	030H
	DW	0c8H
	DW	01d0H
	DW	0d0H
	DW	0110H
	DW	048H
	DW	0610H
	DW	0150H
	DW	060H
	DW	088H
	DW	0fa0H
	DW	07H
	DW	026H
	DW	06H
	DW	03aH
	DW	01bH
	DW	01aH
	DW	02aH
	DW	0aH
	DW	0bH
	DW	0210H
	DW	04H
	DW	013H
	DW	032H
	DW	03H
	DW	01dH
	DW	012H
	DW	0190H
	DW	0dH
	DW	015H
	DW	05H
	DW	019H
	DW	08H
	DW	078H
	DW	0f0H
	DW	070H
	DW	0290H
	DW	0410H
	DW	010H
	DW	07a0H
	DW	0ba0H
	DW	03a0H
	DW	0240H
	DW	01c40H
	DW	0c40H
	DW	01440H
	DW	0440H
	DW	01840H
	DW	0840H
	DW	01040H
	DW	040H
	DW	01f80H
	DW	0f80H
	DW	01780H
	DW	0780H
	DW	01b80H
	DW	0b80H
	DW	01380H
	DW	0380H
	DW	01d80H
	DW	0d80H
	DW	01580H
	DW	0580H
	DW	01980H
	DW	0980H
	DW	01180H
	DW	0180H
	DW	01e80H
	DW	0e80H
	DW	01680H
	DW	0680H
	DW	01a80H
	DW	0a80H
	DW	01280H
	DW	0280H
	DW	01c80H
	DW	0c80H
	DW	01480H
	DW	0480H
	DW	01880H
	DW	0880H
	DW	01080H
	DW	080H
	DW	01f00H
	DW	0f00H
	DW	01700H
	DW	0700H
	DW	01b00H
	DW	0b00H
	DW	01300H
	DW	0da0H
	DW	05a0H
	DW	09a0H
	DW	01a0H
	DW	0ea0H
	DW	06a0H
	DW	0aa0H
	DW	02a0H
	DW	0ca0H
	DW	04a0H
	DW	08a0H
	DW	0a0H
	DW	0f20H
	DW	0720H
	DW	0b20H
	DW	0320H
	DW	0d20H
	DW	0520H
	DW	0920H
	DW	0120H
	DW	0e20H
	DW	0620H
	DW	0a20H
	DW	0220H
	DW	0c20H
	DW	0420H
	DW	0820H
	DW	020H
	DW	0fc0H
	DW	07c0H
	DW	0bc0H
	DW	03c0H
	DW	0dc0H
	DW	05c0H
	DW	09c0H
	DW	01c0H
	DW	0ec0H
	DW	06c0H
	DW	0ac0H
	DW	02c0H
	DW	0cc0H
	DW	04c0H
	DW	08c0H
	DW	0c0H
	DW	0f40H
	DW	0740H
	DW	0b40H
	DW	0340H
	DW	0300H
	DW	0d40H
	DW	01d00H
	DW	0d00H
	DW	01500H
	DW	0540H
	DW	0500H
	DW	01900H
	DW	0900H
	DW	0940H
	DW	01100H
	DW	0100H
	DW	01e00H
	DW	0e00H
	DW	0140H
	DW	01600H
	DW	0600H
	DW	01a00H
	DW	0e40H
	DW	0640H
	DW	0a40H
	DW	0a00H
	DW	01200H
	DW	0200H
	DW	01c00H
	DW	0c00H
	DW	01400H
	DW	0400H
	DW	01800H
	DW	0800H
	DW	01000H
	DW	00H
; Function compile flags: /Ogtp
CONST	ENDS
_TEXT	SEGMENT
_skip_bit PROC
; _mpq_pkzip$ = esi
; _bits$ = ebx
; File c:\programovani\wow editor\test\src\libmpq\explode.c
; Line 122
	mov	ecx, DWORD PTR [esi+24]
	cmp	ebx, ecx
	ja	SHORT $LN3@skip_bit
; Line 124
	sub	ecx, ebx
	mov	DWORD PTR [esi+24], ecx
; Line 125
	mov	ecx, ebx
	shr	DWORD PTR [esi+20], cl
; Line 126
	xor	eax, eax
; Line 148
	ret	0
$LN3@skip_bit:
; Line 131
	mov	eax, DWORD PTR [esi+28]
	shr	DWORD PTR [esi+20], cl
	push	edi
	lea	edi, DWORD PTR [esi+28]
	cmp	eax, DWORD PTR [esi+32]
	jne	SHORT $LN2@skip_bit
; Line 134
	mov	ecx, DWORD PTR [esi+36]
	mov	eax, DWORD PTR [esi+40]
	push	ecx
	lea	edx, DWORD PTR [esi+8756]
	push	edi
	push	edx
	mov	DWORD PTR [edi], 2048			; 00000800H
	call	eax
	add	esp, 12					; 0000000cH
	mov	DWORD PTR [esi+32], eax
	test	eax, eax
	jne	SHORT $LN1@skip_bit
; Line 136
	mov	eax, 1
	pop	edi
; Line 148
	ret	0
$LN1@skip_bit:
; Line 138
	mov	DWORD PTR [edi], 0
$LN2@skip_bit:
; Line 142
	mov	eax, DWORD PTR [edi]
	movzx	ecx, BYTE PTR [eax+esi+8756]
	inc	eax
	shl	ecx, 8
	or	DWORD PTR [esi+20], ecx
	mov	DWORD PTR [edi], eax
; Line 143
	mov	eax, DWORD PTR [esi+24]
	mov	ecx, ebx
	sub	ecx, eax
	shr	DWORD PTR [esi+20], cl
; Line 144
	sub	eax, ebx
	add	eax, 8
	mov	DWORD PTR [esi+24], eax
; Line 147
	xor	eax, eax
	pop	edi
; Line 148
	ret	0
_skip_bit ENDP
; Function compile flags: /Ogtp
tv178 = -4						; size = 4
_generate_tables_decode PROC
; _count$ = edx
; _bits$ = eax
; _code$ = ecx
; _buf2$ = edi
; Line 152
	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
; Line 157
	mov	ebx, edx
	dec	ebx
	js	SHORT $LN4@generate_t
	sub	ecx, eax
	push	esi
	lea	esi, DWORD PTR [ebx+eax]
	mov	DWORD PTR tv178[ebp], ecx
	jmp	SHORT $LN6@generate_t
$LL12@generate_t:
	mov	ecx, DWORD PTR tv178[ebp]
$LN6@generate_t:
; Line 160
	movzx	eax, BYTE PTR [ecx+esi]
; Line 161
	mov	cl, BYTE PTR [esi]
	mov	edx, 1
	shl	edx, cl
$LL3@generate_t:
; Line 166
	mov	BYTE PTR [eax+edi], bl
; Line 167
	add	eax, edx
; Line 169
	cmp	eax, 256				; 00000100H
	jb	SHORT $LL3@generate_t
; Line 157
	dec	esi
	dec	ebx
	jns	SHORT $LL12@generate_t
	pop	esi
$LN4@generate_t:
	pop	ebx
; Line 171
	mov	esp, ebp
	pop	ebp
	ret	0
_generate_tables_decode ENDP
; Function compile flags: /Ogtp
_generate_tables_ascii PROC
; _mpq_pkzip$ = edi
; Line 175
	push	ebx
	push	esi
; Line 177
	mov	ebx, OFFSET _pkzip_code_asc+510
; Line 183
	mov	edx, 255				; 000000ffH
	npad	4
$LL21@generate_t@2:
; Line 185
	movzx	eax, dx
; Line 186
	mov	cl, BYTE PTR [eax+edi+12212]
	lea	eax, DWORD PTR [eax+edi+12212]
; Line 189
	cmp	cl, 8
	ja	SHORT $LN18@generate_t@2
; Line 192
	movzx	eax, WORD PTR [ebx]
	mov	esi, 1
	shl	esi, cl
$LL17@generate_t@2:
; Line 195
	mov	BYTE PTR [edi+eax+11316], dl
; Line 196
	add	eax, esi
; Line 198
	cmp	eax, 256				; 00000100H
	jb	SHORT $LL17@generate_t@2
; Line 200
	jmp	$LN1@generate_t@2
$LN18@generate_t@2:
; Line 202
	movzx	esi, WORD PTR [ebx]
	and	esi, 255				; 000000ffH
	je	SHORT $LN13@generate_t@2
; Line 204
	mov	BYTE PTR [esi+edi+11316], 255		; 000000ffH
; Line 205
	test	BYTE PTR [ebx], 63			; 0000003fH
; Line 210
	mov	esi, 1
	je	SHORT $LN12@generate_t@2
; Line 209
	add	cl, 252					; 000000fcH
	mov	BYTE PTR [eax], cl
; Line 211
	movzx	eax, WORD PTR [ebx]
	shl	esi, cl
	shr	eax, 4
	npad	1
$LL11@generate_t@2:
; Line 214
	mov	BYTE PTR [edi+eax+11572], dl
; Line 215
	add	eax, esi
; Line 217
	cmp	eax, 256				; 00000100H
	jb	SHORT $LL11@generate_t@2
; Line 219
	jmp	SHORT $LN1@generate_t@2
$LN12@generate_t@2:
; Line 222
	add	cl, 250					; 000000faH
; Line 223
	mov	BYTE PTR [eax], cl
; Line 225
	movzx	eax, WORD PTR [ebx]
	shl	esi, cl
	shr	eax, 6
	npad	1
$LL7@generate_t@2:
; Line 228
	mov	BYTE PTR [edi+eax+11828], dl
; Line 229
	add	eax, esi
; Line 231
	cmp	eax, 128				; 00000080H
	jb	SHORT $LL7@generate_t@2
; Line 234
	jmp	SHORT $LN1@generate_t@2
$LN13@generate_t@2:
; Line 237
	add	cl, 248					; 000000f8H
; Line 239
	mov	esi, 1
	mov	BYTE PTR [eax], cl
; Line 240
	movzx	eax, BYTE PTR [ebx+1]
	shl	esi, cl
$LL3@generate_t@2:
; Line 243
	mov	BYTE PTR [edi+eax+11956], dl
; Line 244
	add	eax, esi
; Line 246
	cmp	eax, 256				; 00000100H
	jb	SHORT $LL3@generate_t@2
$LN1@generate_t@2:
; Line 183
	sub	ebx, 2
	add	edx, 65535				; 0000ffffH
	cmp	ebx, OFFSET _pkzip_code_asc
	jae	$LL21@generate_t@2
; Line 250
	pop	esi
	pop	ebx
	ret	0
_generate_tables_ascii ENDP
; Function compile flags: /Ogtp
_value$ = -4						; size = 4
_decode_literal PROC
; _mpq_pkzip$ = eax
; Line 260
	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	mov	esi, eax
; Line 268
	mov	ebx, 1
	push	edi
	test	BYTE PTR [esi+20], bl
	je	$LN17@decode_lit
; Line 271
	call	_skip_bit
	test	eax, eax
	je	SHORT $LN16@decode_lit
$LN26@decode_lit:
	pop	edi
	pop	esi
; Line 273
	mov	eax, 774				; 00000306H
	pop	ebx
; Line 378
	mov	esp, ebp
	pop	ebp
	ret	0
$LN16@decode_lit:
; Line 277
	mov	eax, DWORD PTR [esi+20]
	and	eax, 255				; 000000ffH
	movzx	edi, BYTE PTR [eax+esi+11060]
; Line 280
	movzx	ebx, BYTE PTR [edi+esi+12532]
	mov	DWORD PTR _value$[ebp], edi
	call	_skip_bit
	test	eax, eax
; Line 282
	jne	SHORT $LN26@decode_lit
; Line 286
	movzx	ecx, BYTE PTR [edi+esi+12548]
	test	ecx, ecx
	je	SHORT $LN14@decode_lit
; Line 289
	mov	edi, 1
	shl	edi, cl
; Line 292
	mov	ebx, ecx
	dec	edi
	and	edi, DWORD PTR [esi+20]
	call	_skip_bit
	test	eax, eax
	je	SHORT $LN22@decode_lit
; Line 295
	mov	ecx, DWORD PTR _value$[ebp]
	lea	edx, DWORD PTR [edi+ecx]
	cmp	edx, 270				; 0000010eH
; Line 297
	jne	SHORT $LN26@decode_lit
$LN22@decode_lit:
; Line 302
	mov	eax, DWORD PTR _value$[ebp]
	movzx	ecx, WORD PTR [esi+eax*2+12564]
	add	ecx, edi
	mov	edi, ecx
$LN14@decode_lit:
; Line 306
	lea	eax, DWORD PTR [edi+256]
	pop	edi
	pop	esi
	pop	ebx
; Line 378
	mov	esp, ebp
	pop	ebp
	ret	0
$LN17@decode_lit:
; Line 310
	call	_skip_bit
	test	eax, eax
; Line 312
	jne	$LN26@decode_lit
; Line 316
	cmp	DWORD PTR [esi+4], eax
	jne	SHORT $LN10@decode_lit
; Line 319
	movzx	edi, BYTE PTR [esi+20]
; Line 322
	lea	ebx, DWORD PTR [eax+8]
; Line 328
	jmp	$LN25@decode_lit
$LN10@decode_lit:
; Line 332
	mov	ecx, DWORD PTR [esi+20]
	mov	eax, ecx
	and	eax, 255				; 000000ffH
	je	SHORT $LN8@decode_lit
; Line 335
	movzx	edi, BYTE PTR [eax+esi+11316]
; Line 338
	cmp	edi, 255				; 000000ffH
	jne	SHORT $LN2@decode_lit
; Line 340
	test	cl, 63					; 0000003fH
	je	SHORT $LN6@decode_lit
; Line 343
	mov	ebx, 4
	call	_skip_bit
	test	eax, eax
; Line 345
	jne	$LN26@decode_lit
; Line 349
	mov	edx, DWORD PTR [esi+20]
	and	edx, edi
	movzx	edi, BYTE PTR [edx+esi+11572]
; Line 351
	jmp	SHORT $LN2@decode_lit
$LN6@decode_lit:
; Line 354
	mov	ebx, 6
	call	_skip_bit
	test	eax, eax
; Line 356
	jne	$LN26@decode_lit
; Line 360
	mov	eax, DWORD PTR [esi+20]
	and	eax, 127				; 0000007fH
	movzx	edi, BYTE PTR [eax+esi+11828]
; Line 364
	jmp	SHORT $LN2@decode_lit
$LN8@decode_lit:
; Line 367
	mov	ebx, 8
	call	_skip_bit
	test	eax, eax
; Line 369
	jne	$LN26@decode_lit
; Line 373
	mov	ecx, DWORD PTR [esi+20]
	and	ecx, 255				; 000000ffH
	movzx	edi, BYTE PTR [ecx+esi+11956]
$LN2@decode_lit:
; Line 377
	movzx	ebx, BYTE PTR [edi+esi+12212]
$LN25@decode_lit:
	call	_skip_bit
	test	eax, eax
	mov	eax, 774				; 00000306H
	jne	SHORT $LN18@decode_lit
	mov	eax, edi
$LN18@decode_lit:
	pop	edi
	pop	esi
	pop	ebx
; Line 378
	mov	esp, ebp
	pop	ebp
	ret	0
_decode_literal ENDP
; Function compile flags: /Ogtp
_pos$ = -4						; size = 4
_length$ = 8						; size = 4
_decode_distance PROC
; _mpq_pkzip$ = eax
; Line 382
	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	mov	esi, eax
; Line 384
	mov	eax, DWORD PTR [esi+20]
	and	eax, 255				; 000000ffH
	push	edi
	movzx	edi, BYTE PTR [eax+esi+10804]
; Line 387
	movzx	ebx, BYTE PTR [edi+esi+12468]
	mov	DWORD PTR _pos$[ebp], edi
; Line 390
	call	_skip_bit
	cmp	eax, 1
; Line 392
	je	SHORT $LN8@decode_dis
; Line 396
	mov	ebx, 2
	cmp	DWORD PTR _length$[ebp], ebx
	jne	SHORT $LN4@decode_dis
; Line 398
	mov	ecx, DWORD PTR [esi+20]
	add	edi, edi
	add	edi, edi
	and	ecx, 3
	or	edi, ecx
$LN9@decode_dis:
; Line 401
	call	_skip_bit
	cmp	eax, 1
	jne	SHORT $LN1@decode_dis
$LN8@decode_dis:
	pop	edi
	pop	esi
; Line 403
	xor	eax, eax
	pop	ebx
; Line 419
	mov	esp, ebp
	pop	ebp
	ret	0
$LN4@decode_dis:
; Line 408
	mov	ecx, DWORD PTR [esi+12]
	mov	eax, DWORD PTR _pos$[ebp]
	mov	edi, DWORD PTR [esi+16]
	and	edi, DWORD PTR [esi+20]
	shl	eax, cl
; Line 411
	mov	ebx, ecx
	or	edi, eax
; Line 413
	jmp	SHORT $LN9@decode_dis
$LN1@decode_dis:
; Line 418
	lea	eax, DWORD PTR [edi+1]
	pop	edi
	pop	esi
	pop	ebx
; Line 419
	mov	esp, ebp
	pop	ebp
	ret	0
_decode_distance ENDP
_TEXT	ENDS
EXTRN	_memcpy:PROC
; Function compile flags: /Ogtp
_TEXT	SEGMENT
_buf$ = 8						; size = 4
_size$ = 12						; size = 4
_param$ = 16						; size = 4
_data_read_input PROC
; Line 431
	push	ebp
	mov	ebp, esp
; Line 435
	mov	edx, DWORD PTR _size$[ebp]
	push	esi
	mov	esi, DWORD PTR [edx]
	push	edi
	mov	edi, DWORD PTR _param$[ebp]
	mov	ecx, DWORD PTR [edi+4]
	mov	eax, DWORD PTR [edi+8]
	sub	eax, ecx
; Line 438
	cmp	esi, eax
	jbe	SHORT $LN1@data_read_
; Line 440
	mov	esi, eax
$LN1@data_read_:
; Line 444
	mov	eax, DWORD PTR [edi]
	add	eax, ecx
	mov	ecx, DWORD PTR _buf$[ebp]
	push	esi
	push	eax
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH
; Line 445
	add	DWORD PTR [edi+4], esi
	pop	edi
; Line 448
	mov	eax, esi
	pop	esi
; Line 449
	pop	ebp
	ret	0
_data_read_input ENDP
; Function compile flags: /Ogtp
_buf$ = 8						; size = 4
_size$ = 12						; size = 4
_param$ = 16						; size = 4
_data_write_output PROC
; Line 460
	push	ebp
	mov	ebp, esp
; Line 464
	mov	edx, DWORD PTR _size$[ebp]
	push	esi
	mov	esi, DWORD PTR [edx]
	push	edi
	mov	edi, DWORD PTR _param$[ebp]
	mov	ecx, DWORD PTR [edi+16]
	mov	eax, DWORD PTR [edi+20]
	sub	eax, ecx
; Line 467
	cmp	esi, eax
	jbe	SHORT $LN1@data_write
; Line 469
	mov	esi, eax
$LN1@data_write:
; Line 473
	mov	eax, DWORD PTR _buf$[ebp]
	mov	edx, DWORD PTR [edi+12]
	push	esi
	push	eax
	add	edx, ecx
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH
; Line 474
	add	DWORD PTR [edi+16], esi
	pop	edi
	pop	esi
; Line 475
	pop	ebp
	ret	0
_data_write_output ENDP
; Function compile flags: /Ogtp
_copy_length$2408 = -12				; size = 4
_pos$2474 = -8						; size = 4
_copy_bytes$ = -4					; size = 4
_expand	PROC
; _mpq_pkzip$ = eax
; Line 479
	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	esi, eax
	push	edi
; Line 490
	mov	DWORD PTR [esi+8], 4096			; 00001000H
; Line 493
	call	_decode_literal
	mov	edi, eax
	cmp	eax, 773				; 00000305H
	jae	$LN7@expand
	push	ebx
$LL8@expand:
; Line 496
	cmp	eax, 256				; 00000100H
	jb	$LN6@expand
; Line 505
	lea	edi, DWORD PTR [eax-254]
; Line 509
	mov	eax, DWORD PTR [esi+20]
	and	eax, 255				; 000000ffH
	movzx	eax, BYTE PTR [eax+esi+10804]
	movzx	ebx, BYTE PTR [eax+esi+12468]
	mov	DWORD PTR _copy_length$2408[ebp], edi
	mov	DWORD PTR _pos$2474[ebp], eax
	call	_skip_bit
	cmp	eax, 1
	je	$LN28@expand
	cmp	edi, 2
	jne	SHORT $LN14@expand
	mov	edi, DWORD PTR [esi+20]
	mov	ecx, DWORD PTR _pos$2474[ebp]
	and	edi, 3
	lea	edx, DWORD PTR [ecx*4]
	or	edi, edx
	mov	ebx, 2
	jmp	SHORT $LN31@expand
$LN14@expand:
	mov	ecx, DWORD PTR [esi+12]
	mov	eax, DWORD PTR _pos$2474[ebp]
	mov	edi, DWORD PTR [esi+16]
	and	edi, DWORD PTR [esi+20]
	shl	eax, cl
	mov	ebx, ecx
	or	edi, eax
$LN31@expand:
	call	_skip_bit
	cmp	eax, 1
	je	$LN28@expand
	lea	ecx, DWORD PTR [edi+1]
	test	ecx, ecx
	je	$LN28@expand
; Line 516
	mov	edx, DWORD PTR [esi+8]
	lea	edi, DWORD PTR [edx+esi+48]
; Line 517
	mov	eax, edi
	sub	eax, ecx
; Line 518
	mov	ecx, DWORD PTR _copy_length$2408[ebp]
	add	edx, ecx
	mov	DWORD PTR [esi+8], edx
; Line 521
	test	ecx, ecx
	je	SHORT $LN27@expand
$LL4@expand:
; Line 523
	mov	dl, BYTE PTR [eax]
	mov	BYTE PTR [edi], dl
	inc	edi
	inc	eax
	dec	ecx
	jne	SHORT $LL4@expand
; Line 526
	jmp	SHORT $LN27@expand
$LN6@expand:
; Line 529
	mov	ecx, DWORD PTR [esi+8]
	mov	BYTE PTR [esi+ecx+48], al
	inc	DWORD PTR [esi+8]
$LN27@expand:
; Line 533
	cmp	DWORD PTR [esi+8], 8192			; 00002000H
	jb	SHORT $LN1@expand
; Line 537
	mov	edx, DWORD PTR [esi+36]
	mov	ecx, DWORD PTR [esi+44]
	push	edx
	lea	eax, DWORD PTR _copy_bytes$[ebp]
	push	eax
	lea	edi, DWORD PTR [esi+4144]
	push	edi
	mov	DWORD PTR _copy_bytes$[ebp], 4096	; 00001000H
	call	ecx
; Line 540
	mov	edx, DWORD PTR [esi+8]
	sub	edx, 4096				; 00001000H
	push	edx
	lea	eax, DWORD PTR [esi+48]
	push	edi
	push	eax
	call	_memcpy
	add	esp, 24					; 00000018H
; Line 541
	add	DWORD PTR [esi+8], -4096		; fffff000H
$LN1@expand:
; Line 493
	mov	eax, esi
	call	_decode_literal
	mov	edi, eax
	cmp	eax, 773				; 00000305H
	jb	$LL8@expand
	jmp	SHORT $LN29@expand
$LN28@expand:
; Line 511
	mov	edi, 774				; 00000306H
$LN29@expand:
	pop	ebx
$LN7@expand:
; Line 546
	mov	ecx, DWORD PTR [esi+8]
; Line 547
	mov	edx, DWORD PTR [esi+36]
	sub	ecx, 4096				; 00001000H
	push	edx
	mov	edx, DWORD PTR [esi+44]
	lea	eax, DWORD PTR _copy_bytes$[ebp]
	mov	DWORD PTR _copy_bytes$[ebp], ecx
	push	eax
	lea	ecx, DWORD PTR [esi+4144]
	push	ecx
	call	edx
	add	esp, 12					; 0000000cH
; Line 550
	mov	eax, edi
	pop	edi
	pop	esi
; Line 551
	mov	esp, ebp
	pop	ebp
	ret	0
_expand	ENDP
_TEXT	ENDS
PUBLIC	_libmpq__do_decompress_pkzip
EXTRN	_memset:PROC
; Function compile flags: /Ogtp
_TEXT	SEGMENT
_work_buf$ = 8						; size = 4
_param$ = 12						; size = 4
_libmpq__do_decompress_pkzip PROC
; Line 555
	push	ebp
	mov	ebp, esp
	push	ebx
; Line 560
	mov	ebx, DWORD PTR _work_buf$[ebp]
	push	esi
	push	edi
	push	12596					; 00003134H
	push	0
	push	ebx
	call	_memset
; Line 565
	mov	eax, DWORD PTR _param$[ebp]
; Line 567
	push	eax
	lea	esi, DWORD PTR [ebx+28]
	lea	edi, DWORD PTR [ebx+8756]
	push	esi
	push	edi
	mov	DWORD PTR [ebx+40], OFFSET _data_read_input
	mov	DWORD PTR [ebx+44], OFFSET _data_write_output
	mov	DWORD PTR [ebx+36], eax
	mov	DWORD PTR [esi], 2048			; 00000800H
	call	_data_read_input
	add	esp, 24					; 00000018H
	mov	DWORD PTR [ebx+32], eax
; Line 570
	cmp	eax, 4
	ja	SHORT $LN6@libmpq__do
; Line 572
	pop	edi
	pop	esi
	mov	eax, 3
	pop	ebx
; Line 631
	pop	ebp
	ret	0
$LN6@libmpq__do:
; Line 579
	movzx	eax, BYTE PTR [ebx+8757]
	movzx	edx, BYTE PTR [edi]
; Line 582
	movzx	ecx, BYTE PTR [ebx+8758]
	mov	DWORD PTR [ebx+4], edx
	mov	DWORD PTR [ebx+12], eax
	mov	DWORD PTR [ebx+20], ecx
; Line 585
	mov	DWORD PTR [ebx+24], 0
; Line 588
	mov	DWORD PTR [esi], 3
; Line 591
	cmp	eax, 4
	jb	$LN4@libmpq__do
	cmp	eax, 6
	ja	$LN4@libmpq__do
; Line 597
	mov	ecx, 16					; 00000010H
	sub	ecx, eax
	mov	eax, 65535				; 0000ffffH
	sar	eax, cl
	mov	DWORD PTR [ebx+16], eax
; Line 600
	test	edx, edx
	je	SHORT $LN3@libmpq__do
; Line 603
	cmp	edx, 1
	je	SHORT $LN2@libmpq__do
; Line 605
	pop	edi
	pop	esi
	mov	eax, 2
	pop	ebx
; Line 631
	pop	ebp
	ret	0
$LN2@libmpq__do:
; Line 609
	lea	edi, DWORD PTR [ebx+12212]
	mov	ecx, 64					; 00000040H
	mov	esi, OFFSET _pkzip_bits_asc
	rep movsd
; Line 610
	mov	edi, ebx
	call	_generate_tables_ascii
$LN3@libmpq__do:
; Line 614
	mov	ecx, DWORD PTR _pkzip_slen_bits
	mov	DWORD PTR [ebx+12532], ecx
	mov	edx, DWORD PTR _pkzip_slen_bits+4
	lea	eax, DWORD PTR [ebx+12532]
	mov	DWORD PTR [eax+4], edx
	mov	ecx, DWORD PTR _pkzip_slen_bits+8
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _pkzip_slen_bits+12
	mov	DWORD PTR [eax+12], edx
; Line 615
	lea	edi, DWORD PTR [ebx+11060]
	mov	ecx, OFFSET _pkzip_len_code
	mov	edx, 16					; 00000010H
	call	_generate_tables_decode
; Line 618
	mov	eax, DWORD PTR _pkzip_clen_bits
	mov	DWORD PTR [ebx+12548], eax
	mov	ecx, DWORD PTR _pkzip_clen_bits+4
	mov	DWORD PTR [ebx+12552], ecx
	mov	edx, DWORD PTR _pkzip_clen_bits+8
	mov	DWORD PTR [ebx+12556], edx
	mov	eax, DWORD PTR _pkzip_clen_bits+12
	mov	DWORD PTR [ebx+12560], eax
; Line 619
	lea	edi, DWORD PTR [ebx+12564]
	mov	ecx, 8
	mov	esi, OFFSET _pkzip_len_base
	rep movsd
; Line 620
	lea	eax, DWORD PTR [ebx+12468]
	mov	edi, eax
	mov	ecx, 16					; 00000010H
	mov	esi, OFFSET _pkzip_dist_bits
	rep movsd
; Line 621
	lea	edi, DWORD PTR [ebx+10804]
	mov	ecx, OFFSET _pkzip_dist_code
	mov	edx, 64					; 00000040H
	call	_generate_tables_decode
; Line 624
	mov	eax, ebx
	call	_expand
	sub	eax, 774				; 00000306H
	neg	eax
	sbb	eax, eax
	pop	edi
	and	eax, -4					; fffffffcH
	pop	esi
	add	eax, 4
	pop	ebx
; Line 631
	pop	ebp
	ret	0
$LN4@libmpq__do:
	pop	edi
	pop	esi
; Line 593
	mov	eax, 1
	pop	ebx
; Line 631
	pop	ebp
	ret	0
_libmpq__do_decompress_pkzip ENDP
_TEXT	ENDS
END
