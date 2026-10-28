#include<windows.h>
#include"..\..\shared headers\mytypes.h"
#include"..\..\shared headers\comp.h"
#ifdef PROF
/*  This piece of code can run in two diff terminals. Maybe for later use
    HWND current_console_window_handle=GetConsoleWindow();
    DWORD current_process_pid;
    GetWindowThreadProcessId(current_console_window_handle,&current_process_pid);
    printf("In main console\n");
    getch();
    FreeConsole();
    AllocConsole();
    printf("In next console\n");
    getch();
    FreeConsole();
    AttachConsole(current_process_pid);
    printf("In main console\n");    
    getch();
*/
    #define START_ANALYZE() QueryPerformanceFrequency((LARGE_INTEGER*)&freq)
    #define TIME_BLOCK__(line) time_block timeblock_##line(__COUNTER__,__FILE__,__LINE__,__FUNCTION__)
    #define TIME_BLOCK_(line) TIME_BLOCK__(line)
    #define TIME_BLOCK() TIME_BLOCK_(__LINE__)
    extern u64 debug_count;
    #define DEFINE_DEBUG_ARR() debug_record debug_record_arr[__COUNTER__+1];\
    u64 debug_count=__COUNTER__-1
    u64 freq;
    #define DUMP_DEBUG() {\
        for(int i=0;i<debug_count;i++)\
            if(debug_record_arr[i].func_name!=0)\
                print("Block at linenum %u in file %s in func: %s took %llu us.",debug_record_arr[i].line_num,debug_record_arr[i].file_name,debug_record_arr[i].func_name,(1000000*debug_record_arr[i].cycles)/freq);\
    }
    struct debug_record
    {
        u64 cycles;
        char* file_name;
        char* func_name;
        int line_num;
        int pad;
    };
    extern debug_record debug_record_arr[];   
    struct time_block
    {   
        debug_record *Record;
        u64 startcycle;
        time_block(uint counter,char* filename,uint linenum,char* funcname)
        {
            Record = debug_record_arr + counter;
            Record->file_name=filename;
            Record->line_num=linenum;
            Record->func_name=funcname;
            QueryPerformanceCounter((LARGE_INTEGER*)(&startcycle));
            //startcycle=__rdtsc();
        }
        ~time_block()
        {
            u64 temp;
            QueryPerformanceCounter((LARGE_INTEGER*)(&temp));
            _InterlockedExchangeAdd64((LONG64 *)(&Record->cycles),(temp-startcycle));
            //Record->cycles+=(temp-startcycle);
            //Record->cycles+=(__rdtsc()-startcycle);
        }
    }; 

#else

#define START_ANALYZE() 
#define TIME_BLOCK()
#define DEFINE_DEBUG_ARR()
#define DUMP_DEBUG()

#endif
