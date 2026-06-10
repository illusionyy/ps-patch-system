// clang-format off
static const unsigned long sceVideoOutOpen_offset = 0x2;
static const unsigned long sceVideoOutSetFlipRate_offset = 0xe;
static const unsigned long sceVideoOutConfigureOutput_offset = 0x1a;
static const unsigned long sceVideoOutIsOutputSupported_offset = 0x26;
static const unsigned long sceVideoOutRegisterBuffers_offset = 0x32;
static const unsigned long sceVideodec2CreateDecoder_offset = 0x3e;
static const unsigned long sceVideodec2DeleteDecoder_offset = 0x4a;
static const unsigned long memset_offset = 0x56;
static const unsigned long vsnprintf_offset = 0x62;
static const unsigned long strlen_offset = 0x6e;
static const unsigned long printf_offset = 0x7a;
static const unsigned long sceKernelSendNotificationRequest_offset = 0x86;
static const unsigned long strerror_offset = 0x92;
static const unsigned long __error_offset = 0x9e;
static const unsigned long strncpy_offset = 0xaa;
static const unsigned long __export_sceVideoOutOpen_hook_offset = 0x296;
static const unsigned long __export_sceVideoOutRegisterBuffers_hook_offset = 0x32d;
static const unsigned long __export_sceVideodec2CreateDecoder_hook_offset = 0x38a;
static const unsigned long __export_sceVideodec2DeleteDecoder_hook_offset = 0x3e4;

