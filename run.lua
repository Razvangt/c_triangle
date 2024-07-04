local build = "cmake -DCMAKE_BUILD_TYPE=Debug -B build -S ."
local compile = "cmake --build build"
local run = ".\\build\\Debug\\c_triangle.exe"
os.execute(build)
print("compile")
os.execute(compile)
print("run")
os.execute(run)
