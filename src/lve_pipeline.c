#include "lve_pipeline.h"
void init_pipeline() {
    createGraphicsPipeline("shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");
}
static char* readFile(const char* filepath) {
    FILE* file;
    errno_t err = fopen_s(&file, filepath, "rb");

    if (err != 0) {
        printf("File error opening file %d\n", err);
    }
    // Get file size
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file); // Go Batck to the start

    // allocate memory for data
    char* data = (char*)malloc(fileSize * sizeof(char));
    if (data == NULL) {
        fclose(file);
        return 0;
    }

    // return buffer data
    fclose(file);
    return data;
}
void createGraphicsPipeline(const char* vertFilepath, const char* fragFilePath) {
    char* vertCode = readFile(vertFilepath);
    char* fragCode = readFile(fragFilePath);

    printf("Vertex Shader code Size: %zu \n", strlen(vertCode));
    printf("Fragment Shader code Size: %zu \n", strlen(fragCode));
}
