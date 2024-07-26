-- & "C:\\VulkanSDK\\1.3.275.0\\Bin\\glslc.exe"
local binary = '"C:\\VulkanSDK\\1.3.275.0\\Bin\\glslc.exe"'
local vert = " shaders\\simple_shader.vert -o shaders\\simple_shader.vert.spv"
local frag = " shaders\\simple_shader.frag -o shaders\\simple_shader.frag.spv"

print(binary .. vert)
os.execute(binary .. vert)
print(binary .. frag)
os.execute(binary .. frag)
