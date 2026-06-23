// clang-format off
static const unsigned long onNewProcess_original_offset = 0x7;
static const unsigned long sysctl_offset = 0x13;
static const unsigned long malloc_offset = 0x1f;
static const unsigned long free_offset = 0x2b;
static const unsigned long sceKernelGetAppInfo_offset = 0x37;
static const unsigned long getpid_offset = 0x43;
static const unsigned long kill_offset = 0x4f;
static const unsigned long puts_offset = 0x5b;
static const unsigned long usleep_offset = 0x67;
static const unsigned long fopen_offset = 0x73;
static const unsigned long fwrite_offset = 0x7f;
static const unsigned long fclose_offset = 0x8b;
static const unsigned long stat_offset = 0x97;
static const unsigned long memset_offset = 0xa3;
static const unsigned long vsnprintf_offset = 0xaf;
static const unsigned long printf_offset = 0xbb;
static const unsigned long strlen_offset = 0xc7;
static const unsigned long sceKernelSendNotificationRequest_offset = 0xd3;
static const unsigned long perror_offset = 0xdf;
static const unsigned long sceKernelGetProcessName_offset = 0xeb;
static const unsigned long fgets_offset = 0xf7;
static const unsigned long __export_GetAppInfoHook_offset = 0x101;
static const unsigned long socket_offset = 0x108;
static const unsigned long htons_offset = 0x114;
static const unsigned long connect_offset = 0x120;
static const unsigned long close_offset = 0x12c;
static const unsigned long recv_offset = 0x138;
static const unsigned long send_offset = 0x144;
static const unsigned long __inet_pton_offset = 0x150;
static const unsigned long __error_offset = 0x15c;
static const unsigned long strerror_offset = 0x168;
static const unsigned long strncpy_offset = 0x174;
static const unsigned long dynlib_get_obj_member_offset = 0x180;
static const unsigned long __export_onNewProcess_hook_offset = 0x18a;
static const unsigned long __export_PS4_GetAppInfoHook_offset = 0x4d7;
static const unsigned long __export_PS5_GetAppInfoHook_offset = 0x5b8;
static const unsigned long __export_PS5_GetAppInfoHook2_offset = 0x710;

