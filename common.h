#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>

#define STRINGIFY(x) #x
#define STRINGIFY_DEEP(x) STRINGIFY(x)

#define JOIN_HELPER(x, y) x##y
#define JOIN(x, y) JOIN_HELPER(x, y)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ALIGN_UP(x, alignment) (((x) + ((alignment) - 1)) & ~((alignment) - 1))
#define ALIGN_DOWN(x, alignment) ((x) & ~((alignment) - 1))

#define UNUSED(x) (void)(x)

#define EPRINTF(msg, ...) printf("Error at %s:%s(%d): " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define TYPE_PAD(size) char JOIN(_pad_, __COUNTER__)[size]
#define TYPE_VARIADIC_BEGIN(name) name { union {
#define TYPE_BEGIN(name, size) name { union { TYPE_PAD(size)
#define TYPE_END(...) }; } __VA_ARGS__
#define TYPE_FIELD(field, offset) struct { TYPE_PAD(offset); field; }

#define TYPE_CHECK_SIZE(name, size) \
	_Static_assert(sizeof(name) == (size), "Size of " #name " != " #size)

#define TYPE_CHECK_FIELD_OFFSET(name, member, offset) \
	_Static_assert(offsetof(name, member) == (offset), "Offset of " #name "." #member " != " #offset)

#define TYPE_CHECK_FIELD_SIZE(name, member, size) \
	_Static_assert(sizeof(((name*)0)->member) == (size), "Size of " #name "." #member " != " #size)

extern const char* g_sandbox_word;
