#pragma once

#include "log.h"
#include "zlib.h"


extern bool sameTextCRC32;
extern bool sameTextCRC32_sys;


typedef struct {
    char *buf;
    uint32_t size;
    uint32_t offset;
} LocalTextSegInfo;


__attribute__((constructor()))
void detectCRC32();

__attribute__((constructor()))
void detectCRC32WithSyscall();

uint32_t getLocalTextCRC32(const char* pathanme, uint32_t* offset, uint32_t* size, bool useSyscall);

uint32_t getMapsTextCRC32(const char *soName, uint32_t offset, uint32_t size);

uint32_t calcCRC32(char *message, size_t message_length);

void *GetModuleBase(const char *name);

