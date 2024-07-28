#include "tools.h"
void infoLog() {
    uint32_t instanceApiVersion;

    VkResult result = vkEnumerateInstanceVersion(&instanceApiVersion);
    if (result != VK_SUCCESS) {
        printf(" Couldn't enumerate instance version \n");
        return;
    }

    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor   = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor   = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch   = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}

struct StringArray  create_string_array(const char* strings[], uint32_t count){
  // so allocate count size on datta
  struct StringArray array = {
    .count = count,
    .data = (char**)malloc(count * sizeof(const char*)),
  };
  for (size_t i = 0; i < count; ++i){
    array.data[i] = _strdup(strings[i]);
  }
  return array;
}

void add_s_to_string_array(struct StringArray* arr, const char* newExtension) {
    char** newExtensions = (char**)malloc((arr -> count + 1) * sizeof(char*));

    for (uint32_t i = 0; i < arr ->count; ++i) {
        newExtensions[i] = _strdup(arr -> data[i]); // Copy strings to avoid modifying originals
    }
    newExtensions[arr -> count] = _strdup(newExtension);
    arr -> data = newExtensions;
    arr -> count = arr->count+1;
}

bool find_in_string_array(char* to_find,struct StringArray arr){

};
void free_string_array(struct StringArray* arr){
    // Free the allocated memory
    for (uint32_t i = 0; i <= arr -> count; ++i) {
        free((void *)arr -> data[i]);
    }
    free(arr);
};



//TODO: TEST THIS ??? for GODS SAKE
// i kind of trust geminy did a good copy with this one and i have coplete faith because i dont want to know what the fuck is this ...  
// i want to pass the size and a voidPointer so its kind of generic
int count_pointer(void* p,unsigned long long value){
    unsigned char* byte_ptr = (unsigned char*)p;
    unsigned long long int_value = *(unsigned long long*)p; // Assuming value is the same size as unsigned long long
    int count = 0;

    // Handle different data types based on the size of value
    if (sizeof(value) == sizeof(unsigned char)) {
        for (unsigned char* ptr = byte_ptr; ptr < byte_ptr + sizeof(int_value); ptr++) {
            if (*ptr == (unsigned char)value) {
                count++;
            }
        }
    } else if (sizeof(value) == sizeof(unsigned short)) {
        // Handle unsigned short case
    } else if (sizeof(value) == sizeof(unsigned int)) {
        // Handle unsigned int case
    } else if (sizeof(value) == sizeof(unsigned long long)) {
        for (unsigned long long* ptr = (unsigned long long*)byte_ptr; ptr < (unsigned long long*)(byte_ptr + sizeof(int_value)); ptr++) {
            if (*ptr == value) {
                count++;
            }
        }
    } else {
        // Handle other data types or error
        return -1;
    }
    return count;
}


