#ifndef cppfix
#define cppfix

void * operator new(size_t size);
void operator delete(void * ptr);
void * operator new[](size_t size);
void operator delete[](void * ptr);

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

void * operator new[](size_t size) 
{ 
    return malloc(size); 
} 

void operator delete[](void * ptr) 
{ 
    free(ptr); 
}

#endif
