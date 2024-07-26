#ifndef PIPELINE_HEADER
#define PIPELINE_HEADER
#include <EngineCommons.h>
void init_pipeline();
static char* readFile(const char* filepath);
void createGraphicsPipeline(const char* vertFilepath, const char* fragFilePath);

#endif /* ifndef PIPELINE_HEADER */
