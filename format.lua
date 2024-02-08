function traverseDir(path)
	local files = os.files(path .. "/**.cpp")
	for _, file in ipairs(files) do
		os.exec("clang-format --style='file:"..path.."/.clang-format' -i " .. file)
		print("format ", file)
	end
  local files2 = os.files(path .. "/**.hpp")
    for _,file in ipairs(files2) do
    os.exec("clang-format --style='file:"..path..".clang-format' -i " .. file)
    print("format ", file)
    end
end

function main(...)
	print("start format")
	local path = os.getenv("PWD")
	traverseDir(path)
	print("end format")
end
