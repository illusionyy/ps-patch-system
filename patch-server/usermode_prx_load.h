// clang-format off
static const unsigned long __export_post_prx_load_append_module_offset = 0x0;
static const unsigned long payload_pid_offset = 0x12;
static const unsigned long getpid_offset = 0x1d;
static const unsigned long kill_offset = 0x29;
static const unsigned long payload_console_fd_offset = 0x35;
static const unsigned long __sys_rdup_offset = 0x40;
static const unsigned long dup2_offset = 0x4c;
static const unsigned long printf_offset = 0x58;
static const unsigned long __export_post_prx_load_append_module_hook_offset = 0x62;
static const unsigned long __export_init_env_hook_offset = 0x9a;

static const unsigned char shellcode[] = {
                                                                 // __export_post_prx_load_append_module:
    0x48, 0xb8, 0x52, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // movabs rax, 0x252
    0xe9,                                                        // .byte 0xe9
    0x00, 0x00, 0x00, 0x00,                                      // .long 0
    0xcc,                                                        // int3 # if failed to write, then raise execption nya
                                                                 // payload_pid:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xc3,                                                        // ret
                                                                 // getpid:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // kill:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // payload_console_fd:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xc3,                                                        // ret
                                                                 // __sys_rdup:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // dup2:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // printf:
    0x48, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // movabs rax, -1
    0xff, 0xe0,                                                  // jmp rax
                                                                 // __export_post_prx_load_append_module_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x53,                                                        // push	rbx
    0x50,                                                        // push	rax
    0xe8, 0x93, 0xff, 0xff, 0xff,                                // call	__export_post_prx_load_append_module
    0x89, 0xc3,                                                  // mov	ebx, eax
    0xe8, 0x9c, 0xff, 0xff, 0xff,                                // call	payload_pid
    0x89, 0xc7,                                                  // mov	edi, eax
    0xbe, 0x1f, 0x00, 0x00, 0x00,                                // mov	esi, 31
    0xe8, 0xa7, 0xff, 0xff, 0xff,                                // call	kill
    0xe8, 0x96, 0xff, 0xff, 0xff,                                // call	getpid
    0x89, 0xc7,                                                  // mov	edi, eax
    0xbe, 0x11, 0x00, 0x00, 0x00,                                // mov	esi, 17
    0xe8, 0x96, 0xff, 0xff, 0xff,                                // call	kill
    0x89, 0xd8,                                                  // mov	eax, ebx
    0x48, 0x83, 0xc4, 0x08,                                      // add	rsp, 8
    0x5b,                                                        // pop	rbx
    0x5d,                                                        // pop	rbp
    0xc3,                                                        // ret
                                                                 // __export_init_env_hook:
    0x55,                                                        // push	rbp
    0x48, 0x89, 0xe5,                                            // mov	rbp, rsp
    0x5d,                                                        // pop	rbp
    0xc3,                                                        // ret
};
// clang-format on
