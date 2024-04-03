function traverseDir(path)
	local files = os.files(path .. "/**.cpp")
	for _, file in ipairs(files) do
		os.exec("clang-format --style='file:"..os.getenv("PWD").."/.clang-format' -i " .. file)
		print("format ", file)
	end
  local files2 = os.files(path .. "/**.hpp")
    for _,file in ipairs(files2) do
    os.exec("clang-format --style='file:"..os.getenv("PWD").."/.clang-format' -i " .. file)
    print("format ", file)
    end
end

function main(...)
	print("start format")
	local path = os.getenv("PWD")
	traverseDir(path.."/core")
	traverseDir(path.."/android")
	traverseDir(path.."/linux")
	traverseDir(path.."/test")
	traverseDir(path.."/example")
	traverseDir(path.."/desktop")
	print("end format")
end
