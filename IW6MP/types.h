#ifndef _TYPES_H
#define _TYPES_H

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned __int64	u64;
typedef unsigned __int64	QWORD;

typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed __int64		s64;
typedef unsigned char       BYTE;
typedef char CHAR;

typedef long			NTSTATUS;

typedef QWORD near           *PQWORD;
typedef QWORD far            *LPQWORD;

#define EXPORTNUM(x)

#define USER_PROC	1

#define NT_SUCCESS(Status)              (((NTSTATUS)(Status)) >= 0)

#endif