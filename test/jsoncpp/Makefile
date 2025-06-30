# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS = -ljsoncpp

# 目标文件和源文件
WRITER_SRC = writer_test.cpp
READER_SRC = reader_test.cpp
JSON_UTIL_SRC = json_util.cpp
UTIL_MAIN_SRC = utilmain.cpp  # utilmain.cpp 是使用 json_util 的主程序

WRITER_TARGET = writer_test
READER_TARGET = reader_test
UTIL_MAIN_TARGET = utilmain  # 新可执行文件名称，可按需修改

# 默认目标，编译所有可执行文件
all: $(WRITER_TARGET) $(READER_TARGET) $(UTIL_MAIN_TARGET)

# 生成 writer_test 可执行文件
$(WRITER_TARGET): $(WRITER_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 生成 reader_test 可执行文件
$(READER_TARGET): $(READER_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 生成 utilmain 可执行文件，需链接 json_util.cpp（若 json_util 有实现逻辑）
# 这里假设 json_util.cpp 是工具类实现，utilmain.cpp 调用它，所以一起编译链接
$(UTIL_MAIN_TARGET): $(UTIL_MAIN_SRC) $(JSON_UTIL_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 清理生成文件
clean:
	rm -f $(WRITER_TARGET) $(READER_TARGET) $(UTIL_MAIN_TARGET)

# 运行测试（可按需扩展，比如只运行 utilmain 相关测试等，这里简单示例）
test: $(WRITER_TARGET) $(READER_TARGET) $(UTIL_MAIN_TARGET)
	./$(WRITER_TARGET) && ./$(READER_TARGET) && ./$(UTIL_MAIN_TARGET)

# 声明伪目标
.PHONY: all clean test