static const unsigned char shellcode[] = {
                                                                 // sceVideoOutOpen:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideoOutSetFlipRate:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideoOutConfigureOutput:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideoOutIsOutputSupported:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideoOutRegisterBuffers:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideodec2CreateDecoder:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceVideodec2DeleteDecoder:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // memset:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // vsnprintf:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // strlen:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // printf:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // sceKernelSendNotificationRequest:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // strerror:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // __error:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // strncpy:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // notify_fixed_:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x57,                                                  // push	r15
    0x41, 0x56,                                                  // push	r14
    0x41, 0x54,                                                  // push	r12
    0x53,                                                        // push	rbx
    0x48, 0x81, 0xec, 0x30, 0x0c, 0x00, 0x00,                    // sub	rsp, 3120
    0x49, 0x89, 0xd4,                                            // mov	r12, rdx
    0x49, 0x89, 0xf7,                                            // mov	r15, rsi
    0x89, 0xfb,                                                  // mov	ebx, edi
    0x48, 0x8d, 0xbd, 0xb0, 0xf3, 0xff, 0xff,                    // lea	rdi, [rbp - 3152]
    0xba, 0x30, 0x0c, 0x00, 0x00,                                // mov	edx, 3120
    0x31, 0xf6,                                                  // xor	esi, esi
    0xe8, 0x73, 0xff, 0xff, 0xff,                                // call	memset
    0x4c, 0x8d, 0xb5, 0xdd, 0xf3, 0xff, 0xff,                    // lea	r14, [rbp - 3107]
    0xba, 0xff, 0x03, 0x00, 0x00,                                // mov	edx, 1023
    0x4c, 0x89, 0xe6,                                            // mov	rsi, r12
    0xc7, 0x85, 0xc0, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // mov	dword ptr [rbp - 3136], -1
    0x4c, 0x89, 0xf7,                                            // mov	rdi, r14
    0xe8, 0xa6, 0xff, 0xff, 0xff,                                // call	strncpy
    0x4c, 0x89, 0xf7,                                            // mov	rdi, r14
    0xe8, 0x62, 0xff, 0xff, 0xff,                                // call	strlen
    0x48, 0x85, 0xc0,                                            // test	rax, rax
    0x74, 0x17,                                                  // je	L_BB0_3
                                                                 // L_BB0_1:
    0x80, 0xbc, 0x05, 0xdc, 0xf3, 0xff, 0xff, 0x0a,              // cmp	byte ptr [rbp + rax - 3108], 10
    0x75, 0x0d,                                                  // jne	L_BB0_3
    0xc6, 0x84, 0x05, 0xdc, 0xf3, 0xff, 0xff, 0x00,              // mov	byte ptr [rbp + rax - 3108], 0
    0x48, 0xff, 0xc8,                                            // dec	rax
    0x75, 0xe9,                                                  // jne	L_BB0_1
                                                                 // L_BB0_3:
    0x4d, 0x85, 0xff,                                            // test	r15, r15
    0x74, 0x11,                                                  // je	L_BB0_4
    0x41, 0x80, 0x3f, 0x00,                                      // cmp	byte ptr [r15], 0
    0x48, 0x8d, 0x35, 0x22, 0x03, 0x00, 0x00,                    // lea	rsi, [rip + L_.str.1]
    0x49, 0x0f, 0x45, 0xf7,                                      // cmovne	rsi, r15
    0xeb, 0x07,                                                  // jmp	L_BB0_6
                                                                 // L_BB0_4:
    0x48, 0x8d, 0x35, 0x15, 0x03, 0x00, 0x00,                    // lea	rsi, [rip + L_.str.1]
                                                                 // L_BB0_6:
    0x48, 0x8d, 0x3d, 0xff, 0x02, 0x00, 0x00,                    // lea	rdi, [rip + L_.str]
    0x4c, 0x89, 0xf2,                                            // mov	rdx, r14
    0x31, 0xc0,                                                  // xor	eax, eax
    0xe8, 0x24, 0xff, 0xff, 0xff,                                // call	printf
    0x84, 0xdb,                                                  // test	bl, bl
    0x74, 0x15,                                                  // je	L_BB0_8
    0x48, 0x8d, 0xb5, 0xb0, 0xf3, 0xff, 0xff,                    // lea	rsi, [rbp - 3152]
    0xba, 0x30, 0x0c, 0x00, 0x00,                                // mov	edx, 3120
    0x31, 0xff,                                                  // xor	edi, edi
    0x31, 0xc9,                                                  // xor	ecx, ecx
    0xe8, 0x17, 0xff, 0xff, 0xff,                                // call	sceKernelSendNotificationRequest
                                                                 // L_BB0_8:
    0x48, 0x81, 0xc4, 0x30, 0x0c, 0x00, 0x00,                    // add	rsp, 3120
    0x5b,                                                        // pop	rbx
    0x41, 0x5c,                                                  // pop	r12
    0x41, 0x5e,                                                  // pop	r14
    0x41, 0x5f,                                                  // pop	r15
    0x5d,                                                        // pop	rbp
    0xc3,                                                        // ret
                                                                 // notify_:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x57,                                                  // push	r15
    0x41, 0x56,                                                  // push	r14
    0x41, 0x54,                                                  // push	r12
    0x53,                                                        // push	rbx
    0x48, 0x81, 0xec, 0xd0, 0x04, 0x00, 0x00,                    // sub	rsp, 1232
    0x49, 0x89, 0xd7,                                            // mov	r15, rdx
    0x48, 0x89, 0xf3,                                            // mov	rbx, rsi
    0x41, 0x89, 0xfe,                                            // mov	r14d, edi
    0x48, 0x89, 0x8d, 0x28, 0xff, 0xff, 0xff,                    // mov	qword ptr [rbp - 216], rcx
    0x4c, 0x89, 0x85, 0x30, 0xff, 0xff, 0xff,                    // mov	qword ptr [rbp - 208], r8
    0x4c, 0x89, 0x8d, 0x38, 0xff, 0xff, 0xff,                    // mov	qword ptr [rbp - 200], r9
    0x84, 0xc0,                                                  // test	al, al
    0x74, 0x34,                                                  // je	L_BB1_2
    0xc5, 0xf8, 0x29, 0x85, 0x40, 0xff, 0xff, 0xff,              // vmovaps	xmmword ptr [rbp - 192], xmm0
    0xc5, 0xf8, 0x29, 0x8d, 0x50, 0xff, 0xff, 0xff,              // vmovaps	xmmword ptr [rbp - 176], xmm1
    0xc5, 0xf8, 0x29, 0x95, 0x60, 0xff, 0xff, 0xff,              // vmovaps	xmmword ptr [rbp - 160], xmm2
    0xc5, 0xf8, 0x29, 0x9d, 0x70, 0xff, 0xff, 0xff,              // vmovaps	xmmword ptr [rbp - 144], xmm3
    0xc5, 0xf8, 0x29, 0x65, 0x80,                                // vmovaps	xmmword ptr [rbp - 128], xmm4
    0xc5, 0xf8, 0x29, 0x6d, 0x90,                                // vmovaps	xmmword ptr [rbp - 112], xmm5
    0xc5, 0xf8, 0x29, 0x75, 0xa0,                                // vmovaps	xmmword ptr [rbp - 96], xmm6
    0xc5, 0xf8, 0x29, 0x7d, 0xb0,                                // vmovaps	xmmword ptr [rbp - 80], xmm7
                                                                 // L_BB1_2:
    0x4c, 0x8d, 0xa5, 0x10, 0xfb, 0xff, 0xff,                    // lea	r12, [rbp - 1264]
    0xba, 0x00, 0x04, 0x00, 0x00,                                // mov	edx, 1024
    0x31, 0xf6,                                                  // xor	esi, esi
    0x4c, 0x89, 0xe7,                                            // mov	rdi, r12
    0xe8, 0x59, 0xfe, 0xff, 0xff,                                // call	memset
    0x48, 0x8d, 0x85, 0x10, 0xff, 0xff, 0xff,                    // lea	rax, [rbp - 240]
    0x48, 0x8d, 0x4d, 0x10,                                      // lea	rcx, [rbp + 16]
    0xbe, 0xff, 0x03, 0x00, 0x00,                                // mov	esi, 1023
    0x4c, 0x89, 0xe7,                                            // mov	rdi, r12
    0x4c, 0x89, 0xfa,                                            // mov	rdx, r15
    0x48, 0x89, 0x45, 0xd0,                                      // mov	qword ptr [rbp - 48], rax
    0x48, 0x89, 0x4d, 0xc8,                                      // mov	qword ptr [rbp - 56], rcx
    0x48, 0xb8, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,  // movabs	rax, 206158430232
    0x48, 0x8d, 0x4d, 0xc0,                                      // lea	rcx, [rbp - 64]
    0x48, 0x89, 0x45, 0xc0,                                      // mov	qword ptr [rbp - 64], rax
    0xe8, 0x30, 0xfe, 0xff, 0xff,                                // call	vsnprintf
    0x44, 0x89, 0xf7,                                            // mov	edi, r14d
    0x48, 0x89, 0xde,                                            // mov	rsi, rbx
    0x4c, 0x89, 0xe2,                                            // mov	rdx, r12
    0xe8, 0x76, 0xfe, 0xff, 0xff,                                // call	notify_fixed_
    0x48, 0x81, 0xc4, 0xd0, 0x04, 0x00, 0x00,                    // add	rsp, 1232
    0x5b,                                                        // pop	rbx
    0x41, 0x5c,                                                  // pop	r12
    0x41, 0x5e,                                                  // pop	r14
    0x41, 0x5f,                                                  // pop	r15
    0x5d,                                                        // pop	rbp
    0xc3,                                                        // ret
                                                                 // ui_perror_:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x57,                                                  // push	r15
    0x41, 0x56,                                                  // push	r14
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0x48, 0x89, 0xd3,                                            // mov	rbx, rdx
    0x49, 0x89, 0xf6,                                            // mov	r14, rsi
    0x49, 0x89, 0xff,                                            // mov	r15, rdi
    0xe8, 0x36, 0xfe, 0xff, 0xff,                                // call	__error
    0x8b, 0x38,                                                  // mov	edi, dword ptr [rax]
    0xe8, 0x23, 0xfe, 0xff, 0xff,                                // call	strerror
    0x49, 0x89, 0xc0,                                            // mov	r8, rax
    0x48, 0x8d, 0x15, 0xe9, 0x01, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.2]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x48, 0x89, 0xde,                                            // mov	rsi, rbx
    0x4c, 0x89, 0xf9,                                            // mov	rcx, r15
    0x4d, 0x89, 0xf1,                                            // mov	r9, r14
    0x31, 0xc0,                                                  // xor	eax, eax
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x41, 0x5e,                                                  // pop	r14
    0x41, 0x5f,                                                  // pop	r15
    0x5d,                                                        // pop	rbp
    0xe9, 0xe7, 0xfe, 0xff, 0xff,                                // jmp	notify_
                                                                 // __export_sceVideoOutOpen_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0xe8, 0x5f, 0xfd, 0xff, 0xff,                                // call	sceVideoOutOpen
    0x89, 0xc3,                                                  // mov	ebx, eax
    0x89, 0xc1,                                                  // mov	ecx, eax
    0x48, 0x8d, 0x15, 0xbf, 0x01, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.3]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xf6,                                                  // xor	esi, esi
    0x31, 0xc0,                                                  // xor	eax, eax
    0xe8, 0xc3, 0xfe, 0xff, 0xff,                                // call	notify_
    0x89, 0xdf,                                                  // mov	edi, ebx
    0x89, 0x1d, 0xbe, 0x01, 0x00, 0x00,                          // mov	dword ptr [rip + _ZL8g_handle], ebx
    0xe8, 0x09, 0x00, 0x00, 0x00,                                // call	_ZL8set120hzj
    0x89, 0xd8,                                                  // mov	eax, ebx
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x5d,                                                        // pop	rbp
    0xc3,                                                        // ret
                                                                 // _ZL8set120hzj:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x56,                                                  // push	r14
    0x53,                                                        // push	rbx
    0x41, 0x89, 0xfe,                                            // mov	r14d, edi
    0xe8, 0x48, 0x01, 0x00, 0x00,                                // call	_ZL29sceVideoOutIsOutputSupported2jj
    0x89, 0xc3,                                                  // mov	ebx, eax
    0x85, 0xc0,                                                  // test	eax, eax
    0x7e, 0x26,                                                  // jle	L_BB4_2
    0x44, 0x89, 0xf7,                                            // mov	edi, r14d
    0xe8, 0x4b, 0x01, 0x00, 0x00,                                // call	_ZL27sceVideoOutConfigureOutput2jj
    0x41, 0x89, 0xc0,                                            // mov	r8d, eax
    0x48, 0x8d, 0x15, 0x1d, 0x03, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.7]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xf6,                                                  // xor	esi, esi
    0x89, 0xd9,                                                  // mov	ecx, ebx
    0x31, 0xc0,                                                  // xor	eax, eax
    0x5b,                                                        // pop	rbx
    0x41, 0x5e,                                                  // pop	r14
    0x5d,                                                        // pop	rbp
    0xe9, 0x72, 0xfe, 0xff, 0xff,                                // jmp	notify_
                                                                 // L_BB4_2:
    0x48, 0x8d, 0x15, 0x45, 0x03, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.8]
    0x4c, 0x8d, 0x05, 0x6c, 0x03, 0x00, 0x00,                    // lea	r8, [rip + L_.str.10]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xf6,                                                  // xor	esi, esi
    0x89, 0xd9,                                                  // mov	ecx, ebx
    0x31, 0xc0,                                                  // xor	eax, eax
    0x5b,                                                        // pop	rbx
    0x41, 0x5e,                                                  // pop	r14
    0x5d,                                                        // pop	rbp
    0xe9, 0x50, 0xfe, 0xff, 0xff,                                // jmp	notify_
                                                                 // __export_sceVideoOutRegisterBuffers_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x57,                                                  // push	r15
    0x41, 0x56,                                                  // push	r14
    0x41, 0x55,                                                  // push	r13
    0x41, 0x54,                                                  // push	r12
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0x49, 0x89, 0xd7,                                            // mov	r15, rdx
    0x48, 0x8d, 0x15, 0x3f, 0x01, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.4]
    0x41, 0x89, 0xf4,                                            // mov	r12d, esi
    0x41, 0x89, 0xfd,                                            // mov	r13d, edi
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xc0,                                                  // xor	eax, eax
    0x4c, 0x89, 0xc3,                                            // mov	rbx, r8
    0x41, 0x89, 0xce,                                            // mov	r14d, ecx
    0x48, 0x89, 0xd6,                                            // mov	rsi, rdx
    0xe8, 0x1d, 0xfe, 0xff, 0xff,                                // call	notify_
    0x44, 0x89, 0xef,                                            // mov	edi, r13d
    0xe8, 0x68, 0xff, 0xff, 0xff,                                // call	_ZL8set120hzj
    0x44, 0x89, 0xef,                                            // mov	edi, r13d
    0x44, 0x89, 0xe6,                                            // mov	esi, r12d
    0x4c, 0x89, 0xfa,                                            // mov	rdx, r15
    0x44, 0x89, 0xf1,                                            // mov	ecx, r14d
    0x49, 0x89, 0xd8,                                            // mov	r8, rbx
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x41, 0x5c,                                                  // pop	r12
    0x41, 0x5d,                                                  // pop	r13
    0x41, 0x5e,                                                  // pop	r14
    0x41, 0x5f,                                                  // pop	r15
    0x5d,                                                        // pop	rbp
    0xe9, 0xa6, 0xfc, 0xff, 0xff,                                // jmp	sceVideoOutRegisterBuffers
                                                                 // __export_sceVideodec2CreateDecoder_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x41, 0x57,                                                  // push	r15
    0x41, 0x56,                                                  // push	r14
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0x48, 0x89, 0xd3,                                            // mov	rbx, rdx
    0x48, 0x8d, 0x15, 0x8c, 0x01, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.5]
    0x49, 0x89, 0xf6,                                            // mov	r14, rsi
    0x49, 0x89, 0xff,                                            // mov	r15, rdi
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xc0,                                                  // xor	eax, eax
    0x48, 0x89, 0xd6,                                            // mov	rsi, rdx
    0xe8, 0xca, 0xfd, 0xff, 0xff,                                // call	notify_
    0x80, 0x3d, 0xb0, 0x00, 0x00, 0x00, 0x00,                    // cmp	byte ptr [rip + g_isBigCollection], 0
    0x74, 0x10,                                                  // je	L_BB6_2
    0x8b, 0x3d, 0xbe, 0x00, 0x00, 0x00,                          // mov	edi, dword ptr [rip + _ZL8g_handle]
    0xbe, 0x01, 0x00, 0x00, 0x00,                                // mov	esi, 1
    0xe8, 0x40, 0xfc, 0xff, 0xff,                                // call	sceVideoOutSetFlipRate
                                                                 // L_BB6_2:
    0x4c, 0x89, 0xff,                                            // mov	rdi, r15
    0x4c, 0x89, 0xf6,                                            // mov	rsi, r14
    0x48, 0x89, 0xda,                                            // mov	rdx, rbx
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x41, 0x5e,                                                  // pop	r14
    0x41, 0x5f,                                                  // pop	r15
    0x5d,                                                        // pop	rbp
    0xe9, 0x58, 0xfc, 0xff, 0xff,                                // jmp	sceVideodec2CreateDecoder
                                                                 // __export_sceVideodec2DeleteDecoder_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0x48, 0x8d, 0x15, 0xbc, 0x01, 0x00, 0x00,                    // lea	rdx, [rip + L_.str.6]
    0x48, 0x89, 0xfb,                                            // mov	rbx, rdi
    0xbf, 0x01, 0x00, 0x00, 0x00,                                // mov	edi, 1
    0x31, 0xc0,                                                  // xor	eax, eax
    0x48, 0x89, 0xd6,                                            // mov	rsi, rdx
    0xe8, 0x7a, 0xfd, 0xff, 0xff,                                // call	notify_
    0x80, 0x3d, 0x60, 0x00, 0x00, 0x00, 0x00,                    // cmp	byte ptr [rip + g_isBigCollection], 0
    0x74, 0x0d,                                                  // je	L_BB7_2
    0x8b, 0x3d, 0x6e, 0x00, 0x00, 0x00,                          // mov	edi, dword ptr [rip + _ZL8g_handle]
    0x31, 0xf6,                                                  // xor	esi, esi
    0xe8, 0xf3, 0xfb, 0xff, 0xff,                                // call	sceVideoOutSetFlipRate
                                                                 // L_BB7_2:
    0x48, 0x89, 0xdf,                                            // mov	rdi, rbx
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x5d,                                                        // pop	rbp
    0xe9, 0x21, 0xfc, 0xff, 0xff,                                // jmp	sceVideodec2DeleteDecoder
                                                                 // _ZL29sceVideoOutIsOutputSupported2jj:
    0xbe, 0x0f, 0x00, 0x00, 0x00,                                // mov	esi, 15
    0x31, 0xd2,                                                  // xor	edx, edx
    0x31, 0xc9,                                                  // xor	ecx, ecx
    0x45, 0x31, 0xc0,                                            // xor	r8d, r8d
    0xe9, 0xec, 0xfb, 0xff, 0xff,                                // jmp	sceVideoOutIsOutputSupported
                                                                 // _ZL27sceVideoOutConfigureOutput2jj:
    0xbe, 0x0f, 0x00, 0x00, 0x00,                                // mov	esi, 15
    0x31, 0xd2,                                                  // xor	edx, edx
    0x31, 0xc9,                                                  // xor	ecx, ecx
    0x45, 0x31, 0xc0,                                            // xor	r8d, r8d
    0xe9, 0xcf, 0xfb, 0xff, 0xff,                                // jmp	sceVideoOutConfigureOutput
                                                                 // L_.str:
    0x6e, 0x6f, 0x74, 0x69, 0x66, 0x79, 0x28, 0x25, 0x73, 0x29, 0x20, 0x25, 0x73, 0x0a, 0x00,  // .asciz	"notify(%s) %s\n"
                                                                 // L_.str.1:
    0x75, 0x6e, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x00,  // .asciz	"unknown"
                                                                 // L_.str.2:
    0x25, 0x73, 0x3a, 0x20, 0x25, 0x73, 0x0a, 0x25, 0x73, 0x00,  // .asciz	"%s: %s\n%s"
                                                                 // g_isBigCollection:
    0x00,                                                        // .byte	0
                                                                 // L_.str.3:
    0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x4f, 0x75, 0x74, 0x4f, 0x70, 0x65, 0x6e, 0x20, 0x30, 0x78, 0x25, 0x78, 0x00,  // .asciz	"sceVideoOutOpen 0x%x"
                                                                 // _ZL8g_handle:
    0x00, 0x00, 0x00, 0x00,                                      // .long	0
                                                                 // L_.str.4:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x76, 0x69, 0x64, 0x65, 0x6f, 0x6f, 0x75, 0x74, 0x5f, 0x31, 0x32, 0x30, 0x68, 0x7a, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x37, 0x31, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x4f, 0x75, 0x74, 0x52, 0x65, 0x67, 0x69, 0x73, 0x74, 0x65, 0x72, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x73, 0x5f, 0x68, 0x6f, 0x6f, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x29, 0x00,  // .asciz	"shellcode/user_videoout_120hz.cpp:71 int __cdecl __export_sceVideoOutRegisterBuffers_hook(const uint32_t, const uint32_t, const void *, const uint32_t, const void *)"
                                                                 // L_.str.5:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x76, 0x69, 0x64, 0x65, 0x6f, 0x6f, 0x75, 0x74, 0x5f, 0x31, 0x32, 0x30, 0x68, 0x7a, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x38, 0x33, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x64, 0x65, 0x63, 0x32, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x44, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x72, 0x5f, 0x68, 0x6f, 0x6f, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x29, 0x00,  // .asciz	"shellcode/user_videoout_120hz.cpp:83 int __cdecl __export_sceVideodec2CreateDecoder_hook(const void *, const void *, const void *)"
                                                                 // L_.str.6:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x76, 0x69, 0x64, 0x65, 0x6f, 0x6f, 0x75, 0x74, 0x5f, 0x31, 0x32, 0x30, 0x68, 0x7a, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x39, 0x37, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x64, 0x65, 0x63, 0x32, 0x44, 0x65, 0x6c, 0x65, 0x74, 0x65, 0x44, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x72, 0x5f, 0x68, 0x6f, 0x6f, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x29, 0x00,  // .asciz	"shellcode/user_videoout_120hz.cpp:97 int __cdecl __export_sceVideodec2DeleteDecoder_hook(const void *)"
                                                                 // L_.str.7:
    0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x4f, 0x75, 0x74, 0x49, 0x73, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x53, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x65, 0x64, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0a, 0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x4f, 0x75, 0x74, 0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x75, 0x72, 0x65, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0a, 0x00,  // .asciz	"sceVideoOutIsOutputSupported 0x%x\nsceVideoOutConfigureOutput 0x%x\n"
                                                                 // L_.str.8:
    0x73, 0x63, 0x65, 0x56, 0x69, 0x64, 0x65, 0x6f, 0x4f, 0x75, 0x74, 0x49, 0x73, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x53, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x65, 0x64, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6e, 0x65, 0x64, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0a, 0x25, 0x73, 0x00,  // .asciz	"sceVideoOutIsOutputSupported returned 0x%x\n%s"
                                                                 // L_.str.10:
    0x69, 0x73, 0x20, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x20, 0x70, 0x61, 0x74, 0x63, 0x68, 0x65, 0x64, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x20, 0x73, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x65, 0x64, 0x3f, 0x00,  // .asciz	"is shellcore patched or display supported?"
};
// clang-format on
