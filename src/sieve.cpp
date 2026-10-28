#include<windows.h>
#include<conio.h>
#include <iostream>
#include <string>
#include"..\..\shared headers\profilerv2.h"
#include"..\..\shared headers\comp.h"
#include"..\..\shared headers\mytypes.h"
#include"..\..\shared headers\sysinfo.h"

#define getmax_index(n) num2index_arr_max[n%30]+(((n-7)/30)<<3)
#define getmin_index(n) num2index_arr_min[n%30]+(((n-7)/30)<<3)
#define indextonum(n) (30*(n>>3))+index2num_arr[n&7]

u64 min_index,max_index,index_dist,rangemin,rangemax,primecount;
static char index2num_arr[8] = {7,11,13,17,19,23,29,31};
SIZE_T memorysize;
void* memory;

struct compositegaps{
    u64 arr[8];
    u64 offsetofnext_seq[8];
    u64 curr_starting_index;
    u64 offsetofnext_seq_starting_index;   
};

void allocsieve()
{
    getpagesize();
    getcacheinfo();
    // printf("\nTotal number of cores: %d, PageSize: %lld, LargePageMinimumSize: %lld,maxlevel_cachesize: %lld\n",corecount,PageSize,LargePageMinimumSize,maxlevel_cachesize);
    memorysize=(maxlevel_cachesize+64)*corecount;

    MEM_EXTENDED_PARAMETER ExtendedParameters={0};
    MEM_ADDRESS_REQUIREMENTS memaddressrequirement={0};

    unsigned long index;
    _BitScanReverse64(&index,memorysize);
    index++;

    memaddressrequirement.Alignment = (u64)1<<index;

    ExtendedParameters.Type=MemExtendedParameterAddressRequirements;
    ExtendedParameters.Pointer=&memaddressrequirement;

    memory=VirtualAlloc2(GetCurrentProcess(),0,memorysize,MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE,&ExtendedParameters,1);

    printf("\nMemory Start: %llu\n",(u64)memory);
}

void printprimes()
{
    HWND current_console_window_handle=GetConsoleWindow();
    DWORD current_process_pid;
    GetWindowThreadProcessId(current_console_window_handle,&current_process_pid);
    _getch();
    FreeConsole();
    AllocConsole();
    std::cout<<"2 3 5";
    for(u64 i=0;i<=max_index;i++)
    {
        if(!(getbit(memory,i))){
            printf(" %llu",indextonum(i));primecount++;
        }
        //printf("\n\nHello\n\n");
    }
    std::cout<<"\nTotal number of primes: "<<primecount+3<<"\n";
    _getch();
    FreeConsole();
    AttachConsole(current_process_pid);
    _getch();
}

void initcompositegaps()
{
    

    // for(int i=0;i<8;i++)
    // {
    //     printf("\ncompositegaps array[%d] ->\n curr_starting_index: %llu\n",i,array[i].curr_starting_index);
    //     printf("arr -> ");
    //     for(int j=0;j<8;j++)printf("%llu, ",array[i].arr[j]);
    //     printf("\n");
    //     for(int j=0;j<8;j++)printf("%llu, ",array[i].offsetofnext_seq[j]);
    //     printf("\n");
    // }
}

