# LCTHW Makefile Template

# 定义编译参数和额外链接库,可以在make参数中指定OPTFLAGS和OPTLIBS追加
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
#CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
#C_INCLUDE_PATH=./src/
LIBS=-ldl $(OPTLIBS)

# 安装目标目录,可在make参数中覆盖
PREFIX?=/usr/local

# 全部源代码文件,和它们的编译目标*.o
SOURCES=$(wildcard src/**/*.c src/*.c) # 通配符包含模式
OBJECTS=$(patsubst %.c,%.o,$(SOURCES)) # 替换后缀名,将参数3中的所有文件名从.c替换成.o

# 全部单元测试文件和它们的编译目标,同上,区别在于目标为可执行文件
TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

# 最终链接目标
TARGET=build/liblcthw.a # 静态库
SO_TARGET=$(patsubst %.a,%.so,$(TARGET)) # 动态库

# 第一个目标是未指定目标时的默认目标
all: $(TARGET) $(SO_TARGET) tests # all需求TARGET SO_TARGET中所有文件和tests目标

# 开发版目标
# 如果要临时修改变量,则用第一行写变量修改,第二行写基础目标
dev: CFLAGS=-g -Wall -I src -Wall -Wextra $(OPTFLAGS)
dev: all

# 静态库目标,同上;$@代表当前目标本身
$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS) # 需求build目标和OBJECTS中所有文件
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) --shared -o $@ $(OBJECTS)

# build目标确保以下两个文件夹已被创建
build:
	@mkdir -p build
	@mkdir -p bin

.PHONY: tests # 让make忽略掉与这个目标同名的tests文件夹
#tests: CFLAGS += $(TARGET) # 让单元测试编译时链接上静态库
tests: $(TESTS)
#$(CC) $(CFLAGS) $(TEST_SRC) $(TARGET) -o $(TESTS)
	sh ./tests/runtests.sh

$(TESTS): $(TARGET)
#$(TESTS): $(TEST_SRC) $(TARGET) # 傻逼gcc，非得被依赖方在后面，编译才能通过，只能这样手动指定依赖顺序

# 清理垃圾
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	rm -f *.stackdump
#find . -name "*.gc*" -exec rm {} \;
#rm -rf `find . -name "*.dSYM" -print`

# 安装到系统
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib

# 检查代码中是否存在不安全函数
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
	|stpn?cpy|a?sn?printf|byte_)' $(SOURCE) || true