static const unsigned char shellcode[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,                                                                    // .zero 5 # pad out original entry point
                                                                                                     // onNewProcess_original:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // sysctl:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // malloc:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // free:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // sceKernelGetAppInfo:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // getpid:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // kill:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // puts:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // usleep:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // fopen:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // fwrite:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // fclose:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // stat:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // memset:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // vsnprintf:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // printf:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // strlen:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // sceKernelSendNotificationRequest:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // perror:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // sceKernelGetProcessName:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // fgets:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // __export_GetAppInfoHook:
    0x00, 0x00, 0x00, 0x00, 0x00,                                                                    // .zero 5
                                                                                                     // socket:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // htons:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // connect:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // close:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // recv:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // send:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // __inet_pton:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // __error:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // strerror:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // strncpy:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // dynlib_get_obj_member:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // movabs rax, -1
    0xff, 0xe0,                                                                                      // jmp rax
                                                                                                     // __export_onNewProcess_hook:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x48, 0x89, 0xd3,                                                                                // mov	rbx, rdx
    0x49, 0x89, 0xf6,                                                                                // mov	r14, rsi
    0x49, 0x89, 0xff,                                                                                // mov	r15, rdi
    0x48, 0x85, 0xd2,                                                                                // test	rdx, rdx
    0x74, 0x04,                                                                                      // je	L_BB0_1
    0x8b, 0x33,                                                                                      // mov	esi, dword ptr [rbx]
    0xeb, 0x05,                                                                                      // jmp	L_BB0_3
                                                                                                     // L_BB0_1:
    0xbe, 0xff, 0xff, 0xff, 0xff,                                                                    // mov	esi, -1
                                                                                                     // L_BB0_3:
    0x48, 0x8d, 0x3d, 0x14, 0x0b, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str]
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x00, 0xff, 0xff, 0xff,                                                                    // call	printf
    0x4c, 0x89, 0xff,                                                                                // mov	rdi, r15
    0x4c, 0x89, 0xf6,                                                                                // mov	rsi, r14
    0x48, 0x89, 0xda,                                                                                // mov	rdx, rbx
    0xe8, 0x3e, 0xfe, 0xff, 0xff,                                                                    // call	onNewProcess_original
    0x41, 0x89, 0xc6,                                                                                // mov	r14d, eax
    0x48, 0x85, 0xdb,                                                                                // test	rbx, rbx
    0x74, 0x18,                                                                                      // je	L_BB0_6
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0xe8, 0x1e, 0x00, 0x00, 0x00,                                                                    // call	_ZL10patch_appsP6config
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x75, 0x0c,                                                                                      // jne	L_BB0_6
    0x48, 0x8d, 0x3d, 0x16, 0x0b, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.1]
    0xe8, 0x72, 0xfe, 0xff, 0xff,                                                                    // call	puts
                                                                                                     // L_BB0_6:
    0x44, 0x89, 0xf0,                                                                                // mov	eax, r14d
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL10patch_appsP6config:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x55,                                                                                      // push	r13
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x81, 0xec, 0xa8, 0x00, 0x00, 0x00,                                                        // sub	rsp, 168
    0x49, 0x89, 0xfe,                                                                                // mov	r14, rdi
    0x0f, 0x57, 0xc0,                                                                                // xorps	xmm0, xmm0
    0x0f, 0x29, 0x45, 0xc0,                                                                          // movaps	xmmword ptr [rbp - 64], xmm0
    0x48, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,                                      // movabs	rax, 60129542145
    0x48, 0x89, 0x45, 0xc0,                                                                          // mov	qword ptr [rbp - 64], rax
    0xc7, 0x45, 0xc8, 0x08, 0x00, 0x00, 0x00,                                                        // mov	dword ptr [rbp - 56], 8
    0x48, 0xc7, 0x45, 0xd0, 0x00, 0x00, 0x00, 0x00,                                                  // mov	qword ptr [rbp - 48], 0
    0x48, 0x8d, 0x7d, 0xc0,                                                                          // lea	rdi, [rbp - 64]
    0x48, 0x8d, 0x4d, 0xd0,                                                                          // lea	rcx, [rbp - 48]
    0xbe, 0x04, 0x00, 0x00, 0x00,                                                                    // mov	esi, 4
    0x31, 0xd2,                                                                                      // xor	edx, edx
    0x45, 0x31, 0xc0,                                                                                // xor	r8d, r8d
    0x45, 0x31, 0xc9,                                                                                // xor	r9d, r9d
    0xe8, 0xc7, 0xfd, 0xff, 0xff,                                                                    // call	sysctl
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x0f, 0x85, 0x44, 0x02, 0x00, 0x00,                                                              // jne	L_BB1_1
    0x48, 0x8b, 0x7d, 0xd0,                                                                          // mov	rdi, qword ptr [rbp - 48]
    0xe8, 0xc2, 0xfd, 0xff, 0xff,                                                                    // call	malloc
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x0f, 0x84, 0x45, 0x02, 0x00, 0x00,                                                              // je	L_BB1_3
    0x48, 0x89, 0xc3,                                                                                // mov	rbx, rax
    0x48, 0x8d, 0x7d, 0xc0,                                                                          // lea	rdi, [rbp - 64]
    0x48, 0x8d, 0x4d, 0xd0,                                                                          // lea	rcx, [rbp - 48]
    0xbe, 0x04, 0x00, 0x00, 0x00,                                                                    // mov	esi, 4
    0x48, 0x89, 0xc2,                                                                                // mov	rdx, rax
    0x45, 0x31, 0xc0,                                                                                // xor	r8d, r8d
    0x45, 0x31, 0xc9,                                                                                // xor	r9d, r9d
    0xe8, 0x8f, 0xfd, 0xff, 0xff,                                                                    // call	sysctl
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x0f, 0x85, 0x32, 0x02, 0x00, 0x00,                                                              // jne	L_BB1_5
    0x48, 0x8d, 0x3d, 0xde, 0x11, 0x00, 0x00,                                                        // lea	rdi, [rip + L_str]
    0xe8, 0xc3, 0xfd, 0xff, 0xff,                                                                    // call	puts
    0x48, 0x83, 0x7d, 0xd0, 0x00,                                                                    // cmp	qword ptr [rbp - 48], 0
    0x0f, 0x8e, 0xd9, 0x01, 0x00, 0x00,                                                              // jle	L_BB1_19
    0x4c, 0x89, 0x75, 0xb8,                                                                          // mov	qword ptr [rbp - 72], r14
    0x4c, 0x8d, 0x7d, 0x90,                                                                          // lea	r15, [rbp - 112]
    0x49, 0x89, 0xde,                                                                                // mov	r14, rbx
    0xe9, 0x87, 0x00, 0x00, 0x00,                                                                    // jmp	L_BB1_8
                                                                                                     // L_BB1_10:
    0x4d, 0x01, 0xf4,                                                                                // add	r12, r14
    0x49, 0x0f, 0xbe, 0xbe, 0x84, 0x01, 0x00, 0x00,                                                  // movsx	rdi, byte ptr [r14 + 388]
    0xe8, 0x32, 0x07, 0x00, 0x00,                                                                    // call	_ZL12state_abbrevm
    0x49, 0x89, 0xc2,                                                                                // mov	r10, rax
    0x41, 0x8b, 0x76, 0x48,                                                                          // mov	esi, dword ptr [r14 + 72]
    0x41, 0x8b, 0x56, 0x4c,                                                                          // mov	edx, dword ptr [r14 + 76]
    0x41, 0x8b, 0x4e, 0x50,                                                                          // mov	ecx, dword ptr [r14 + 80]
    0x45, 0x8b, 0x46, 0x58,                                                                          // mov	r8d, dword ptr [r14 + 88]
    0x45, 0x8b, 0x8e, 0xa8, 0x00, 0x00, 0x00,                                                        // mov	r9d, dword ptr [r14 + 168]
    0x49, 0x8b, 0x86, 0x08, 0x01, 0x00, 0x00,                                                        // mov	rax, qword ptr [r14 + 264]
    0x4c, 0x8d, 0x58, 0x3f,                                                                          // lea	r11, [rax + 63]
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x4c, 0x0f, 0x49, 0xd8,                                                                          // cmovns	r11, rax
    0x44, 0x8b, 0xad, 0x30, 0xff, 0xff, 0xff,                                                        // mov	r13d, dword ptr [rbp - 208]
    0x49, 0xc1, 0xfb, 0x06,                                                                          // sar	r11, 6
    0x4d, 0x8b, 0xb6, 0x00, 0x01, 0x00, 0x00,                                                        // mov	r14, qword ptr [r14 + 256]
    0x49, 0xc1, 0xee, 0x14,                                                                          // shr	r14, 20
    0x48, 0x8d, 0x3d, 0x5f, 0x0c, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.14]
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x53,                                                                                      // push	r11
    0x4c, 0x8d, 0x9d, 0x40, 0xff, 0xff, 0xff,                                                        // lea	r11, [rbp - 192]
    0x41, 0x53,                                                                                      // push	r11
    0x41, 0x55,                                                                                      // push	r13
    0x41, 0x52,                                                                                      // push	r10
    0xe8, 0x95, 0xfd, 0xff, 0xff,                                                                    // call	printf
    0x48, 0x83, 0xc4, 0x30,                                                                          // add	rsp, 48
    0x48, 0x8b, 0x45, 0xd0,                                                                          // mov	rax, qword ptr [rbp - 48]
    0x48, 0x8d, 0x0c, 0x03,                                                                          // lea	rcx, [rbx + rax]
    0x4d, 0x89, 0xe6,                                                                                // mov	r14, r12
    0x49, 0x39, 0xcc,                                                                                // cmp	r12, rcx
    0x73, 0x77,                                                                                      // jae	L_BB1_11
                                                                                                     // L_BB1_8:
    0x4d, 0x63, 0x26,                                                                                // movsxd	r12, dword ptr [r14]
    0x0f, 0x57, 0xc0,                                                                                // xorps	xmm0, xmm0
    0x0f, 0x29, 0x85, 0x70, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 144], xmm0
    0x0f, 0x29, 0x85, 0x60, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 160], xmm0
    0x0f, 0x29, 0x85, 0x50, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 176], xmm0
    0x0f, 0x29, 0x85, 0x40, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 192], xmm0
    0x0f, 0x29, 0x85, 0x30, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 208], xmm0
    0x48, 0xc7, 0x45, 0x80, 0x00, 0x00, 0x00, 0x00,                                                  // mov	qword ptr [rbp - 128], 0
    0x0f, 0x29, 0x45, 0xa0,                                                                          // movaps	xmmword ptr [rbp - 96], xmm0
    0x0f, 0x29, 0x45, 0x90,                                                                          // movaps	xmmword ptr [rbp - 112], xmm0
    0x41, 0x8b, 0x7e, 0x48,                                                                          // mov	edi, dword ptr [r14 + 72]
    0xba, 0x20, 0x00, 0x00, 0x00,                                                                    // mov	edx, 32
    0x4c, 0x89, 0xfe,                                                                                // mov	rsi, r15
    0xe8, 0x67, 0xfd, 0xff, 0xff,                                                                    // call	sceKernelGetProcessName
    0x41, 0x8b, 0x7e, 0x48,                                                                          // mov	edi, dword ptr [r14 + 72]
    0x48, 0x8d, 0xb5, 0x30, 0xff, 0xff, 0xff,                                                        // lea	rsi, [rbp - 208]
    0xe8, 0xa3, 0xfc, 0xff, 0xff,                                                                    // call	sceKernelGetAppInfo
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x0f, 0x84, 0x17, 0xff, 0xff, 0xff,                                                              // je	L_BB1_10
    0x48, 0x8d, 0x3d, 0xae, 0x0b, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.13]
    0x89, 0xc6,                                                                                      // mov	esi, eax
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x0f, 0xfd, 0xff, 0xff,                                                                    // call	printf
    0xe9, 0x02, 0xff, 0xff, 0xff,                                                                    // jmp	L_BB1_10
                                                                                                     // L_BB1_11:
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x48, 0x8b, 0x45, 0xb8,                                                                          // mov	rax, qword ptr [rbp - 72]
    0x0f, 0x8e, 0xbe, 0x00, 0x00, 0x00,                                                              // jle	L_BB1_19
    0x44, 0x8b, 0x30,                                                                                // mov	r14d, dword ptr [rax]
    0x4c, 0x8d, 0xbd, 0x30, 0xff, 0xff, 0xff,                                                        // lea	r15, [rbp - 208]
    0x49, 0x89, 0xdc,                                                                                // mov	r12, rbx
                                                                                                     // L_BB1_14:
    0x0f, 0x57, 0xc0,                                                                                // xorps	xmm0, xmm0
    0x0f, 0x29, 0x85, 0x70, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 144], xmm0
    0x0f, 0x29, 0x85, 0x60, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 160], xmm0
    0x0f, 0x29, 0x85, 0x50, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 176], xmm0
    0x0f, 0x29, 0x85, 0x40, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 192], xmm0
    0x0f, 0x29, 0x85, 0x30, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 208], xmm0
    0x48, 0xc7, 0x45, 0x80, 0x00, 0x00, 0x00, 0x00,                                                  // mov	qword ptr [rbp - 128], 0
    0x4d, 0x63, 0x2c, 0x24,                                                                          // movsxd	r13, dword ptr [r12]
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0x4c, 0x89, 0xfe,                                                                                // mov	rsi, r15
    0xe8, 0x2f, 0xfc, 0xff, 0xff,                                                                    // call	sceKernelGetAppInfo
    0x49, 0x8d, 0xbc, 0x24, 0xbf, 0x01, 0x00, 0x00,                                                  // lea	rdi, [r12 + 447]
    0xe8, 0x62, 0x06, 0x00, 0x00,                                                                    // call	_Z8stringidPKc
    0x3d, 0x42, 0x6c, 0xc7, 0xeb,                                                                    // cmp	eax, -339252158
    0x74, 0x1f,                                                                                      // je	L_BB1_17
    0x3d, 0xc4, 0xc1, 0xe4, 0xb1,                                                                    // cmp	eax, -1310408252
    0x74, 0x2b,                                                                                      // je	L_BB1_18
    0x3d, 0x45, 0x97, 0x63, 0xa0,                                                                    // cmp	eax, -1604085947
    0x74, 0x11,                                                                                      // je	L_BB1_17
    0x4d, 0x01, 0xec,                                                                                // add	r12, r13
    0x48, 0x8b, 0x45, 0xd0,                                                                          // mov	rax, qword ptr [rbp - 48]
    0x48, 0x01, 0xd8,                                                                                // add	rax, rbx
    0x49, 0x39, 0xc4,                                                                                // cmp	r12, rax
    0x72, 0x92,                                                                                      // jb	L_BB1_14
    0xeb, 0x41,                                                                                      // jmp	L_BB1_19
                                                                                                     // L_BB1_17:
    0x48, 0x8d, 0x3d, 0x71, 0x0b, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.16]
    0x44, 0x89, 0xf6,                                                                                // mov	esi, r14d
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x6f, 0xfc, 0xff, 0xff,                                                                    // call	printf
    0xeb, 0x2e,                                                                                      // jmp	L_BB1_19
                                                                                                     // L_BB1_18:
    0xc7, 0x45, 0x98, 0x00, 0x00, 0x00, 0x00,                                                        // mov	dword ptr [rbp - 104], 0
    0x44, 0x89, 0x75, 0x90,                                                                          // mov	dword ptr [rbp - 112], r14d
    0x8b, 0x95, 0x30, 0xff, 0xff, 0xff,                                                              // mov	edx, dword ptr [rbp - 208]
    0x89, 0x55, 0x94,                                                                                // mov	dword ptr [rbp - 108], edx
    0x48, 0x8d, 0x3d, 0x67, 0x0b, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.17]
    0x44, 0x89, 0xf6,                                                                                // mov	esi, r14d
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x48, 0xfc, 0xff, 0xff,                                                                    // call	printf
    0x48, 0x8d, 0x7d, 0x90,                                                                          // lea	rdi, [rbp - 112]
    0xe8, 0x1c, 0x06, 0x00, 0x00,                                                                    // call	_ZL22send_signal_to_payloadiR11client_data
                                                                                                     // L_BB1_19:
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0xe8, 0xa7, 0xfb, 0xff, 0xff,                                                                    // call	free
    0x31, 0xc0,                                                                                      // xor	eax, eax
                                                                                                     // L_BB1_20:
    0x48, 0x81, 0xc4, 0xa8, 0x00, 0x00, 0x00,                                                        // add	rsp, 168
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5d,                                                                                      // pop	r13
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB1_1:
    0x48, 0x8d, 0x3d, 0xa4, 0x0a, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.10]
    0xe8, 0x3b, 0xfc, 0xff, 0xff,                                                                    // call	perror
    0xb8, 0x78, 0xff, 0xff, 0xff,                                                                    // mov	eax, -136
    0xeb, 0xdb,                                                                                      // jmp	L_BB1_20
                                                                                                     // L_BB1_3:
    0x48, 0x8d, 0x3d, 0x98, 0x0a, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.11]
    0xe8, 0x28, 0xfc, 0xff, 0xff,                                                                    // call	perror
    0xb8, 0x71, 0xff, 0xff, 0xff,                                                                    // mov	eax, -143
    0xeb, 0xc8,                                                                                      // jmp	L_BB1_20
                                                                                                     // L_BB1_5:
    0x48, 0x8d, 0x3d, 0x7e, 0x0a, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.10]
    0xe8, 0x15, 0xfc, 0xff, 0xff,                                                                    // call	perror
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0xe8, 0x59, 0xfb, 0xff, 0xff,                                                                    // call	free
    0xb8, 0x69, 0xff, 0xff, 0xff,                                                                    // mov	eax, -151
    0xeb, 0xad,                                                                                      // jmp	L_BB1_20
                                                                                                     // __export_PS4_GetAppInfoHook:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x49, 0x89, 0xd6,                                                                                // mov	r14, rdx
    0x49, 0x89, 0xf7,                                                                                // mov	r15, rsi
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x48, 0x8d, 0x3d, 0x21, 0x08, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.2]
    0x48, 0x89, 0xde,                                                                                // mov	rsi, rbx
    0x4c, 0x89, 0xfa,                                                                                // mov	rdx, r15
    0x4c, 0x89, 0xf1,                                                                                // mov	rcx, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0xb8, 0xfb, 0xff, 0xff,                                                                    // call	printf
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x4c, 0x89, 0xfe,                                                                                // mov	rsi, r15
    0x4c, 0x89, 0xf2,                                                                                // mov	rdx, r14
    0x31, 0xc9,                                                                                      // xor	ecx, ecx
    0xe8, 0xf0, 0xfb, 0xff, 0xff,                                                                    // call	__export_GetAppInfoHook
    0x48, 0x85, 0xdb,                                                                                // test	rbx, rbx
    0x74, 0x23,                                                                                      // je	L_BB2_3
    0x4d, 0x85, 0xf6,                                                                                // test	r14, r14
    0x74, 0x1e,                                                                                      // je	L_BB2_3
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xbe, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	esi, 1
    0x41, 0x89, 0xc6,                                                                                // mov	r14d, eax
    0xe8, 0x19, 0x00, 0x00, 0x00,                                                                    // call	_ZL14get_attr_fieldmi
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x48, 0x89, 0xc6,                                                                                // mov	rsi, rax
    0xe8, 0x37, 0x00, 0x00, 0x00,                                                                    // call	_ZL11checkPS4ProPKcR9ATTRIBUTEi
    0x44, 0x89, 0xf0,                                                                                // mov	eax, r14d
                                                                                                     // L_BB2_3:
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL14get_attr_fieldmi:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x83, 0xfe, 0x01,                                                                                // cmp	esi, 1
    0x75, 0x09,                                                                                      // jne	L_BB3_2
    0x48, 0x81, 0xc3, 0x9c, 0x01, 0x00, 0x00,                                                        // add	rbx, 412
    0xeb, 0x08,                                                                                      // jmp	L_BB3_3
                                                                                                     // L_BB3_2:
    0xe8, 0xaf, 0x06, 0x00, 0x00,                                                                    // call	_ZL12get_ps5_attrv
    0x48, 0x01, 0xc3,                                                                                // add	rbx, rax
                                                                                                     // L_BB3_3:
    0x48, 0x89, 0xd8,                                                                                // mov	rax, rbx
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL11checkPS4ProPKcR9ATTRIBUTEi:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x56,                                                                                      // push	r14
    0x53,                                                                                            // push	rbx
    0x48, 0x89, 0xf3,                                                                                // mov	rbx, rsi
    0x49, 0x89, 0xfe,                                                                                // mov	r14, rdi
    0x48, 0x8d, 0x3d, 0xa9, 0x0e, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.39]
    0x4c, 0x89, 0xf6,                                                                                // mov	rsi, r14
    0xe8, 0x2a, 0x03, 0x00, 0x00,                                                                    // call	match_line_in_file
    0x83, 0xf8, 0x01,                                                                                // cmp	eax, 1
    0x75, 0x25,                                                                                      // jne	L_BB4_3
    0xb8, 0x00, 0x00, 0x80, 0x00,                                                                    // mov	eax, 8388608
    0x23, 0x03,                                                                                      // and	eax, dword ptr [rbx]
    0x74, 0x1c,                                                                                      // je	L_BB4_3
    0x48, 0x8d, 0x15, 0xab, 0x0e, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.42]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edi, 1
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0x4c, 0x89, 0xf1,                                                                                // mov	rcx, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x8c, 0x00, 0x00, 0x00,                                                                    // call	notify_
    0x80, 0x63, 0x02, 0x7f,                                                                          // and	byte ptr [rbx + 2], 127
                                                                                                     // L_BB4_3:
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5e,                                                                                      // pop	r14
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // __export_PS5_GetAppInfoHook:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x49, 0x89, 0xd6,                                                                                // mov	r14, rdx
    0x49, 0x89, 0xf7,                                                                                // mov	r15, rsi
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x48, 0x8d, 0x15, 0xc3, 0x07, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.3]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edi, 1
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0x48, 0x89, 0xd9,                                                                                // mov	rcx, rbx
    0x4d, 0x89, 0xf8,                                                                                // mov	r8, r15
    0x4d, 0x89, 0xf1,                                                                                // mov	r9, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x52, 0x00, 0x00, 0x00,                                                                    // call	notify_
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x4c, 0x89, 0xfe,                                                                                // mov	rsi, r15
    0x4c, 0x89, 0xf2,                                                                                // mov	rdx, r14
    0x31, 0xc9,                                                                                      // xor	ecx, ecx
    0xe8, 0x08, 0xfb, 0xff, 0xff,                                                                    // call	__export_GetAppInfoHook
    0x48, 0x85, 0xdb,                                                                                // test	rbx, rbx
    0x74, 0x32,                                                                                      // je	L_BB5_3
    0x4d, 0x85, 0xf6,                                                                                // test	r14, r14
    0x74, 0x2d,                                                                                      // je	L_BB5_3
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xbe, 0x02, 0x00, 0x00, 0x00,                                                                    // mov	esi, 2
    0x41, 0x89, 0xc7,                                                                                // mov	r15d, eax
    0xe8, 0x31, 0xff, 0xff, 0xff,                                                                    // call	_ZL14get_attr_fieldmi
    0x49, 0x89, 0xc6,                                                                                // mov	r14, rax
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x48, 0x89, 0xc6,                                                                                // mov	rsi, rax
    0xe8, 0x4c, 0xff, 0xff, 0xff,                                                                    // call	_ZL11checkPS4ProPKcR9ATTRIBUTEi
    0x49, 0x83, 0xc6, 0x08,                                                                          // add	r14, 8
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xe8, 0xd7, 0x00, 0x00, 0x00,                                                                    // call	_ZL9enableHFRPKcR10ATTRIBUTE3
    0x44, 0x89, 0xf8,                                                                                // mov	eax, r15d
                                                                                                     // L_BB5_3:
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // notify_:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x81, 0xec, 0xd0, 0x04, 0x00, 0x00,                                                        // sub	rsp, 1232
    0x49, 0x89, 0xd7,                                                                                // mov	r15, rdx
    0x48, 0x89, 0xf3,                                                                                // mov	rbx, rsi
    0x41, 0x89, 0xfe,                                                                                // mov	r14d, edi
    0x48, 0x89, 0x8d, 0x28, 0xff, 0xff, 0xff,                                                        // mov	qword ptr [rbp - 216], rcx
    0x4c, 0x89, 0x85, 0x30, 0xff, 0xff, 0xff,                                                        // mov	qword ptr [rbp - 208], r8
    0x4c, 0x89, 0x8d, 0x38, 0xff, 0xff, 0xff,                                                        // mov	qword ptr [rbp - 200], r9
    0x84, 0xc0,                                                                                      // test	al, al
    0x74, 0x2c,                                                                                      // je	L_BB6_2
    0x0f, 0x29, 0x85, 0x40, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 192], xmm0
    0x0f, 0x29, 0x8d, 0x50, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 176], xmm1
    0x0f, 0x29, 0x95, 0x60, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 160], xmm2
    0x0f, 0x29, 0x9d, 0x70, 0xff, 0xff, 0xff,                                                        // movaps	xmmword ptr [rbp - 144], xmm3
    0x0f, 0x29, 0x65, 0x80,                                                                          // movaps	xmmword ptr [rbp - 128], xmm4
    0x0f, 0x29, 0x6d, 0x90,                                                                          // movaps	xmmword ptr [rbp - 112], xmm5
    0x0f, 0x29, 0x75, 0xa0,                                                                          // movaps	xmmword ptr [rbp - 96], xmm6
    0x0f, 0x29, 0x7d, 0xb0,                                                                          // movaps	xmmword ptr [rbp - 80], xmm7
                                                                                                     // L_BB6_2:
    0x4c, 0x8d, 0xa5, 0x10, 0xfb, 0xff, 0xff,                                                        // lea	r12, [rbp - 1264]
    0xba, 0x00, 0x04, 0x00, 0x00,                                                                    // mov	edx, 1024
    0x4c, 0x89, 0xe7,                                                                                // mov	rdi, r12
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0xe8, 0xf0, 0xf9, 0xff, 0xff,                                                                    // call	memset
    0x48, 0x8d, 0x85, 0x10, 0xff, 0xff, 0xff,                                                        // lea	rax, [rbp - 240]
    0x48, 0x89, 0x45, 0xd0,                                                                          // mov	qword ptr [rbp - 48], rax
    0x48, 0x8d, 0x45, 0x10,                                                                          // lea	rax, [rbp + 16]
    0x48, 0x89, 0x45, 0xc8,                                                                          // mov	qword ptr [rbp - 56], rax
    0x48, 0xb8, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,                                      // movabs	rax, 206158430232
    0x48, 0x89, 0x45, 0xc0,                                                                          // mov	qword ptr [rbp - 64], rax
    0x48, 0x8d, 0x4d, 0xc0,                                                                          // lea	rcx, [rbp - 64]
    0xbe, 0xff, 0x03, 0x00, 0x00,                                                                    // mov	esi, 1023
    0x4c, 0x89, 0xe7,                                                                                // mov	rdi, r12
    0x4c, 0x89, 0xfa,                                                                                // mov	rdx, r15
    0xe8, 0xc7, 0xf9, 0xff, 0xff,                                                                    // call	vsnprintf
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0x48, 0x89, 0xde,                                                                                // mov	rsi, rbx
    0x4c, 0x89, 0xe2,                                                                                // mov	rdx, r12
    0xe8, 0xae, 0x00, 0x00, 0x00,                                                                    // call	notify_fixed_
    0x48, 0x81, 0xc4, 0xd0, 0x04, 0x00, 0x00,                                                        // add	rsp, 1232
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL9enableHFRPKcR10ATTRIBUTE3:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x81, 0x0f, 0x40, 0x00, 0x04, 0x00,                                                              // or	dword ptr [rdi], 262208
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // __export_PS5_GetAppInfoHook2:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x83, 0xec, 0x10,                                                                          // sub	rsp, 16
    0x49, 0x89, 0xce,                                                                                // mov	r14, rcx
    0x49, 0x89, 0xd7,                                                                                // mov	r15, rdx
    0x49, 0x89, 0xf4,                                                                                // mov	r12, rsi
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x48, 0x89, 0x0c, 0x24,                                                                          // mov	qword ptr [rsp], rcx
    0x48, 0x8d, 0x15, 0xe2, 0x06, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.4]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edi, 1
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0x48, 0x89, 0xd9,                                                                                // mov	rcx, rbx
    0x4d, 0x89, 0xe0,                                                                                // mov	r8, r12
    0x4d, 0x89, 0xf9,                                                                                // mov	r9, r15
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0xee, 0xfe, 0xff, 0xff,                                                                    // call	notify_
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x4c, 0x89, 0xe6,                                                                                // mov	rsi, r12
    0x4c, 0x89, 0xfa,                                                                                // mov	rdx, r15
    0x4c, 0x89, 0xf1,                                                                                // mov	rcx, r14
    0xe8, 0xa3, 0xf9, 0xff, 0xff,                                                                    // call	__export_GetAppInfoHook
    0x48, 0x85, 0xdb,                                                                                // test	rbx, rbx
    0x74, 0x32,                                                                                      // je	L_BB8_3
    0x4d, 0x85, 0xf6,                                                                                // test	r14, r14
    0x74, 0x2d,                                                                                      // je	L_BB8_3
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xbe, 0x02, 0x00, 0x00, 0x00,                                                                    // mov	esi, 2
    0x41, 0x89, 0xc7,                                                                                // mov	r15d, eax
    0xe8, 0xcc, 0xfd, 0xff, 0xff,                                                                    // call	_ZL14get_attr_fieldmi
    0x49, 0x89, 0xc6,                                                                                // mov	r14, rax
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0x48, 0x89, 0xc6,                                                                                // mov	rsi, rax
    0xe8, 0xe7, 0xfd, 0xff, 0xff,                                                                    // call	_ZL11checkPS4ProPKcR9ATTRIBUTEi
    0x49, 0x83, 0xc6, 0x08,                                                                          // add	r14, 8
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xe8, 0x72, 0xff, 0xff, 0xff,                                                                    // call	_ZL9enableHFRPKcR10ATTRIBUTE3
    0x44, 0x89, 0xf8,                                                                                // mov	eax, r15d
                                                                                                     // L_BB8_3:
    0x48, 0x83, 0xc4, 0x10,                                                                          // add	rsp, 16
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // notify_fixed_:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x81, 0xec, 0x30, 0x0c, 0x00, 0x00,                                                        // sub	rsp, 3120
    0x49, 0x89, 0xd4,                                                                                // mov	r12, rdx
    0x49, 0x89, 0xf7,                                                                                // mov	r15, rsi
    0x89, 0xfb,                                                                                      // mov	ebx, edi
    0x48, 0x8d, 0xbd, 0xb0, 0xf3, 0xff, 0xff,                                                        // lea	rdi, [rbp - 3152]
    0xba, 0x30, 0x0c, 0x00, 0x00,                                                                    // mov	edx, 3120
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0xe8, 0xd2, 0xf8, 0xff, 0xff,                                                                    // call	memset
    0xc7, 0x85, 0xc0, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                      // mov	dword ptr [rbp - 3136], -1
    0x4c, 0x8d, 0xb5, 0xdd, 0xf3, 0xff, 0xff,                                                        // lea	r14, [rbp - 3107]
    0xba, 0xff, 0x03, 0x00, 0x00,                                                                    // mov	edx, 1023
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0x4c, 0x89, 0xe6,                                                                                // mov	rsi, r12
    0xe8, 0x82, 0xf9, 0xff, 0xff,                                                                    // call	strncpy
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xe8, 0xcd, 0xf8, 0xff, 0xff,                                                                    // call	strlen
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x74, 0x17,                                                                                      // je	L_BB9_3
                                                                                                     // L_BB9_1:
    0x80, 0xbc, 0x05, 0xdc, 0xf3, 0xff, 0xff, 0x0a,                                                  // cmp	byte ptr [rbp + rax - 3108], 10
    0x75, 0x0d,                                                                                      // jne	L_BB9_3
    0xc6, 0x84, 0x05, 0xdc, 0xf3, 0xff, 0xff, 0x00,                                                  // mov	byte ptr [rbp + rax - 3108], 0
    0x48, 0xff, 0xc8,                                                                                // dec	rax
    0x75, 0xe9,                                                                                      // jne	L_BB9_1
                                                                                                     // L_BB9_3:
    0x4d, 0x85, 0xff,                                                                                // test	r15, r15
    0x74, 0x11,                                                                                      // je	L_BB9_4
    0x41, 0x80, 0x3f, 0x00,                                                                          // cmp	byte ptr [r15], 0
    0x48, 0x8d, 0x35, 0x98, 0x06, 0x00, 0x00,                                                        // lea	rsi, [rip + L_.str.6]
    0x49, 0x0f, 0x45, 0xf7,                                                                          // cmovne	rsi, r15
    0xeb, 0x07,                                                                                      // jmp	L_BB9_6
                                                                                                     // L_BB9_4:
    0x48, 0x8d, 0x35, 0x8b, 0x06, 0x00, 0x00,                                                        // lea	rsi, [rip + L_.str.6]
                                                                                                     // L_BB9_6:
    0x48, 0x8d, 0x3d, 0x75, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.5]
    0x4c, 0x89, 0xf2,                                                                                // mov	rdx, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x77, 0xf8, 0xff, 0xff,                                                                    // call	printf
    0x84, 0xdb,                                                                                      // test	bl, bl
    0x74, 0x15,                                                                                      // je	L_BB9_8
    0x48, 0x8d, 0xb5, 0xb0, 0xf3, 0xff, 0xff,                                                        // lea	rsi, [rbp - 3152]
    0xba, 0x30, 0x0c, 0x00, 0x00,                                                                    // mov	edx, 3120
    0x31, 0xff,                                                                                      // xor	edi, edi
    0x31, 0xc9,                                                                                      // xor	ecx, ecx
    0xe8, 0x76, 0xf8, 0xff, 0xff,                                                                    // call	sceKernelSendNotificationRequest
                                                                                                     // L_BB9_8:
    0x48, 0x81, 0xc4, 0x30, 0x0c, 0x00, 0x00,                                                        // add	rsp, 3120
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // ui_perror_:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x48, 0x89, 0xd3,                                                                                // mov	rbx, rdx
    0x49, 0x89, 0xf6,                                                                                // mov	r14, rsi
    0x49, 0x89, 0xff,                                                                                // mov	r15, rdi
    0xe8, 0xd7, 0xf8, 0xff, 0xff,                                                                    // call	__error
    0x8b, 0x38,                                                                                      // mov	edi, dword ptr [rax]
    0xe8, 0xdc, 0xf8, 0xff, 0xff,                                                                    // call	strerror
    0x48, 0x8d, 0x15, 0x33, 0x06, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.7]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edi, 1
    0x48, 0x89, 0xde,                                                                                // mov	rsi, rbx
    0x4c, 0x89, 0xf9,                                                                                // mov	rcx, r15
    0x49, 0x89, 0xc0,                                                                                // mov	r8, rax
    0x4d, 0x89, 0xf1,                                                                                // mov	r9, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xe9, 0x88, 0xfd, 0xff, 0xff,                                                                    // jmp	notify_
                                                                                                     // match_line_in_file:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x55,                                                                                      // push	r13
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x81, 0xec, 0x08, 0x04, 0x00, 0x00,                                                        // sub	rsp, 1032
    0x49, 0x89, 0xf5,                                                                                // mov	r13, rsi
    0x49, 0x89, 0xfe,                                                                                // mov	r14, rdi
    0x48, 0x8d, 0x35, 0xfa, 0x05, 0x00, 0x00,                                                        // lea	rsi, [rip + L_.str.8]
    0xe8, 0x98, 0xf7, 0xff, 0xff,                                                                    // call	fopen
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x0f, 0x84, 0x9a, 0x00, 0x00, 0x00,                                                              // je	L_BB11_1
    0x48, 0x89, 0xc3,                                                                                // mov	rbx, rax
    0x4c, 0x8d, 0xb5, 0xd0, 0xfb, 0xff, 0xff,                                                        // lea	r14, [rbp - 1072]
    0x45, 0x31, 0xff,                                                                                // xor	r15d, r15d
    0xba, 0x01, 0x04, 0x00, 0x00,                                                                    // mov	edx, 1025
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0xe8, 0xa3, 0xf7, 0xff, 0xff,                                                                    // call	memset
    0x4c, 0x89, 0xef,                                                                                // mov	rdi, r13
    0xe8, 0xbf, 0xf7, 0xff, 0xff,                                                                    // call	strlen
    0x49, 0x89, 0xc4,                                                                                // mov	r12, rax
    0x4c, 0x89, 0xef,                                                                                // mov	rdi, r13
    0x48, 0x89, 0xc6,                                                                                // mov	rsi, rax
    0xe8, 0xa4, 0x00, 0x00, 0x00,                                                                    // call	_ZL10stringid_lPKcl
    0x41, 0x89, 0xc5,                                                                                // mov	r13d, eax
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xbe, 0x00, 0x04, 0x00, 0x00,                                                                    // mov	esi, 1024
    0x48, 0x89, 0xda,                                                                                // mov	rdx, rbx
    0xe8, 0xce, 0xf7, 0xff, 0xff,                                                                    // call	fgets
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x74, 0x6f,                                                                                      // je	L_BB11_12
    0x4d, 0x89, 0xf7,                                                                                // mov	r15, r14
    0xeb, 0x03,                                                                                      // jmp	L_BB11_4
                                                                                                     // L_BB11_7:
    0x49, 0xff, 0xc7,                                                                                // inc	r15
                                                                                                     // L_BB11_4:
    0x41, 0x0f, 0xb6, 0x07,                                                                          // movzx	eax, byte ptr [r15]
    0x83, 0xf8, 0x09,                                                                                // cmp	eax, 9
    0x74, 0xf4,                                                                                      // je	L_BB11_7
    0x83, 0xf8, 0x23,                                                                                // cmp	eax, 35
    0x74, 0x1d,                                                                                      // je	L_BB11_10
    0x83, 0xf8, 0x20,                                                                                // cmp	eax, 32
    0x74, 0xea,                                                                                      // je	L_BB11_7
    0x4c, 0x89, 0xff,                                                                                // mov	rdi, r15
    0xe8, 0x0a, 0xf7, 0xff, 0xff,                                                                    // call	puts
    0x4c, 0x89, 0xff,                                                                                // mov	rdi, r15
    0x4c, 0x89, 0xe6,                                                                                // mov	rsi, r12
    0xe8, 0x5e, 0x00, 0x00, 0x00,                                                                    // call	_ZL10stringid_lPKcl
    0x44, 0x39, 0xe8,                                                                                // cmp	eax, r13d
    0x74, 0x36,                                                                                      // je	L_BB11_9
                                                                                                     // L_BB11_10:
    0x4c, 0x89, 0xf7,                                                                                // mov	rdi, r14
    0xbe, 0x00, 0x04, 0x00, 0x00,                                                                    // mov	esi, 1024
    0x48, 0x89, 0xda,                                                                                // mov	rdx, rbx
    0xe8, 0x86, 0xf7, 0xff, 0xff,                                                                    // call	fgets
    0x4d, 0x89, 0xf7,                                                                                // mov	r15, r14
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x75, 0xbd,                                                                                      // jne	L_BB11_4
    0x45, 0x31, 0xff,                                                                                // xor	r15d, r15d
    0xeb, 0x1f,                                                                                      // jmp	L_BB11_12
                                                                                                     // L_BB11_1:
    0x48, 0x8d, 0x3d, 0x4e, 0x05, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.9]
    0x4c, 0x89, 0xf6,                                                                                // mov	rsi, r14
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x2c, 0xf7, 0xff, 0xff,                                                                    // call	printf
    0x41, 0xbf, 0xf1, 0xff, 0xff, 0xff,                                                              // mov	r15d, -15
    0xeb, 0x0e,                                                                                      // jmp	L_BB11_13
                                                                                                     // L_BB11_9:
    0x41, 0xbf, 0x01, 0x00, 0x00, 0x00,                                                              // mov	r15d, 1
                                                                                                     // L_BB11_12:
    0x48, 0x89, 0xdf,                                                                                // mov	rdi, rbx
    0xe8, 0xe6, 0xf6, 0xff, 0xff,                                                                    // call	fclose
                                                                                                     // L_BB11_13:
    0x44, 0x89, 0xf8,                                                                                // mov	eax, r15d
    0x48, 0x81, 0xc4, 0x08, 0x04, 0x00, 0x00,                                                        // add	rsp, 1032
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5d,                                                                                      // pop	r13
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL10stringid_lPKcl:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x0f, 0xb6, 0x0f,                                                                                // movzx	ecx, byte ptr [rdi]
    0xb8, 0xc5, 0x9d, 0x1c, 0x81,                                                                    // mov	eax, -2128831035
    0x84, 0xc9,                                                                                      // test	cl, cl
    0x74, 0x29,                                                                                      // je	L_BB12_5
    0x48, 0x85, 0xf6,                                                                                // test	rsi, rsi
    0x74, 0x24,                                                                                      // je	L_BB12_5
    0xba, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edx, 1
                                                                                                     // L_BB12_3:
    0x0f, 0xbe, 0xc9,                                                                                // movsx	ecx, cl
    0x31, 0xc1,                                                                                      // xor	ecx, eax
    0x69, 0xc1, 0x93, 0x01, 0x00, 0x01,                                                              // imul	eax, ecx, 16777619
    0x0f, 0xb6, 0x0c, 0x17,                                                                          // movzx	ecx, byte ptr [rdi + rdx]
    0x84, 0xc9,                                                                                      // test	cl, cl
    0x74, 0x0c,                                                                                      // je	L_BB12_5
    0x4c, 0x8d, 0x42, 0x01,                                                                          // lea	r8, [rdx + 1]
    0x48, 0x39, 0xd6,                                                                                // cmp	rsi, rdx
    0x4c, 0x89, 0xc2,                                                                                // mov	rdx, r8
    0x75, 0xe1,                                                                                      // jne	L_BB12_3
                                                                                                     // L_BB12_5:
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL12state_abbrevm:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x48, 0x83, 0xff, 0x03,                                                                          // cmp	rdi, 3
    0x7e, 0x1b,                                                                                      // jle	L_BB13_1
    0x48, 0x83, 0xff, 0x05,                                                                          // cmp	rdi, 5
    0x7f, 0x30,                                                                                      // jg	L_BB13_10
    0x48, 0x83, 0xff, 0x04,                                                                          // cmp	rdi, 4
    0x74, 0x51,                                                                                      // je	L_BB13_14
    0x48, 0x83, 0xff, 0x05,                                                                          // cmp	rdi, 5
    0x75, 0x42,                                                                                      // jne	L_BB13_16
    0x48, 0x8d, 0x05, 0xfd, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.22]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_1:
    0x48, 0x83, 0xff, 0x01,                                                                          // cmp	rdi, 1
    0x74, 0x2a,                                                                                      // je	L_BB13_2
    0x48, 0x83, 0xff, 0x02,                                                                          // cmp	rdi, 2
    0x74, 0x48,                                                                                      // je	L_BB13_13
    0x48, 0x83, 0xff, 0x03,                                                                          // cmp	rdi, 3
    0x75, 0x27,                                                                                      // jne	L_BB13_16
    0x48, 0x8d, 0x05, 0xd7, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.20]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_10:
    0x48, 0x83, 0xff, 0x06,                                                                          // cmp	rdi, 6
    0x74, 0x2a,                                                                                      // je	L_BB13_15
    0x48, 0x83, 0xff, 0x07,                                                                          // cmp	rdi, 7
    0x75, 0x12,                                                                                      // jne	L_BB13_16
    0x48, 0x8d, 0x05, 0xdc, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.24]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_2:
    0x48, 0x8d, 0x05, 0xaf, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.18]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_16:
    0x48, 0x8d, 0x05, 0x58, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.15]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_14:
    0x48, 0x8d, 0x05, 0xad, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.21]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_15:
    0x48, 0x8d, 0x05, 0xb3, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.23]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB13_13:
    0x48, 0x8d, 0x05, 0x91, 0x05, 0x00, 0x00,                                                        // lea	rax, [rip + L_.str.19]
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _Z8stringidPKc:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x48, 0xc7, 0xc6, 0xff, 0xff, 0xff, 0xff,                                                        // mov	rsi, -1
    0x5d,                                                                                            // pop	rbp
    0xe9, 0x32, 0xff, 0xff, 0xff,                                                                    // jmp	_ZL10stringid_lPKcl
                                                                                                     // L_CPI15_0:
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // .zero	16
                                                                                                     // _ZL22send_signal_to_payloadiR11client_data:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x41, 0x57,                                                                                      // push	r15
    0x41, 0x56,                                                                                      // push	r14
    0x41, 0x55,                                                                                      // push	r13
    0x41, 0x54,                                                                                      // push	r12
    0x53,                                                                                            // push	rbx
    0x48, 0x83, 0xec, 0x18,                                                                          // sub	rsp, 24
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x41, 0xbc, 0x19, 0x00, 0x00, 0x00,                                                              // mov	r12d, 25
    0x4c, 0x8d, 0x6d, 0xc0,                                                                          // lea	r13, [rbp - 64]
                                                                                                     // L_BB15_1:
    0xbf, 0x02, 0x00, 0x00, 0x00,                                                                    // mov	edi, 2
    0xbe, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	esi, 1
    0x31, 0xd2,                                                                                      // xor	edx, edx
    0xe8, 0x41, 0xf6, 0xff, 0xff,                                                                    // call	socket
    0x41, 0x89, 0xc6,                                                                                // mov	r14d, eax
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x0f, 0x88, 0x03, 0x01, 0x00, 0x00,                                                              // js	L_BB15_2
    0x0f, 0x57, 0xc0,                                                                                // xorps	xmm0, xmm0
    0x0f, 0x29, 0x45, 0xc0,                                                                          // movaps	xmmword ptr [rbp - 64], xmm0
    0xc6, 0x45, 0xc1, 0x02,                                                                          // mov	byte ptr [rbp - 63], 2
    0x66, 0xc7, 0x45, 0xc2, 0x5d, 0x6c,                                                              // mov	word ptr [rbp - 62], 27741
    0xbf, 0x02, 0x00, 0x00, 0x00,                                                                    // mov	edi, 2
    0x48, 0x8d, 0x35, 0xac, 0x06, 0x00, 0x00,                                                        // lea	rsi, [rip + L_.str.30]
    0x48, 0x8d, 0x55, 0xc4,                                                                          // lea	rdx, [rbp - 60]
    0xe8, 0x58, 0xf6, 0xff, 0xff,                                                                    // call	__inet_pton
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0x4c, 0x89, 0xee,                                                                                // mov	rsi, r13
    0xba, 0x10, 0x00, 0x00, 0x00,                                                                    // mov	edx, 16
    0xe8, 0x18, 0xf6, 0xff, 0xff,                                                                    // call	connect
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x79, 0x63,                                                                                      // jns	L_BB15_4
    0x41, 0x89, 0xc7,                                                                                // mov	r15d, eax
    0x48, 0x8d, 0x3d, 0x8d, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.31]
    0xe8, 0xc4, 0xf5, 0xff, 0xff,                                                                    // call	perror
    0x48, 0x8d, 0x3d, 0xc6, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.32]
    0x44, 0x89, 0xfe,                                                                                // mov	esi, r15d
    0x44, 0x89, 0xfa,                                                                                // mov	edx, r15d
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x8c, 0xf5, 0xff, 0xff,                                                                    // call	printf
    0x48, 0x8d, 0x3d, 0xe5, 0x09, 0x00, 0x00,                                                        // lea	rdi, [rip + L_str.52]
    0xe8, 0x20, 0xf5, 0xff, 0xff,                                                                    // call	puts
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0xe8, 0xe9, 0xf5, 0xff, 0xff,                                                                    // call	close
    0xbf, 0x40, 0x42, 0x0f, 0x00,                                                                    // mov	edi, 1000000
    0xe8, 0x1a, 0xf5, 0xff, 0xff,                                                                    // call	usleep
    0x49, 0xff, 0xcc,                                                                                // dec	r12
    0x0f, 0x85, 0x60, 0xff, 0xff, 0xff,                                                              // jne	L_BB15_1
    0x8b, 0x0b,                                                                                      // mov	ecx, dword ptr [rbx]
    0x48, 0x8d, 0x15, 0x51, 0x07, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.36]
    0xbf, 0x01, 0x00, 0x00, 0x00,                                                                    // mov	edi, 1
    0x31, 0xf6,                                                                                      // xor	esi, esi
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0xd0, 0xfa, 0xff, 0xff,                                                                    // call	notify_
    0xeb, 0x57,                                                                                      // jmp	L_BB15_9
                                                                                                     // L_BB15_4:
    0xba, 0x0c, 0x00, 0x00, 0x00,                                                                    // mov	edx, 12
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0x48, 0x89, 0xde,                                                                                // mov	rsi, rbx
    0x31, 0xc9,                                                                                      // xor	ecx, ecx
    0xe8, 0xc3, 0xf5, 0xff, 0xff,                                                                    // call	send
    0xc7, 0x43, 0x08, 0x00, 0x00, 0x00, 0x00,                                                        // mov	dword ptr [rbx + 8], 0
    0xba, 0x0c, 0x00, 0x00, 0x00,                                                                    // mov	edx, 12
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0x48, 0x89, 0xde,                                                                                // mov	rsi, rbx
    0x31, 0xc9,                                                                                      // xor	ecx, ecx
    0xe8, 0x9e, 0xf5, 0xff, 0xff,                                                                    // call	recv
    0x8b, 0x5b, 0x08,                                                                                // mov	ebx, dword ptr [rbx + 8]
    0x48, 0x8d, 0x3d, 0xf2, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.34]
    0x89, 0xde,                                                                                      // mov	esi, ebx
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0x0e, 0xf5, 0xff, 0xff,                                                                    // call	printf
    0x83, 0xfb, 0x01,                                                                                // cmp	ebx, 1
    0x75, 0x0c,                                                                                      // jne	L_BB15_6
    0x48, 0x8d, 0x3d, 0x44, 0x09, 0x00, 0x00,                                                        // lea	rdi, [rip + L_str.51]
    0xe8, 0x9d, 0xf4, 0xff, 0xff,                                                                    // call	puts
                                                                                                     // L_BB15_6:
    0x44, 0x89, 0xf7,                                                                                // mov	edi, r14d
    0xe8, 0x66, 0xf5, 0xff, 0xff,                                                                    // call	close
                                                                                                     // L_BB15_9:
    0x48, 0x83, 0xc4, 0x18,                                                                          // add	rsp, 24
    0x5b,                                                                                            // pop	rbx
    0x41, 0x5c,                                                                                      // pop	r12
    0x41, 0x5d,                                                                                      // pop	r13
    0x41, 0x5e,                                                                                      // pop	r14
    0x41, 0x5f,                                                                                      // pop	r15
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB15_2:
    0x48, 0x8d, 0x3d, 0x4d, 0x04, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.25]
    0xe8, 0xfe, 0xf4, 0xff, 0xff,                                                                    // call	perror
    0x48, 0x8d, 0x3d, 0x75, 0x04, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.26]
    0x44, 0x89, 0xf6,                                                                                // mov	esi, r14d
    0x44, 0x89, 0xf2,                                                                                // mov	edx, r14d
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0xc6, 0xf4, 0xff, 0xff,                                                                    // call	printf
    0x48, 0x8d, 0x3d, 0xfd, 0x04, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.27]
    0x48, 0x8d, 0x35, 0xfd, 0x04, 0x00, 0x00,                                                        // lea	rsi, [rip + L_.str.28]
    0x48, 0x8d, 0x15, 0x29, 0x05, 0x00, 0x00,                                                        // lea	rdx, [rip + L_.str.29]
    0xe8, 0x5e, 0xfc, 0xff, 0xff,                                                                    // call	ui_perror_
    0xeb, 0xb5,                                                                                      // jmp	L_BB15_9
                                                                                                     // _ZL12get_ps5_attrv:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0xe8, 0x1a, 0x00, 0x00, 0x00,                                                                    // call	_ZL6get_fwi
    0x05, 0x00, 0xff, 0xff, 0xff,                                                                    // add	eax, -256
    0x3d, 0x61, 0x0f, 0x00, 0x00,                                                                    // cmp	eax, 3937
    0xb9, 0x6c, 0x02, 0x00, 0x00,                                                                    // mov	ecx, 620
    0xb8, 0xf4, 0x01, 0x00, 0x00,                                                                    // mov	eax, 500
    0x48, 0x0f, 0x42, 0xc1,                                                                          // cmovb	rax, rcx
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL6get_fwi:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x48, 0x83, 0xec, 0x10,                                                                          // sub	rsp, 16
    0xc7, 0x45, 0xfc, 0x00, 0x00, 0x00, 0x00,                                                        // mov	dword ptr [rbp - 4], 0
    0x48, 0x8d, 0x7d, 0xfc,                                                                          // lea	rdi, [rbp - 4]
    0xe8, 0x0a, 0x00, 0x00, 0x00,                                                                    // call	_ZL30get_firmware_version_from_diskiRj
    0x0f, 0xb7, 0x45, 0xfe,                                                                          // movzx	eax, word ptr [rbp - 2]
    0x48, 0x83, 0xc4, 0x10,                                                                          // add	rsp, 16
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // _ZL30get_firmware_version_from_diskiRj:
    0x8b, 0x05, 0xcc, 0x06, 0x00, 0x00,                                                              // mov	eax, dword ptr [rip + _ZZL30get_firmware_version_from_diskiRjE6cached]
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x74, 0x03,                                                                                      // je	L_BB18_1
    0x89, 0x07,                                                                                      // mov	dword ptr [rdi], eax
    0xc3,                                                                                            // ret
                                                                                                     // L_BB18_1:
    0x55,                                                                                            // push	rbp
    0x48, 0x89, 0xe5,                                                                                // mov	rbp, rsp
    0x53,                                                                                            // push	rbx
    0x50,                                                                                            // push	rax
    0x48, 0x89, 0xfb,                                                                                // mov	rbx, rdi
    0x48, 0xc7, 0x45, 0xf0, 0x00, 0x00, 0x00, 0x00,                                                  // mov	qword ptr [rbp - 16], 0
    0x48, 0x8d, 0x55, 0xf0,                                                                          // lea	rdx, [rbp - 16]
    0xbe, 0x08, 0x00, 0x00, 0x00,                                                                    // mov	esi, 8
    0xbf, 0x01, 0x20, 0x00, 0x00,                                                                    // mov	edi, 8193
    0xe8, 0xf9, 0xf4, 0xff, 0xff,                                                                    // call	dynlib_get_obj_member
    0x85, 0xc0,                                                                                      // test	eax, eax
    0x75, 0x1b,                                                                                      // jne	L_BB18_2
    0x48, 0x8b, 0x45, 0xf0,                                                                          // mov	rax, qword ptr [rbp - 16]
    0x48, 0x85, 0xc0,                                                                                // test	rax, rax
    0x74, 0x0b,                                                                                      // je	L_BB18_5
    0x8b, 0x40, 0x14,                                                                                // mov	eax, dword ptr [rax + 20]
    0x89, 0x05, 0x8b, 0x06, 0x00, 0x00,                                                              // mov	dword ptr [rip + _ZZL30get_firmware_version_from_diskiRjE6cached], eax
    0x89, 0x03,                                                                                      // mov	dword ptr [rbx], eax
                                                                                                     // L_BB18_5:
    0x48, 0x83, 0xc4, 0x08,                                                                          // add	rsp, 8
    0x5b,                                                                                            // pop	rbx
    0x5d,                                                                                            // pop	rbp
    0xc3,                                                                                            // ret
                                                                                                     // L_BB18_2:
    0x48, 0x8d, 0x3d, 0x7f, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.37]
    0x89, 0xc3,                                                                                      // mov	ebx, eax
    0xe8, 0x2b, 0xf4, 0xff, 0xff,                                                                    // call	perror
    0x48, 0x8d, 0x3d, 0xbd, 0x06, 0x00, 0x00,                                                        // lea	rdi, [rip + L_.str.38]
    0x89, 0xde,                                                                                      // mov	esi, ebx
    0x89, 0xda,                                                                                      // mov	edx, ebx
    0x31, 0xc0,                                                                                      // xor	eax, eax
    0xe8, 0xf5, 0xf3, 0xff, 0xff,                                                                    // call	printf
    0xeb, 0xd7,                                                                                      // jmp	L_BB18_5
                                                                                                     // L_.str:
    0x6f, 0x6e, 0x4e, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x5f, 0x68, 0x6f, 0x6f, 0x6b, 0x20, 0x63, 0x61, 0x6c, 0x6c, 0x65, 0x64, 0x21, 0x20, 0x70, 0x61, 0x72, 0x61, 0x6d, 0x5f, 0x33, 0x2d, 0x3e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x50, 0x69, 0x64, 0x3a, 0x20, 0x25, 0x64, 0x0a, 0x00,  // .asciz	"onNewProcess_hook called! param_3->clientPid: %d\n"
                                                                                                     // L_.str.1:
    0x70, 0x61, 0x74, 0x63, 0x68, 0x5f, 0x61, 0x70, 0x70, 0x73, 0x28, 0x70, 0x61, 0x72, 0x61, 0x6d, 0x5f, 0x33, 0x29, 0x20, 0x6f, 0x6b, 0x61, 0x79, 0x21, 0x00,  // .asciz	"patch_apps(param_3) okay!"
                                                                                                     // L_.str.2:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x32, 0x34, 0x33, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x50, 0x53, 0x34, 0x5f, 0x47, 0x65, 0x74, 0x41, 0x70, 0x70, 0x49, 0x6e, 0x66, 0x6f, 0x48, 0x6f, 0x6f, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x63, 0x68, 0x61, 0x72, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x70, 0x74, 0x72, 0x5f, 0x74, 0x29, 0x3a, 0x20, 0x28, 0x25, 0x70, 0x2c, 0x25, 0x70, 0x2c, 0x25, 0x6c, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_attribute.cpp:243 int __cdecl __export_PS4_GetAppInfoHook(const char *, const void *, uintptr_t): (%p,%p,%lx)\n"
                                                                                                     // L_.str.3:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x32, 0x35, 0x37, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x50, 0x53, 0x35, 0x5f, 0x47, 0x65, 0x74, 0x41, 0x70, 0x70, 0x49, 0x6e, 0x66, 0x6f, 0x48, 0x6f, 0x6f, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x63, 0x68, 0x61, 0x72, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x70, 0x74, 0x72, 0x5f, 0x74, 0x29, 0x3a, 0x20, 0x28, 0x25, 0x70, 0x2c, 0x25, 0x70, 0x2c, 0x25, 0x6c, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_attribute.cpp:257 int __cdecl __export_PS5_GetAppInfoHook(const char *, const void *, uintptr_t): (%p,%p,%lx)\n"
                                                                                                     // L_.str.4:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x32, 0x37, 0x31, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x5f, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x50, 0x53, 0x35, 0x5f, 0x47, 0x65, 0x74, 0x41, 0x70, 0x70, 0x49, 0x6e, 0x66, 0x6f, 0x48, 0x6f, 0x6f, 0x6b, 0x32, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x63, 0x68, 0x61, 0x72, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x2a, 0x2c, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x70, 0x74, 0x72, 0x5f, 0x74, 0x29, 0x3a, 0x20, 0x28, 0x25, 0x70, 0x2c, 0x25, 0x70, 0x2c, 0x25, 0x70, 0x2c, 0x25, 0x6c, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_attribute.cpp:271 int __cdecl __export_PS5_GetAppInfoHook2(const char *, const void *, const void *, uintptr_t): (%p,%p,%p,%lx)\n"
                                                                                                     // L_.str.5:
    0x6e, 0x6f, 0x74, 0x69, 0x66, 0x79, 0x28, 0x25, 0x73, 0x29, 0x20, 0x25, 0x73, 0x0a, 0x00,  // .asciz	"notify(%s) %s\n"
                                                                                                     // L_.str.6:
    0x75, 0x6e, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x00,  // .asciz	"unknown"
                                                                                                     // L_.str.7:
    0x25, 0x73, 0x3a, 0x20, 0x25, 0x73, 0x0a, 0x25, 0x73, 0x00,  // .asciz	"%s: %s\n%s"
                                                                                                     // L_.str.8:
    0x72, 0x62, 0x00,  // .asciz	"rb"
                                                                                                     // L_.str.9:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x2e, 0x2e, 0x2f, 0x2e, 0x2e, 0x2f, 0x73, 0x68, 0x61, 0x72, 0x65, 0x64, 0x2f, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x2e, 0x63, 0x3a, 0x31, 0x34, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x5f, 0x69, 0x6e, 0x5f, 0x66, 0x69, 0x6c, 0x65, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x63, 0x68, 0x61, 0x72, 0x20, 0x2a, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x63, 0x68, 0x61, 0x72, 0x20, 0x2a, 0x29, 0x3a, 0x20, 0x25, 0x73, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 0x20, 0x6e, 0x79, 0x61, 0x0a, 0x00,  // .asciz	"shellcode/../../shared/match.c:14 int __cdecl match_line_in_file(const char *, const char *): %s not found nya\n"
                                                                                                     // L_.str.10:
    0x73, 0x79, 0x73, 0x63, 0x74, 0x6c, 0x00,  // .asciz	"sysctl"
                                                                                                     // L_.str.11:
    0x6d, 0x61, 0x6c, 0x6c, 0x6f, 0x63, 0x00,  // .asciz	"malloc"
                                                                                                     // L_.str.13:
    0x73, 0x63, 0x65, 0x4b, 0x65, 0x72, 0x6e, 0x65, 0x6c, 0x47, 0x65, 0x74, 0x41, 0x70, 0x70, 0x49, 0x6e, 0x66, 0x6f, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0a, 0x00,  // .asciz	"sceKernelGetAppInfo 0x%x\n"
                                                                                                     // L_.str.14:
    0x25, 0x38, 0x75, 0x20, 0x20, 0x25, 0x38, 0x75, 0x20, 0x25, 0x38, 0x75, 0x20, 0x25, 0x38, 0x75, 0x20, 0x25, 0x38, 0x75, 0x20, 0x25, 0x31, 0x30, 0x73, 0x20, 0x20, 0x20, 0x25, 0x30, 0x38, 0x78, 0x20, 0x20, 0x25, 0x31, 0x30, 0x73, 0x20, 0x20, 0x25, 0x30, 0x36, 0x6c, 0x64, 0x6d, 0x62, 0x20, 0x2f, 0x20, 0x25, 0x30, 0x36, 0x6c, 0x64, 0x6d, 0x62, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x25, 0x33, 0x31, 0x73, 0x0a, 0x00,  // .asciz	"%8u  %8u %8u %8u %8u %10s   %08x  %10s  %06ldmb / %06ldmb        %31s\n"
                                                                                                     // L_.str.15:
    0x00,                                                                                            // .zero	1
                                                                                                     // L_.str.16:
    0x69, 0x67, 0x6e, 0x6f, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x20, 0x70, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x20, 0x25, 0x64, 0x0a, 0x00,  // .asciz	"ignoring payload process %d\n"
                                                                                                     // L_.str.17:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x6f, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x3a, 0x20, 0x70, 0x69, 0x64, 0x20, 0x25, 0x64, 0x20, 0x61, 0x70, 0x70, 0x69, 0x64, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0a, 0x00,  // .asciz	"shellcore sending to server: pid %d appid 0x%x\n"
                                                                                                     // L_.str.18:
    0x53, 0x54, 0x41, 0x52, 0x54, 0x00,  // .asciz	"START"
                                                                                                     // L_.str.19:
    0x52, 0x55, 0x4e, 0x00,  // .asciz	"RUN"
                                                                                                     // L_.str.20:
    0x53, 0x4c, 0x45, 0x45, 0x50, 0x00,  // .asciz	"SLEEP"
                                                                                                     // L_.str.21:
    0x53, 0x54, 0x4f, 0x50, 0x00,  // .asciz	"STOP"
                                                                                                     // L_.str.22:
    0x5a, 0x41, 0x20, 0x5a, 0x4f, 0x4d, 0x42, 0x49, 0x45, 0x00,  // .asciz	"ZA ZOMBIE"
                                                                                                     // L_.str.23:
    0x57, 0x41, 0x49, 0x54, 0x00,  // .asciz	"WAIT"
                                                                                                     // L_.str.24:
    0x4c, 0x4f, 0x43, 0x4b, 0x00,  // .asciz	"LOCK"
                                                                                                     // L_.str.25:
    0x66, 0x64, 0x20, 0x3c, 0x20, 0x30, 0x20, 0x62, 0x65, 0x63, 0x61, 0x75, 0x73, 0x65, 0x20, 0x66, 0x64, 0x20, 0x3d, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x28, 0x41, 0x46, 0x5f, 0x49, 0x4e, 0x45, 0x54, 0x2c, 0x20, 0x53, 0x4f, 0x43, 0x4b, 0x5f, 0x53, 0x54, 0x52, 0x45, 0x41, 0x4d, 0x2c, 0x20, 0x30, 0x29, 0x00,  // .asciz	"fd < 0 because fd = socket(AF_INET, SOCK_STREAM, 0)"
                                                                                                     // L_.str.26:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x6f, 0x6e, 0x4e, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x33, 0x37, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x61, 0x6c, 0x5f, 0x74, 0x6f, 0x5f, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x70, 0x69, 0x64, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x64, 0x61, 0x74, 0x61, 0x20, 0x26, 0x29, 0x3a, 0x20, 0x66, 0x64, 0x20, 0x3d, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x28, 0x41, 0x46, 0x5f, 0x49, 0x4e, 0x45, 0x54, 0x2c, 0x20, 0x53, 0x4f, 0x43, 0x4b, 0x5f, 0x53, 0x54, 0x52, 0x45, 0x41, 0x4d, 0x2c, 0x20, 0x30, 0x29, 0x3a, 0x20, 0x25, 0x64, 0x20, 0x28, 0x30, 0x78, 0x25, 0x30, 0x38, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_onNewProcess.cpp:37 void __cdecl send_signal_to_payload(const pid_t, client_data &): fd = socket(AF_INET, SOCK_STREAM, 0): %d (0x%08x)\n"
                                                                                                     // L_.str.27:
    0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x00,  // .asciz	"socket"
                                                                                                     // L_.str.28:
    0x66, 0x61, 0x69, 0x6c, 0x65, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x6f, 0x70, 0x65, 0x6e, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2c, 0x20, 0x6d, 0x61, 0x79, 0x62, 0x65, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x20, 0x70, 0x65, 0x72, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x3f, 0x00,  // .asciz	"failed to open socket, maybe not valid permission?"
                                                                                                     // L_.str.29:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x6f, 0x6e, 0x4e, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x34, 0x30, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x61, 0x6c, 0x5f, 0x74, 0x6f, 0x5f, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x70, 0x69, 0x64, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x64, 0x61, 0x74, 0x61, 0x20, 0x26, 0x29, 0x00,  // .asciz	"shellcode/shellcore_onNewProcess.cpp:40 void __cdecl send_signal_to_payload(const pid_t, client_data &)"
                                                                                                     // L_.str.30:
    0x30, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x30, 0x00,  // .asciz	"0.0.0.0"
                                                                                                     // L_.str.31:
    0x72, 0x20, 0x3c, 0x20, 0x30, 0x20, 0x62, 0x65, 0x63, 0x61, 0x75, 0x73, 0x65, 0x20, 0x72, 0x20, 0x3d, 0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x28, 0x66, 0x64, 0x2c, 0x20, 0x28, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x61, 0x64, 0x64, 0x72, 0x2a, 0x29, 0x26, 0x61, 0x64, 0x64, 0x72, 0x2c, 0x20, 0x73, 0x69, 0x7a, 0x65, 0x6f, 0x66, 0x28, 0x61, 0x64, 0x64, 0x72, 0x29, 0x29, 0x00,  // .asciz	"r < 0 because r = connect(fd, (struct sockaddr*)&addr, sizeof(addr))"
                                                                                                     // L_.str.32:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x6f, 0x6e, 0x4e, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x34, 0x38, 0x20, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x61, 0x6c, 0x5f, 0x74, 0x6f, 0x5f, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x70, 0x69, 0x64, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x64, 0x61, 0x74, 0x61, 0x20, 0x26, 0x29, 0x3a, 0x20, 0x72, 0x20, 0x3d, 0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x28, 0x66, 0x64, 0x2c, 0x20, 0x28, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x61, 0x64, 0x64, 0x72, 0x2a, 0x29, 0x26, 0x61, 0x64, 0x64, 0x72, 0x2c, 0x20, 0x73, 0x69, 0x7a, 0x65, 0x6f, 0x66, 0x28, 0x61, 0x64, 0x64, 0x72, 0x29, 0x29, 0x3a, 0x20, 0x25, 0x64, 0x20, 0x28, 0x30, 0x78, 0x25, 0x30, 0x38, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_onNewProcess.cpp:48 void __cdecl send_signal_to_payload(const pid_t, client_data &): r = connect(fd, (struct sockaddr*)&addr, sizeof(addr)): %d (0x%08x)\n"
                                                                                                     // L_.str.34:
    0x5b, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5d, 0x20, 0x72, 0x65, 0x63, 0x65, 0x69, 0x76, 0x65, 0x64, 0x3a, 0x20, 0x25, 0x64, 0x0a, 0x00,  // .asciz	"[shellcore] received: %d\n"
                                                                                                     // L_.str.36:
    0x76, 0x6f, 0x69, 0x64, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x61, 0x6c, 0x5f, 0x74, 0x6f, 0x5f, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x70, 0x69, 0x64, 0x5f, 0x74, 0x2c, 0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x64, 0x61, 0x74, 0x61, 0x20, 0x26, 0x29, 0x3a, 0x37, 0x31, 0x3a, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x64, 0x20, 0x6f, 0x75, 0x74, 0x20, 0x77, 0x61, 0x69, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x2e, 0x0a, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x70, 0x69, 0x64, 0x20, 0x25, 0x64, 0x00,  // .asciz	"void __cdecl send_signal_to_payload(const pid_t, client_data &):71: timed out waiting for payload server.\nclient pid %d"
                                                                                                     // _ZZL30get_firmware_version_from_diskiRjE6cached:
    0x00, 0x00, 0x00, 0x00,                                                                          // .long 0
                                                                                                     // L_.str.37:
    0x72, 0x20, 0x21, 0x3d, 0x20, 0x30, 0x20, 0x62, 0x65, 0x63, 0x61, 0x75, 0x73, 0x65, 0x20, 0x72, 0x20, 0x3d, 0x20, 0x64, 0x79, 0x6e, 0x6c, 0x69, 0x62, 0x5f, 0x67, 0x65, 0x74, 0x5f, 0x6f, 0x62, 0x6a, 0x5f, 0x6d, 0x65, 0x6d, 0x62, 0x65, 0x72, 0x28, 0x6c, 0x69, 0x62, 0x6b, 0x65, 0x72, 0x6e, 0x65, 0x6c, 0x5f, 0x73, 0x79, 0x73, 0x2c, 0x20, 0x38, 0x2c, 0x20, 0x26, 0x73, 0x63, 0x65, 0x5f, 0x70, 0x72, 0x6f, 0x63, 0x5f, 0x70, 0x61, 0x72, 0x61, 0x6d, 0x29, 0x00,  // .asciz	"r != 0 because r = dynlib_get_obj_member(libkernel_sys, 8, &sce_proc_param)"
                                                                                                     // L_.str.38:
    0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x64, 0x65, 0x2f, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5f, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x2e, 0x63, 0x70, 0x70, 0x3a, 0x31, 0x36, 0x32, 0x20, 0x69, 0x6e, 0x74, 0x20, 0x5f, 0x5f, 0x63, 0x64, 0x65, 0x63, 0x6c, 0x20, 0x67, 0x65, 0x74, 0x5f, 0x66, 0x69, 0x72, 0x6d, 0x77, 0x61, 0x72, 0x65, 0x5f, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x5f, 0x66, 0x72, 0x6f, 0x6d, 0x5f, 0x64, 0x69, 0x73, 0x6b, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20, 0x69, 0x6e, 0x74, 0x2c, 0x20, 0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x5f, 0x74, 0x20, 0x26, 0x29, 0x3a, 0x20, 0x72, 0x20, 0x3d, 0x20, 0x64, 0x79, 0x6e, 0x6c, 0x69, 0x62, 0x5f, 0x67, 0x65, 0x74, 0x5f, 0x6f, 0x62, 0x6a, 0x5f, 0x6d, 0x65, 0x6d, 0x62, 0x65, 0x72, 0x28, 0x6c, 0x69, 0x62, 0x6b, 0x65, 0x72, 0x6e, 0x65, 0x6c, 0x5f, 0x73, 0x79, 0x73, 0x2c, 0x20, 0x38, 0x2c, 0x20, 0x26, 0x73, 0x63, 0x65, 0x5f, 0x70, 0x72, 0x6f, 0x63, 0x5f, 0x70, 0x61, 0x72, 0x61, 0x6d, 0x29, 0x3a, 0x20, 0x25, 0x64, 0x20, 0x28, 0x30, 0x78, 0x25, 0x30, 0x38, 0x78, 0x29, 0x0a, 0x00,  // .asciz	"shellcode/shellcore_attribute.cpp:162 int __cdecl get_firmware_version_from_disk(const int, uint32_t &): r = dynlib_get_obj_member(libkernel_sys, 8, &sce_proc_param): %d (0x%08x)\n"
                                                                                                     // L_.str.39:
    0x2f, 0x64, 0x61, 0x74, 0x61, 0x2f, 0x75, 0x73, 0x65, 0x72, 0x2f, 0x64, 0x69, 0x73, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x70, 0x73, 0x34, 0x5f, 0x70, 0x72, 0x6f, 0x2e, 0x74, 0x78, 0x74, 0x00,  // .asciz	"/data/user/disable_ps4_pro.txt"
                                                                                                     // L_.str.42:
    0x25, 0x73, 0x20, 0x77, 0x61, 0x6e, 0x74, 0x65, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x64, 0x69, 0x73, 0x61, 0x62, 0x6c, 0x65, 0x20, 0x50, 0x53, 0x34, 0x20, 0x50, 0x72, 0x6f, 0x20, 0x73, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x0a, 0x00,  // .asciz	"%s wanted to disable PS4 Pro support\n"
                                                                                                     // L_str:
    0x20, 0x20, 0x20, 0x20, 0x20, 0x50, 0x49, 0x44, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x50, 0x50, 0x49, 0x44, 0x20, 0x20, 0x20, 0x20, 0x20, 0x50, 0x47, 0x49, 0x44, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53, 0x49, 0x44, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x55, 0x49, 0x44, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53, 0x74, 0x61, 0x74, 0x65, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x41, 0x70, 0x70, 0x49, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x54, 0x69, 0x74, 0x6c, 0x65, 0x49, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x4d, 0x65, 0x6d, 0x6f, 0x72, 0x79, 0x20, 0x28, 0x4d, 0x69, 0x42, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x6f, 0x6d, 0x6d, 0x61, 0x6e, 0x64, 0x00,  // .asciz	"     PID      PPID     PGID      SID      UID      State      AppId     TitleId         Memory (MiB)                                Command"
                                                                                                     // L_str.51:
    0x5b, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5d, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x73, 0x61, 0x69, 0x64, 0x20, 0x6f, 0x6b, 0x61, 0x79, 0x21, 0x00,  // .asciz	"[shellcore] server said okay!"
                                                                                                     // L_str.52:
    0x5b, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72, 0x65, 0x5d, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x75, 0x70, 0x2c, 0x20, 0x72, 0x65, 0x74, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x2e, 0x2e, 0x2e, 0x00,  // .asciz	"[shellcore] server not up, retrying..."
};
// clang-format on