void threadfunc(void* thread_local_mem)
{
    compositegaps array[8];

    array[0].curr_starting_index=12;
    array[0].offsetofnext_seq_starting_index=352;
    initarr8(array[0].arr,7,4,7,4,7,12,3,12);
    initarr8(array[0].offsetofnext_seq,32,16,32,16,32,48,16,48);
    
    array[1].curr_starting_index=31;
    array[1].offsetofnext_seq_starting_index=416;
    initarr8(array[1].arr,6,11,6,12,18,5,18,12);
    initarr8(array[1].offsetofnext_seq,16,32,16,32,48,16,48,32);

    array[2].curr_starting_index=44;
    array[2].offsetofnext_seq_starting_index=448;
    initarr8(array[2].arr,13,7,14,21,7,21,14,7);
    initarr8(array[2].offsetofnext_seq,32,16,32,48,16,48,32,16);
    
    array[3].curr_starting_index=76;
    array[3].offsetofnext_seq_starting_index=512;
    initarr8(array[3].arr,9,18,27,9,27,18,9,19);
    initarr8(array[3].offsetofnext_seq,16,32,48,16,48,32,16,32);


    array[4].curr_starting_index=95;
    array[4].offsetofnext_seq_starting_index=544;
    initarr8(array[4].arr,20,30,11,30,20,10,21,10);
    initarr8(array[4].offsetofnext_seq,32,48,16,48,32,16,32,16);

    array[5].curr_starting_index=140;
    array[5].offsetofnext_seq_starting_index=608;
    initarr8(array[5].arr,36,13,36,25,12,25,12,25);
    initarr8(array[5].offsetofnext_seq,48,16,48,32,16,32,16,32);

    array[6].curr_starting_index=223;
    array[6].offsetofnext_seq_starting_index=704;
    initarr8(array[6].arr,15,47,31,15,31,15,31,47);
    initarr8(array[6].offsetofnext_seq,16,48,32,16,32,16,32,48);

    array[7].curr_starting_index=255;
    array[7].offsetofnext_seq_starting_index=736;
    initarr8(array[7].arr,49,33,17,33,17,33,49,17);
    initarr8(array[7].offsetofnext_seq,48,32,16,32,16,32,48,16);
    // printf("Memory: %llu",(u64)memory);

    while(array[0].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(i=array[0].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[0].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[0].curr_starting_index=array[0].curr_starting_index+array[0].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[0].offsetofnext_seq_starting_index+=480;
    }

    while(array[1].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(i=array[1].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[1].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[1].curr_starting_index=array[1].curr_starting_index+array[1].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[1].offsetofnext_seq_starting_index+=480;
    }

    while(array[2].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(u64 i=array[2].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[2].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[2].curr_starting_index=array[2].curr_starting_index+array[2].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[2].offsetofnext_seq_starting_index+=480;
    }

    while(array[3].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(u64 i=array[3].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[3].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[3].curr_starting_index=array[3].curr_starting_index+array[3].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[3].offsetofnext_seq_starting_index+=480;
    }

    while(array[4].curr_starting_index<=max_index)
    {
    u64 i,j;  
    for(u64 i=array[4].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[4].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[4].curr_starting_index=array[4].curr_starting_index+array[4].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[4].offsetofnext_seq_starting_index+=480;
    }

    while(array[5].curr_starting_index<=max_index)
    {
    u64 i,j;   
    for(u64 i=array[5].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[5].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[5].curr_starting_index=array[5].curr_starting_index+array[5].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[5].offsetofnext_seq_starting_index+=480;
    }   

    while(array[6].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(u64 i=array[6].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[6].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[6].curr_starting_index=array[6].curr_starting_index+array[6].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[6].offsetofnext_seq_starting_index+=480;
    }

    while(array[7].curr_starting_index<=max_index)
    {
    u64 i,j;
    for(u64 i=array[7].curr_starting_index,j=0;(i<cache[1].CacheSize<<3)&(i<=max_index);i+=array[7].arr[j],j++,j&=7)
    {
        setbit(memory,i);
        //printf("i: %llu\n",i);
    }
        array[7].curr_starting_index=array[7].curr_starting_index+array[7].offsetofnext_seq_starting_index-(cache[1].CacheSize<<3);
        array[7].offsetofnext_seq_starting_index+=480;
    }

    printprimes();

}

int main()
{
    static char num2index_arr_max[30] = {6,DUP(6,7),DUP(4,0),DUP(2,1),DUP(4,2),DUP(2,3),DUP(4,4),DUP(6,5),6};
    static char num2index_arr_min[30]={DUP(2,7),DUP(5,8),0,DUP(4,1),DUP(2,2),DUP(4,3),DUP(2,4),DUP(4,5),DUP(6,6)};

    std::cout<<"Enter the number from where you want to start your search: ";
    std::cin>>rangemin;
    std::cout<<"Enter the number from where you want to end your search: ";
    std::cin>>rangemax;

    printf("\nmin_index:%llu, max_index: %llu\n",min_index=(u64)getmin_index(rangemin),max_index=(u64)getmax_index(rangemax));
    printf("min_num: %llu, max_num: %llu\n",indextonum(min_index),(u64)indextonum(max_index));

    START_ANALYZE();
    {
    TIME_BLOCK();
    allocsieve();
    
    //NOTE: Make multithreaded.
    index_dist=max_index-min_index;
    u64 required_num_of_cores_L1=index_dist/(cache[1].CacheSize<<3);
    u64 required_num_of_cores_L2=index_dist/(cache[2].CacheSize<<3);
    u64 required_num_of_cores_L3=index_dist/(cache[3].CacheSize<<3);
    // printf("\nrequired_num_of_cores_L1: %llu, required_num_of_cores_L2: %llu, required_num_of_cores_L3: %llu\n",required_num_of_cores_L1,required_num_of_cores_L2,required_num_of_cores_L3);

    u64 required_num_of_threadsexcludingMainThread=required_num_of_cores_L1<corecount?required_num_of_cores_L1:corecount;
    for(u64 i=1;i<required_num_of_threadsexcludingMainThread;i++,memory=(void*)((u64)(memory)+maxlevel_cachesize+64))
    {
        CreateThread(0,0,(LPTHREAD_START_ROUTINE)threadfunc,memory,0,0);
    }

    threadfunc(memory);

    // printf("\nalignof compositegaps: %zd, &(array[0].offsetofnext_seq[7])=%llu, &(array[1])=%llu\n",alignof(compositegaps),(u64)&(array[0]),(u64)&(array[1]));

    }
    DUMP_DEBUG();

    // u64 temp=1<<4;
    // u64* arr=&temp;
    // printf("%llu",getbit(arr,3));

    // static const u64 p[8] ;
    // static const u64 p[8]={1,2,3,4,5,6,7,8};
    // printf("%llu",p[1]);
}

DEFINE_DEBUG_ARR();

#if 0

#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include"..\..\shared headers\sysinfo.h"
#include"..\..\shared headers\profilerv2.h"

#define getmax_index(n) num2index_arr_max[n%30]+(((n-7)/30)<<3)
#define getmin_index(n) num2index_arr_min[n%30]+(((n-7)/30)<<3)
#define indextonum(n) (30*(n>>3))+index2num_arr[n&7]

void* memory;

static char num2index_arr_max[30] = {6,DUP(6,7),DUP(4,0),DUP(2,1),DUP(4,2),DUP(2,3),DUP(4,4),DUP(6,5),6};
static char num2index_arr_min[30] = {DUP(2,7),DUP(5,8),0,DUP(4,1),DUP(2,2),DUP(4,3),DUP(2,4),DUP(4,5),DUP(6,6)};
static char index2num_arr[8] = {7,11,13,17,19,23,29,31};

DWORD thread_count;
u64 min_index,max_index,rangemin,rangemax,primecount,sieve_size;

struct compositegaps{
    u64 arr[8];
    u64 offsetofnext_seq[8];
    u64 curr_starting_index;
    u64 offsetofnext_seq_starting_index;   
};

void threadfunc(void* sieve_mem)
{
	print("In thread func. sieve_mem: %llu",(u64)sieve_mem);

    compositegaps array[8];

    array[0].curr_starting_index=12;
    array[0].offsetofnext_seq_starting_index=352;
    initarr8(array[0].arr,7,4,7,4,7,12,3,12);
    initarr8(array[0].offsetofnext_seq,32,16,32,16,32,48,16,48);
    
    array[1].curr_starting_index=31;
    array[1].offsetofnext_seq_starting_index=416;
    initarr8(array[1].arr,6,11,6,12,18,5,18,12);
    initarr8(array[1].offsetofnext_seq,16,32,16,32,48,16,48,32);

    array[2].curr_starting_index=44;
    array[2].offsetofnext_seq_starting_index=448;
    initarr8(array[2].arr,13,7,14,21,7,21,14,7);
    initarr8(array[2].offsetofnext_seq,32,16,32,48,16,48,32,16);
    
    array[3].curr_starting_index=76;
    array[3].offsetofnext_seq_starting_index=512;
    initarr8(array[3].arr,9,18,27,9,27,18,9,19);
    initarr8(array[3].offsetofnext_seq,16,32,48,16,48,32,16,32);

    array[4].curr_starting_index=95;
    array[4].offsetofnext_seq_starting_index=544;
    initarr8(array[4].arr,20,30,11,30,20,10,21,10);
    initarr8(array[4].offsetofnext_seq,32,48,16,48,32,16,32,16);

    array[5].curr_starting_index=140;
    array[5].offsetofnext_seq_starting_index=608;
    initarr8(array[5].arr,36,13,36,25,12,25,12,25);
    initarr8(array[5].offsetofnext_seq,48,16,48,32,16,32,16,32);

    array[6].curr_starting_index=223;
    array[6].offsetofnext_seq_starting_index=704;
    initarr8(array[6].arr,15,47,31,15,31,15,31,47);
    initarr8(array[6].offsetofnext_seq,16,48,32,16,32,16,32,48);

    array[7].curr_starting_index=255;
    array[7].offsetofnext_seq_starting_index=736;
    initarr8(array[7].arr,49,33,17,33,17,33,49,17);
    initarr8(array[7].offsetofnext_seq,48,32,16,32,16,32,48,16);

}

int main()
{
    getcacheinfo();
    printf("Enter the number from where you want to start your search: ");
    scanf("%llu",&rangemin);

    printf("Enter the number where you want to end your search: ");
    scanf("%llu",&rangemax);
	START_ANALYZE();
    {
    TIME_BLOCK();
    min_index=(u64)getmin_index(rangemin);max_index=(u64)getmax_index(rangemax);
    print("min_index:%llu, max_index: %llu",min_index,max_index);
    print("min_num: %llu, max_num: %llu",indextonum(min_index),(u64)indextonum(max_index));

    printcacheinfo();

    u64 temp_threadcount=((max_index-min_index+1)/(maxlevel_cachesize<<3));
    thread_count=(DWORD)(corecount<temp_threadcount?corecount:temp_threadcount);
    print("sieve_size: %llu bytes, thread_count: %lu",maxlevel_cachesize,thread_count);

    if(thread_count>1)
    {
        HANDLE* thread_handles;
        
        thread_handles=(HANDLE*)malloc((thread_count-1)*sizeof(HANDLE));
        for(unsigned char i=1;i<thread_count;i++)
        {
            thread_handles[i-1]=CreateThread(0,0,(LPTHREAD_START_ROUTINE)threadfunc,memory,0,0);
            memory=(void*)((u64)(memory)+cache[maxlevel].CacheSize);
        }
        threadfunc(memory);
        WaitForMultipleObjects(thread_count-1,thread_handles,TRUE,INFINITE);
    }
    else threadfunc(memory);
	}
	DUMP_DEBUG();

}
DEFINE_DEBUG_ARR();
#endif