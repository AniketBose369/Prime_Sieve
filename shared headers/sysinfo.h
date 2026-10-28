#include"..\..\shared headers\mytypes.h"
#include"..\..\shared headers\comp.h"

struct CACHE{
    DWORD CacheSize;
    DWORD CacheLineSize;
}cache[5]={0};

DWORD corecount;
SIZE_T PageSize,LargePageMinimumSize,maxlevel_cachesize;
unsigned char maxlevel;

void printcacheinfo()
{
    print("Total number of cores: %d",corecount);
    for(char i=1;cache[i].CacheSize!=0;i++)print("L%d Cache(Data/Unified) Size per Core: %d, Line Size: %d",i,cache[i].CacheSize,cache[i].CacheLineSize);
}

void printpageinfo(SYSTEM_INFO* SystemInfo)
{
    print("PageSize: %d, MinimumApplicationAddress: %llu, MaximumApplicationAddress: %llu, AllocationGranularity: %d",SystemInfo->dwPageSize,(u64)SystemInfo->lpMinimumApplicationAddress,(u64)SystemInfo->lpMaximumApplicationAddress,SystemInfo->dwAllocationGranularity);
}

void getcacheinfo(){

    if(!corecount)
    {

        SYSTEM_LOGICAL_PROCESSOR_INFORMATION *buffer = 0,*buff_start;
        u64 buff_end;
        DWORD buffer_size = 0;
        DWORD byteOffset = 0;

        GetLogicalProcessorInformation(0, &buffer_size);
        buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
        GetLogicalProcessorInformation(&buffer[0], &buffer_size);

        buff_start=buffer;
        buff_end=(u64)buffer+buffer_size;

        for(;(u64)buffer < buff_end;buffer++) {
        switch(buffer->Relationship) {
            case RelationProcessorCore:
                (corecount)++;
                break;
            case RelationCache:
                //const char* cache_type[4]={"Unified","Instruction","Data","Trace"};
                if(((buffer->Cache.Type==CacheUnified)|(buffer->Cache.Type==CacheData))&(__popcnt64(buffer->ProcessorMask)==1))
                {
                    cache[buffer->Cache.Level].CacheSize+=buffer->Cache.Size;
                    cache[buffer->Cache.Level].CacheLineSize=buffer->Cache.LineSize;
                    if(maxlevel < buffer->Cache.Level)
                        maxlevel=buffer->Cache.Level;
                }
                // print("Cache Type: %s, Level: %d, Associativity: %d, LineSize: %d, CacheSize: %d\n",cache_type[buffer->Cache.Type],buffer->Cache.Level,buffer->Cache.Associativity,buffer->Cache.LineSize,buffer->Cache.Size);
                    break;
            }
        }
        free(buff_start);
        maxlevel_cachesize=cache[maxlevel].CacheSize;
        for(unsigned char i=1;cache[i].CacheSize!=0;i++)cache[i].CacheSize=cache[i].CacheSize/corecount;
    }

}

void getpagesize(){
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    PageSize=SystemInfo.dwPageSize;
    LargePageMinimumSize=(DWORD)GetLargePageMinimum();
    //printpageinfo(&SystemInfo);
}