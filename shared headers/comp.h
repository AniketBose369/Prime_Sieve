#define DUP2(...) __VA_ARGS__,__VA_ARGS__
#define DUP3(...) DUP2(__VA_ARGS__),__VA_ARGS__
#define DUP4(...) DUP2(__VA_ARGS__),DUP2(__VA_ARGS__)
#define DUP5(...) DUP4(__VA_ARGS__),__VA_ARGS__
#define DUP6(...) DUP4(__VA_ARGS__),DUP2(__VA_ARGS__)
#define DUP(count,...) DUP##count(__VA_ARGS__)

#define setbit(arr,n) ((u64*)arr)[n>>6]|=1ULL<<(n&63)
#define getbit(arr,n) ((u64*)arr)[n>>6]&((1ULL<<(n&63)))

#define initarr8(arr,a,b,c,d,e,f,g,h) arr[0]=a,arr[1]=b;arr[2]=c;arr[3]=d;arr[4]=e;arr[5]=f;arr[6]=g;arr[7]=h

#define print(...) {printf("\n");printf(__VA_ARGS__);printf("\n");}

#define checkifpowof2() (i && !(i&(i-1)))

#ifndef min 
#define min(x,y) (x<y)?x:y;
#endif
#ifndef max
#define max(x,y) (x>y)?x:y;
#endif