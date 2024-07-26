#ifndef ERROR_HANDLING
#define ERROR_HANDLING

// Success or return ErrorCode to the next function
typedef enum { SUCCESS, ERROR_INVALID_ARGUMENT, ERROR_VULKAN, ERROR_OUT_MEMORY } ErrorCode;
#define SOR(x)               \
    do {                     \
        ErrorCode err = (x); \
        if (err != SUCCESS)  \
            return err;      \
    } while (0)

#endif // !ERROR_HANDLING
