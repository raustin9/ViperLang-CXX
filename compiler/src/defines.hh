#pragma once

/*
 *  This file contains type definitions, macros, and other useful things
 *  that will be used across the application
 */

// TODO: Platform detection

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

// FOR C  -- boolean types //
//typedef char b8;
//typedef int b32;
//#define TRUE 1
//#define FALSE 0

// TODO: add static assertions

// Platform Detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define H_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64 bit is required for windows"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define H_PLATFORM_LINUX 1
#elif defined(__unix__)
#define H_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define H_PLATFORM_POSIX 1
#elif __APPLE__
#include <TargetConditionals.h>
#define H_PLATFORM_APPLE 1
#endif

#ifdef HEXPORT
#ifdef _MSC_VER
#define HAPI __declspec(dllexport)
#else
#define HAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define HAPI __declspec(dllimport)
#else
#define HAPI
#endif
#endif


