#include "detect-crc32.h"
#include "elf-parser.h"


void* GetModuleBase(const char *name) {
    FILE *maps;
    char buff[256];
    off_t load_addr;
    int found = 0;
    maps = fopen("/proc/self/maps", "r");
    while (fgets(buff, sizeof(buff), maps)) {
        if ((strstr(buff, "r-xp") || strstr(buff, "r--p")) && strstr(buff, name)) {
            found = 1;
            __android_log_print(ANDROID_LOG_DEBUG, "dlopen", "%s\n", buff);
            break;
        }
    }

    if (!found) {
        LOGE("failed to read load address for %s", name);
        return nullptr;
    }

    if (sscanf(buff, "%lx", &load_addr) != 1)
        LOGE("failed to read load address for %s", name);

    fclose(maps);

    LOGD("Get module baseInRam %s: %lu", name, load_addr);

    return reinterpret_cast<void *>(load_addr);
}


uint32_t calcCRC32(char* message, size_t message_length){
    uint32_t crc = crc32(0L, (const Bytef *)message, message_length);
    return crc;
}

// 獲取指定文件的.text段的CRC32值
uint32_t getLocalTextCRC32(const char* pathanme, uint32_t& offset, uint32_t& size){
    int32_t fd;
    Elf32_Ehdr eh;		/* elf-header is fixed size */

    fd = open(pathanme, O_RDONLY|O_SYNC);
    if(fd < 0) {
        LOGE("Error %d Unable to open %s\n", fd, pathanme);
        return -1;
    }

    /* ELF header : at start of file */
    read_elf_header(fd, &eh);
    if(!is_ELF(eh)) {
        return -1;
    }
    if(is64Bit(eh)){
        Elf64_Ehdr eh64;	/* elf-header is fixed size */
        Elf64_Shdr* sh_tbl;	/* section-header table is variable size */

        read_elf_header64(fd, &eh64);

        /* Section header table :  */
        sh_tbl = static_cast<Elf64_Shdr *>(malloc(eh64.e_shentsize * eh64.e_shnum));
        if(!sh_tbl) {
            LOGE("Failed to allocate %d bytes\n",
                 (eh64.e_shentsize * eh64.e_shnum));
        }
        read_section_header_table64(fd, eh64, sh_tbl);

        /* Save .text section as text.S
        */
        LocalTextSegInfo* localTextSegInfo = save_text_section64(fd, eh64, sh_tbl);

        uint32_t crcVal = calcCRC32(localTextSegInfo->buf, localTextSegInfo->size);
//        LOGI("local libc.so crc32 = %p, size = %p", crcVal, crcInfo->size);

        offset = localTextSegInfo->offset;
        size = localTextSegInfo->size;
        return crcVal;


    } else{
        LOGI("暫時未能處理32位的文件.....");
    }

    return -1;
}


uint32_t getMapsTextCRC32(const char* soName, uint32_t offset, uint32_t size){
    void* base = GetModuleBase(soName);


    uint32_t crcVal = calcCRC32((char*)((uintptr_t)base + offset), size);

    return crcVal;

}

bool sameTextCRC32;

__attribute__((constructor()))
void detectCRC32(){

    // Env: pixelXL1 aosp8
    const char* localLibcPath = "/system/lib64/libc.so";
    uint32_t offset, size;
    uint32_t localTextCRC32 = getLocalTextCRC32(localLibcPath, offset, size);

    LOGD("local libc.so CRC32 = %p", localTextCRC32);


    uint32_t mapsTextCRC32 = getMapsTextCRC32("libc.so", offset, size);

    LOGD("maps libc.so CRC32 = %p", mapsTextCRC32);

    sameTextCRC32 = (localTextCRC32 == mapsTextCRC32);
}