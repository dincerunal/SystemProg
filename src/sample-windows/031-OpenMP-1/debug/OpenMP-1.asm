; Listing generated by Microsoft (R) Optimizing Compiler Version 19.15.26726.0 

	TITLE	C:\Dropbox\Shared\Kurslar\Netas-SysProg-2018-Agustos\Src\Sample-Windows\031-OpenMP-1\OpenMP-1.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

msvcjmc	SEGMENT
__320E01E0_corecrt_stdio_config.h DB 01H
__BAC7FC50_corecrt_wstdio.h DB 01H
__A3797CDC_stdio.h DB 01H
__7C512EE2_ctype.h DB 01H
__5BEBA810_basetsd.h DB 01H
__07523DAF_corecrt_memcpy_s.h DB 01H
__9CE3A74D_corecrt_wstring.h DB 01H
__78F4D6C6_string.h DB 01H
__1D5C983A_winnt.h DB 01H
__4D75C327_processthreadsapi.h DB 01H
__A1C010E7_memoryapi.h DB 01H
__3CD9AC13_winerror.h DB 01H
__7D6EA4B0_winbase.h DB 01H
__ED00179D_winioctl.h DB 01H
__49821EBB_stralign.h DB 01H
__6C6CFDD1_openmp-1.c DB 01H
msvcjmc	ENDS
PUBLIC	___local_stdio_printf_options
PUBLIC	__vfprintf_l
PUBLIC	_printf
PUBLIC	_main
PUBLIC	??_C@_03PPOCCAPH@?$CFf?6@			; `string'
EXTRN	__imp____acrt_iob_func:PROC
EXTRN	__imp____stdio_common_vfprintf:PROC
EXTRN	__imp__QueryPerformanceCounter@4:PROC
EXTRN	__imp__QueryPerformanceFrequency@4:PROC
EXTRN	@_RTC_CheckStackVars@8:PROC
EXTRN	@__CheckForDebuggerJustMyCode@4:PROC
EXTRN	__RTC_CheckEsp:PROC
EXTRN	__RTC_InitBase:PROC
EXTRN	__RTC_Shutdown:PROC
EXTRN	__ltod3:PROC
EXTRN	__vcomp_for_static_end:PROC
EXTRN	__vcomp_for_static_simple_init_i8:PROC
EXTRN	__vcomp_fork:PROC
EXTRN	__You_must_link_with_Microsoft_OpenMP_library:DWORD
EXTRN	__fltused:DWORD
_DATA	SEGMENT
COMM	?_OptionsStorage@?1??__local_stdio_printf_options@@9@9:QWORD							; `__local_stdio_printf_options'::`2'::_OptionsStorage
_DATA	ENDS
;	COMDAT rtc$TMZ
rtc$TMZ	SEGMENT
__RTC_Shutdown.rtc$TMZ DD FLAT:__RTC_Shutdown
rtc$TMZ	ENDS
;	COMDAT rtc$IMZ
rtc$IMZ	SEGMENT
__RTC_InitBase.rtc$IMZ DD FLAT:__RTC_InitBase
rtc$IMZ	ENDS
;	COMDAT ??_C@_03PPOCCAPH@?$CFf?6@
CONST	SEGMENT
??_C@_03PPOCCAPH@?$CFf?6@ DB '%f', 0aH, 00H		; `string'
CONST	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
;	COMDAT _main$omp$1
_TEXT	SEGMENT
$T1 = -284						; size = 8
$T2 = -268						; size = 8
_i$3 = -60						; size = 8
_$S3$4 = -44						; size = 8
_$S2$5 = -28						; size = 8
_$S1$6 = -12						; size = 8
_main$omp$1 PROC					; COMDAT
; File c:\dropbox\shared\kurslar\netas-sysprog-2018-agustos\src\sample-windows\031-openmp-1\openmp-1.c
; Line 15
	push	ebp
	mov	ebp, esp
	sub	esp, 288				; 00000120H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-288]
	mov	ecx, 72					; 00000048H
	mov	eax, -858993460				; ccccccccH
	rep stosd
; Line 16
	xorps	xmm0, xmm0
	movlpd	QWORD PTR _$S1$6[ebp], xmm0
	mov	DWORD PTR _$S3$4[ebp], 1
	mov	DWORD PTR _$S3$4[ebp+4], 0
	mov	DWORD PTR _$S2$5[ebp], 1000000000	; 3b9aca00H
	mov	DWORD PTR _$S2$5[ebp+4], 0
	mov	eax, DWORD PTR _$S1$6[ebp+4]
	cmp	eax, DWORD PTR _$S2$5[ebp+4]
	jg	$LN1@main$omp$1
	jl	SHORT $LN3@main$omp$1
	mov	ecx, DWORD PTR _$S1$6[ebp]
	cmp	ecx, DWORD PTR _$S2$5[ebp]
	jae	$LN1@main$omp$1
$LN3@main$omp$1:
	mov	edx, DWORD PTR _$S2$5[ebp]
	sub	edx, 1
	mov	eax, DWORD PTR _$S2$5[ebp+4]
	sbb	eax, 0
	mov	DWORD PTR _$S2$5[ebp], edx
	mov	DWORD PTR _$S2$5[ebp+4], eax
	lea	eax, DWORD PTR $T1[ebp]
	push	eax
	lea	ecx, DWORD PTR $T2[ebp]
	push	ecx
	push	1
	mov	edx, DWORD PTR _$S3$4[ebp+4]
	push	edx
	mov	eax, DWORD PTR _$S3$4[ebp]
	push	eax
	mov	ecx, DWORD PTR _$S2$5[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _$S2$5[ebp]
	push	edx
	mov	eax, DWORD PTR _$S1$6[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _$S1$6[ebp]
	push	ecx
	call	__vcomp_for_static_simple_init_i8
	add	esp, 36					; 00000024H
	mov	eax, DWORD PTR $T2[ebp]
	mov	DWORD PTR _i$3[ebp], eax
	mov	ecx, DWORD PTR $T2[ebp+4]
	mov	DWORD PTR _i$3[ebp+4], ecx
	jmp	SHORT $LN2@main$omp$1
$LN2@main$omp$1:
	mov	edx, DWORD PTR _i$3[ebp]
	add	edx, DWORD PTR _$S3$4[ebp]
	mov	eax, DWORD PTR _i$3[ebp+4]
	adc	eax, DWORD PTR _$S3$4[ebp+4]
	mov	DWORD PTR _i$3[ebp], edx
	mov	DWORD PTR _i$3[ebp+4], eax
$LN2@main$omp$1:
	mov	ecx, DWORD PTR _i$3[ebp+4]
	cmp	ecx, DWORD PTR $T1[ebp+4]
	jg	SHORT $LN3@main$omp$1
	jl	SHORT $LN4@main$omp$1
	mov	edx, DWORD PTR _i$3[ebp]
	cmp	edx, DWORD PTR $T1[ebp]
	ja	SHORT $LN3@main$omp$1
$LN4@main$omp$1:
	jmp	SHORT $LN2@main$omp$1
$LN3@main$omp$1:
	call	__vcomp_for_static_end
$LN1@main$omp$1:
; Line 18
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 288				; 00000120H
	cmp	ebp, esp
	call	__RTC_CheckEsp
	mov	esp, ebp
	pop	ebp
	ret	0
_main$omp$1 ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
;	COMDAT _main
_TEXT	SEGMENT
tv170 = -280						; size = 8
tv168 = -280						; size = 8
_result$ = -76						; size = 8
_freq$ = -60						; size = 8
_li2$ = -44						; size = 8
_li1$ = -28						; size = 8
_i$ = -12						; size = 8
_main	PROC						; COMDAT
; File c:\dropbox\shared\kurslar\netas-sysprog-2018-agustos\src\sample-windows\031-openmp-1\openmp-1.c
; Line 6
	push	ebp
	mov	ebp, esp
	sub	esp, 280				; 00000118H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-280]
	mov	ecx, 70					; 00000046H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	ecx, OFFSET __6C6CFDD1_openmp-1.c
	call	@__CheckForDebuggerJustMyCode@4
; Line 11
	mov	esi, esp
	lea	eax, DWORD PTR _freq$[ebp]
	push	eax
	call	DWORD PTR __imp__QueryPerformanceFrequency@4
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 13
	mov	esi, esp
	lea	eax, DWORD PTR _li1$[ebp]
	push	eax
	call	DWORD PTR __imp__QueryPerformanceCounter@4
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 15
	push	OFFSET _main$omp$1
	push	0
	push	1
	call	__vcomp_fork
	add	esp, 12					; 0000000cH
; Line 20
	mov	esi, esp
	lea	eax, DWORD PTR _li2$[ebp]
	push	eax
	call	DWORD PTR __imp__QueryPerformanceCounter@4
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 22
	mov	eax, DWORD PTR _li2$[ebp]
	sub	eax, DWORD PTR _li1$[ebp]
	mov	ecx, DWORD PTR _li2$[ebp+4]
	sbb	ecx, DWORD PTR _li1$[ebp+4]
	mov	DWORD PTR _result$[ebp], eax
	mov	DWORD PTR _result$[ebp+4], ecx
; Line 23
	mov	edx, DWORD PTR _result$[ebp+4]
	mov	ecx, DWORD PTR _result$[ebp]
	call	__ltod3
	mov	edx, DWORD PTR _freq$[ebp+4]
	mov	ecx, DWORD PTR _freq$[ebp]
	movsd	QWORD PTR tv168[ebp], xmm0
	call	__ltod3
	movsd	xmm1, QWORD PTR tv168[ebp]
	divsd	xmm1, xmm0
	sub	esp, 8
	movsd	QWORD PTR [esp], xmm1
	push	OFFSET ??_C@_03PPOCCAPH@?$CFf?6@
	call	_printf
	add	esp, 12					; 0000000cH
; Line 25
	mov	esi, esp
	lea	eax, DWORD PTR _li1$[ebp]
	push	eax
	call	DWORD PTR __imp__QueryPerformanceCounter@4
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 28
	xorps	xmm0, xmm0
	movlpd	QWORD PTR _i$[ebp], xmm0
	jmp	SHORT $LN6@main
$LN4@main:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	ecx, DWORD PTR _i$[ebp+4]
	adc	ecx, 0
	mov	DWORD PTR _i$[ebp], eax
	mov	DWORD PTR _i$[ebp+4], ecx
$LN6@main:
	cmp	DWORD PTR _i$[ebp+4], 0
	jg	SHORT $LN5@main
	jl	SHORT $LN8@main
	cmp	DWORD PTR _i$[ebp], 1000000000		; 3b9aca00H
	jae	SHORT $LN5@main
$LN8@main:
; Line 30
	jmp	SHORT $LN4@main
$LN5@main:
; Line 32
	mov	esi, esp
	lea	eax, DWORD PTR _li2$[ebp]
	push	eax
	call	DWORD PTR __imp__QueryPerformanceCounter@4
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 34
	mov	eax, DWORD PTR _li2$[ebp]
	sub	eax, DWORD PTR _li1$[ebp]
	mov	ecx, DWORD PTR _li2$[ebp+4]
	sbb	ecx, DWORD PTR _li1$[ebp+4]
	mov	DWORD PTR _result$[ebp], eax
	mov	DWORD PTR _result$[ebp+4], ecx
; Line 35
	mov	edx, DWORD PTR _result$[ebp+4]
	mov	ecx, DWORD PTR _result$[ebp]
	call	__ltod3
	mov	edx, DWORD PTR _freq$[ebp+4]
	mov	ecx, DWORD PTR _freq$[ebp]
	movsd	QWORD PTR tv170[ebp], xmm0
	call	__ltod3
	movsd	xmm1, QWORD PTR tv170[ebp]
	divsd	xmm1, xmm0
	sub	esp, 8
	movsd	QWORD PTR [esp], xmm1
	push	OFFSET ??_C@_03PPOCCAPH@?$CFf?6@
	call	_printf
	add	esp, 12					; 0000000cH
; Line 37
	xor	eax, eax
; Line 38
	push	edx
	mov	ecx, ebp
	push	eax
	lea	edx, DWORD PTR $LN13@main
	call	@_RTC_CheckStackVars@8
	pop	eax
	pop	edx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 280				; 00000118H
	cmp	ebp, esp
	call	__RTC_CheckEsp
	mov	esp, ebp
	pop	ebp
	ret	0
$LN13@main:
	DD	3
	DD	$LN12@main
$LN12@main:
	DD	-28					; ffffffe4H
	DD	8
	DD	$LN9@main
	DD	-44					; ffffffd4H
	DD	8
	DD	$LN10@main
	DD	-60					; ffffffc4H
	DD	8
	DD	$LN11@main
$LN11@main:
	DB	102					; 00000066H
	DB	114					; 00000072H
	DB	101					; 00000065H
	DB	113					; 00000071H
	DB	0
$LN10@main:
	DB	108					; 0000006cH
	DB	105					; 00000069H
	DB	50					; 00000032H
	DB	0
$LN9@main:
	DB	108					; 0000006cH
	DB	105					; 00000069H
	DB	49					; 00000031H
	DB	0
_main	ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
;	COMDAT _printf
_TEXT	SEGMENT
__ArgList$ = -20					; size = 4
__Result$ = -8						; size = 4
__Format$ = 8						; size = 4
_printf	PROC						; COMDAT
; File c:\program files (x86)\windows kits\10\include\10.0.17134.0\ucrt\stdio.h
; Line 954
	push	ebp
	mov	ebp, esp
	sub	esp, 216				; 000000d8H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-216]
	mov	ecx, 54					; 00000036H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	ecx, OFFSET __A3797CDC_stdio.h
	call	@__CheckForDebuggerJustMyCode@4
; Line 957
	lea	eax, DWORD PTR __Format$[ebp+4]
	mov	DWORD PTR __ArgList$[ebp], eax
; Line 958
	mov	eax, DWORD PTR __ArgList$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR __Format$[ebp]
	push	ecx
	mov	esi, esp
	push	1
	call	DWORD PTR __imp____acrt_iob_func
	add	esp, 4
	cmp	esi, esp
	call	__RTC_CheckEsp
	push	eax
	call	__vfprintf_l
	add	esp, 16					; 00000010H
	mov	DWORD PTR __Result$[ebp], eax
; Line 959
	mov	DWORD PTR __ArgList$[ebp], 0
; Line 960
	mov	eax, DWORD PTR __Result$[ebp]
; Line 961
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 216				; 000000d8H
	cmp	ebp, esp
	call	__RTC_CheckEsp
	mov	esp, ebp
	pop	ebp
	ret	0
_printf	ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
;	COMDAT __vfprintf_l
_TEXT	SEGMENT
__Stream$ = 8						; size = 4
__Format$ = 12						; size = 4
__Locale$ = 16						; size = 4
__ArgList$ = 20						; size = 4
__vfprintf_l PROC					; COMDAT
; File c:\program files (x86)\windows kits\10\include\10.0.17134.0\ucrt\stdio.h
; Line 642
	push	ebp
	mov	ebp, esp
	sub	esp, 192				; 000000c0H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-192]
	mov	ecx, 48					; 00000030H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	ecx, OFFSET __A3797CDC_stdio.h
	call	@__CheckForDebuggerJustMyCode@4
; Line 643
	mov	esi, esp
	mov	eax, DWORD PTR __ArgList$[ebp]
	push	eax
	mov	ecx, DWORD PTR __Locale$[ebp]
	push	ecx
	mov	edx, DWORD PTR __Format$[ebp]
	push	edx
	mov	eax, DWORD PTR __Stream$[ebp]
	push	eax
	call	___local_stdio_printf_options
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR [eax]
	push	edx
	call	DWORD PTR __imp____stdio_common_vfprintf
	add	esp, 24					; 00000018H
	cmp	esi, esp
	call	__RTC_CheckEsp
; Line 644
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 192				; 000000c0H
	cmp	ebp, esp
	call	__RTC_CheckEsp
	mov	esp, ebp
	pop	ebp
	ret	0
__vfprintf_l ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
;	COMDAT ___local_stdio_printf_options
_TEXT	SEGMENT
___local_stdio_printf_options PROC			; COMDAT
; File c:\program files (x86)\windows kits\10\include\10.0.17134.0\ucrt\corecrt_stdio_config.h
; Line 85
	push	ebp
	mov	ebp, esp
	sub	esp, 192				; 000000c0H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-192]
	mov	ecx, 48					; 00000030H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	ecx, OFFSET __320E01E0_corecrt_stdio_config.h
	call	@__CheckForDebuggerJustMyCode@4
; Line 87
	mov	eax, OFFSET ?_OptionsStorage@?1??__local_stdio_printf_options@@9@9 ; `__local_stdio_printf_options'::`2'::_OptionsStorage
; Line 88
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 192				; 000000c0H
	cmp	ebp, esp
	call	__RTC_CheckEsp
	mov	esp, ebp
	pop	ebp
	ret	0
___local_stdio_printf_options ENDP
_TEXT	ENDS
END