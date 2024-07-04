local commands_build = "./build/compile_commands.json"

local commands_local = "compile_commands.json"

local function exits(name)
  local file = io.open(name, "r")
  if file then
    io.close(file)
    return true
  else
    return false
  end
end

if exits(commands_local) then
  os.remove(commands_local)
end

os.rename(commands_build, commands_local)
