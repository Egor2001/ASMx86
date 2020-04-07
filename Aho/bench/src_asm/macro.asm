%macro multipush 1-*
	%rep %0
		push QWORD %1
		%rotate 1
	%endrep
%endmacro

%macro multipop 1-*
	%rep %0
		%rotate -1
		pop QWORD %1
	%endrep
%endmacro

%macro sysv_call 1-*
	%rotate 1
%endmacro

%macro sysv_prologue 0
%if __BITS__ != 64
	%error "non - 64 BITS arch uses sys V"
%endif

	push QWORD rbp
	mov rbp, rsp
	sub rbp, 2*8

	multipush rbx, r15, r14, r13, r12

%endmacro

%macro sysv_epilogue 0
%if __BITS__ != 64
	%error "non - 64 BITS arch uses sys V"
%endif

	multipop rbx, r15, r14, r13, r12
	pop QWORD rbp

%endmacro
