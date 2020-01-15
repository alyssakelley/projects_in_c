// Author: Alyssa Kelley
// Assignment 5 - Part 1

# Execution begins at address 0 
    .pos 0
    irmovq stack, %rsp      # Set up stack pointer
    call main       # Execute main program
    halt            # Terminate program 

# Array of 10 elements
    .align 8
array:  .quad 0x000000000001
    .quad 0x000000000002
    .quad 0x000000000007
    .quad 0x000000000003
    .quad 0x000000000008
    .quad 0x000000000010
    .quad 0x000000000009
    .quad 0x000000000006
    .quad 0x000000000004
    .quad 0x000000000005

 #irmovq $8, %r9
  #  addq    %r9, %r8
   # rrmovq %r8, %rsi
main:   irmovq array, %rdi
    call sortIntArray       #sortIntArray(array)
    ret

swap:
        mrmovq    (%rdi), %rax
        mrmovq    (%rsi), %rdx
        rmmovq    %rdx, (%rdi)
        rmmovq    %rax, (%rsi)
        ret
// EDIT THIS TO REMOVE LEAQ/CMPL/MULT ETC.
# Execution begins at address 0 
    .pos 0
    irmovq stack, %rsp      # Set up stack pointer
    call main       # Execute main program
    halt            # Terminate program 

# Array of 10 elements
    .align 8
array:  .quad 0x000000000001
    .quad 0x000000000002
    .quad 0x000000000007
    .quad 0x000000000003
    .quad 0x000000000008
    .quad 0x000000000010
    .quad 0x000000000009
    .quad 0x000000000006
    .quad 0x000000000004
    .quad 0x000000000005

 #irmovq $8, %r9
  #  addq    %r9, %r8
   # rrmovq %r8, %rsi
main:   irmovq array, %rdi
    call sortIntArray       #sortIntArray(array)
    ret

swap:
        mrmovq    (%rdi), %rax
        mrmovq    (%rsi), %rdx
        rmmovq    %rdx, (%rdi)
        rmmovq    %rax, (%rsi)
        ret

sortIntArray:
        rrmovq    %rdi, %rbx
        irmovq $0, %r9
        jmp     .L2
.L4:
        irmovq $1, %r14
        addq %r14, %eax
.L3:
        #cmpl    %eax, %r8d
        rrmovq %r8, %r14
        subq %eax, %r14
        jle     .L9
        movq    8(%rbx), %rcx
        rrmovq  %eax, %rdx
        rrmovq  %edi, %rsi
        # movl    (%rcx,%rsi,4), %esi
        # esi = rcx + 4 * rsi
        irmovq $4, %r14
        addq %r14, %rcx
    .MUL:
        rrmovq %rsi, %r13
        irmovq $1, %r12
        addq %rcx, %rcx
        subq %r12, %r13
        jne .MUL
    .MUL2:
        addq %rcx, %esi
        
        # cmpl    %esi, (%rcx,%rdx,4)
        
        jge     .L4
        rrmovq    %eax, %edi
        jmp     .L4
.L9:
        movq    8(%rbx), %rax
        rrmovq  %r9, %r9
        rrmovq  %edi, %rdi
        leaq    (%rax,%r9,4), %rsi
        leaq    (%rax,%rdi,4), %rdi
        irmovq    $0, %eax
        call    swap
        rrmovq   %ebp, %r9d
.L2:
        rrmovq    (%rbx), %r8
        # leal    -1(%r8), %eax
        # %eax = %r8 + -1
        irmovq $1, %r14
        addq %r14, %r8
        rrmovq %r8, %eax
        
        # cmpl    %r9, %eax
        # %eax - %r9
        rrmovq %eax, %r14
        subq %r9, %r14
        jle     .L10
        # leal    1(%r9), %ebp
        # ebp = r9 + 1
        irmovq $1, %r14
        addq %r14, %r9
        rrmovq %r9, %ebp
        rrmovq    %r9, %edi
        rrmovq    %ebp, %eax
        jmp     .L3
.L10:
        irmovq $8, %r14
        addq    %r14, %rsp
        popq    %rbx
        popq    %rbp
        ret

# Stack starts here and grows to lower addresses
    .pos 0x200
stack:




/*       
sortIntArray:
        rrmovq    %rdi, %rbx
        jmp     .L2
.L4:
        addl    $1, %eax
.L3:
        cmpl    %eax, %r8d
        jle     .L9
        movq    8(%rbx), %rcx
        movslq  %eax, %rdx
        movslq  %edi, %rsi
        movl    (%rcx,%rsi,4), %esi
        cmpl    %esi, (%rcx,%rdx,4)
        jge     .L4
        movl    %eax, %edi
        jmp     .L4
.L9:
        movq    8(%rbx), %rax
        movslq  %r9d, %r9
        movslq  %edi, %rdi
        leaq    (%rax,%r9,4), %rsi
        leaq    (%rax,%rdi,4), %rdi
        movl    $0, %eax
        call    swap
        movl    %ebp, %r9d
.L2:
        movl    (%rbx), %r8
        leal    -1(%r8), %eax
        cmpl    %r9d, %eax
        jle     .L10
        leal    1(%r9), %ebp
        movl    %r9d, %edi
        movl    %ebp, %eax
        jmp     .L3
.L10:
        addq    $8, %rsp
        popq    %rbx
        popq    %rbp
        ret

# Stack starts here and grows to lower addresses
    .pos 0x200
stack:

*/