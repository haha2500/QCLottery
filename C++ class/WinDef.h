//
//  WinDef.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// 下面是类型定义
typedef unsigned char BYTE, *LPBYTE;
typedef unsigned short WORD, *LPWORD;
typedef unsigned long DWORD, *LPDWORD;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef void *LPVOID;
typedef int *LPINT;
typedef signed char BOOL, *LPBOOL;

typedef void *HANDLE;

// 下面是相关宏定义
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

// 检查字符串是否越界，只适用于直接使用类似char szString[10]定义的变量
#define		CHECK_STRING_STACK(szString) ASSERT(strlen(szString) < sizeof(szString))
// 检查指定数据(datavalue)是否全部包含指定标志(flag)的宏，为TRUE则表示全部包含
#define		IS_INCLUDE_ALL_FLAG(datavalue, flag) ((((datavalue) & (flag)) == (flag)) ? TRUE : FALSE)
// 检查指定数据(datavalue)是否全部不包含指定标志(flag)的宏，为TRUE则表示全部不包含
#define		IS_EXCLUDE_ALL_FLAG(datavalue, flag) (((datavalue) & (flag)) ? FALSE : TRUE)

// 创建动态数组空间并进行初始化
#define		NEW_BUFFER(lpBuf, DATATYPE, nCount)  \
                ASSERT(lpBuf == NULL); \
                if(nCount > 0) { \
                    lpBuf = new DATATYPE[nCount]; \
                    ASSERT(lpBuf != NULL); \
                    ZeroMemory(lpBuf, sizeof(DATATYPE) * nCount); \
                }

// 释放动态数组空间，该空间是通过new XXX[n] 创建的
#define DELETE_BUFFER(buf) \
                if(buf != NULL) {delete []buf; buf = NULL;}

// 创建一般指针并进行初始化
#define		NEW_POINTER(lpPointer, DATATYPE) \
                ASSERT(lpPointer == NULL); \
                lpPointer = new DATATYPE; \
                ASSERT(lpPointer != NULL); \
                ZeroMemory(lpPointer, sizeof(DATATYPE)); 

// 释放一般指针
#define		DELETE_POINTER(lpPointer) if(lpPointer != NULL) {delete lpPointer; lpPointer = NULL;}

// 定义一些类型的最大值
#define MAXDWORD 0XFFFFFFFF
#define MAXWORD  (WORD)0XFFFF
#define MAXBYTE  (BYTE)0XFF
#define TRUE    1
#define FALSE   0
#undef NULL
#define NULL    0

// 定义无效句柄值
#define INVALID_HANDLE_VALUE (HANDLE)-1

// 定义调试诊断
#ifdef DEBUG
    #define ASSERT(x) assert(x)
#else
    #define ASSERT(x)
#endif

#define UNUSED_ALWAYS(x)
