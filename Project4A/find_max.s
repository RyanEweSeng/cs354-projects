	.file	"find_max_template.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"The length of the array is %d\n"
	.text
	.globl	Find_Max
	.type	Find_Max, @function
Find_Max:
// prologue
    endbr64                         # security command 
    pushq   %rbp                    # store caller base pointer onto the stack for the caller return
    movq    %rsp, %rbp              # update base pointer to prepare for the new stack frame   

// reserve space for local variables
// Table containing the variables from your c code and the addresses used in assembly 
/* VARIABLE NAME    | ADDRESS                                                     | VALUE
 * n                |   %rdi            -> -32(%rbp)    -> %esi                   |
 * a                |   %rsi            -> -28(%rbp)    -> %rax                   |
 * max_index        |   -20(%rbp)       -> %edx                                   |   0
 * i                |   -16(%rbp)       -> %edx                                   |   1             (this treats i as an index)    
 * i (condition)    |   -16(%rbp)       -> %edi                                   |                 (this is when checking i < n)
 * a[max_index]     |   (%rax, %rdx, 4) -> %esi                                   |
 * a[i]             |   (%rax, %rdx, 4) -> %edi                                   |
*/
    subq    $32, %rsp               # reserve space on stack for local variables (n, *a, max_index, i)
    movq    %rdi, -32(%rbp)         # move the value of n (stored in the 1st argument register rdi) onto the stack; it requires 4 bits of space as it is an int
    movq    %rsi, -28(%rbp)         # move the value of a (store in the 2nd argument register rsi) onto the stack; it requires 8 bits of space as it is a pointer
    movl    $0, -20(%rbp)           # initialize max_index to 0 and store it on the stack; it requires 4 bits of space as it is an int
    movl    $1, -16(%rbp)           # initialize i to 1 and store it on the stack; it requires 4 bits of space as it is an int

// printf("The length of the array is %d\n", n);
// update this code if necessary to print the length of the array 
// this block prints the value of register %edi; initially the parameter n
	movl    %edi, %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

// This label corresponds with the condition part of the for loop
LOOP_COND:
// These 4 lines correspond with the i < n part of the for loop
    movl    -16(%rbp), %edi         # move i into %edi to prepare for comparison
    movl    -32(%rbp), %esi         # move n into %esi to prepare for comparison
    cmpl    %edi, %esi              # compare n and i (n - i)
    jle     EXIT_LOOP               # jump when compare gives less than equals to 0 (exit the loop; jump to EXIT_LOOP)

// This label signifies the body of the loop (printf and condition check)
LOOP_BODY:
// Call Print_One_Number on the element at index i of the array i.e. a[i]
    movq    -28(%rbp), %rax         # move array base to %rax to prepare for addressing mode
    movl    -16(%rbp), %edx         # move index to %edx to prepare for addressing mode
    movl    (%rax,%rdx,4), %eax     # the following 3 lines just print a[i]
    movl    %eax, %edi
    call    Print_One_Number@PLT

// Perform the check for the index of the max element
// Prepare a[i] for comparison
    movq    -28(%rbp), %rax         # move a into %rax
    movl    -16(%rbp), %edx         # move i into %edx
    movl    (%rax,%rdx,4), %edi     # store a[i] for comparison    
// Prepare a[max_index] for comparison
    movq    -28(%rbp), %rax         # move a into %rax
    movl    -20(%rbp), %edx         # move max_index into %edx
    movl    (%rax,%rdx,4), %esi     # store a[max_index] for comparison
// Perform comparion
    cmpl    %esi, %edi              # this does a[i] - a[max_index] (so a result <= 0 will mean there is no new max)
    jle     LOOP_END                # if a[max_index] > a[i], then  we jump to the end of the loop (no new max so we skip the true block)

// This label corresponds to updating max_index if a[max_index] < a[i]
TRUE_BLOCK:
    movl    -16(%rbp), %edx         # load i (the new max_index) into %edx
    movl    %edx, -20(%rbp)         # move i into max_index

// This label corresponds with the end part (i++) of the for loop
LOOP_END:
// These 3 lines correspond with the i++ part of the for loop
    movq    -16(%rbp), %rdx         # load i back into %rdx
    incq    %rdx                    # increment i by 1
    movq    %rdx, -16(%rbp)         # update i by moving the new i (located in %ecx) back into -16(%rbp)

    jmp     LOOP_COND               # jump back to the start of the loop for the condition check

// This label is for what happens after the loop ends (exit the loop; condition check is false)
EXIT_LOOP:
// prepare the return value
	movl    -20(%rbp), %eax         # max_index was stored at -20(%rbp) which is the value we want to return

// epilogue
    mov     %rbp, %rsp              # update the stack pointer to the base pointer clear the stack
    pop     %rbp                    # pop the base pointer (we are going back down the stack so we move to the earlier base pointer)
	ret
.LFE0:
	.size	Find_Max, .-Find_Max
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
