default:all

all:tips lib

tips:
	-@echo  "\033[32m Building liblog ... \033[0m"

ifndef TOP_DIR
TOP_DIR = $(shell pwd)/../../
endif

V:=0
include env.mk

INC+= 

LD:= -L./lib -Wl,-Bstatic -llog -Wl,-Bdynamic 

CFLAGS+=$(INC)
LDFLAGS+=$(LD)

lib:log.o 
	-@mkdir -p lib
	$(Q)$(AR) rucs lib/liblog.a $^ 
	@$(E) "  AR " $<
	$(Q)$(CC) -shared -fPIC -o lib/liblog.so $^
	@$(E) "  Shared " $<

%.o:%.c
	$(Q)$(CC) -g -c $(CFLAGS) $< -o $@
	@$(E) "  CC " $<

test_main:test_main.o lib   
	$(Q)$(CC)   $<  $(LD) -o  $@
	@$(E) "  GEN " $<

clean:
	-@$(RM) *.o *.a lib/*.a lib/*.so

install:
	-@install -v lib/*.a $(DST_DIR)
	-@install -v lib/*.so $(DST_DIR)

.PHONY:clean
