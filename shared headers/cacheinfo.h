#define printcacheinfo(){\
DWORD buffer_size = 0;\
DWORD i = 0;\
SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;\
GetLogicalProcessorInformation(0, &buffer_size);\
buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);\
GetLogicalProcessorInformation(&buffer[0], &buffer_size);\
for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {\
    if(buffer[i].Relationship == RelationCache) {\
        printf("Level: %d, Associativity: %d, LineSize: %d, CacheSize: %d\n",buffer[i].Cache.Level,buffer[i].Cache.Associativity,buffer[i].Cache.LineSize,buffer[i].Cache.Size);\
    }\
}\
free(buffer);\
}