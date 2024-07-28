#ifndef TOOLS
#define TOOLS
#include "ExternalLibs.h"
void infoLog();



struct StringArray{
  uint32_t count;
  // not Constant Because i want to increase the sizel
  char** data;
};

// functions better than arrays for genral and generic stuff because you can test them 
// Should i make it more generic ??? maybe it seems the void pointer was the solution however i dont see how it holds the original type for size increase;

/** Examples
 * strings : const char* strings[] = {"Value","value","value", NULL} 
 * count  : 3
*/
struct StringArray  create_string_array(const char* strings[], uint32_t count);

bool find_in_string_array(char* to_find,struct StringArray arr);
void add_s_to_string_array(struct StringArray* arr, const char* newExtension); 
void free_string_array(struct StringArray* arr);


// i want to pass the size and a voidPointer so its kind of generic
int count_pointer(void* p,unsigned long long value);
#endif // !TOOLS
