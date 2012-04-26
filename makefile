OBJS=loader.o kmain.o cputables.o console.o buildtables.o isrs.o auxhelper.o cxa_atexit.o cxa_finalize.o \
	dynamic_cast.o exception.o guard.o libelftc_dem_gnu3.o memory.o stdexcept.o terminate.o typeinfo.o \
	libc-kernel/abort.o libc-kernel/assert.o
TARGET=lemon-os
IMAGE=floppy.img

CPPFLAGS=-g -Wall -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -m32 \
 -fno-stack-protector -I./include/ -I./include/common/ --sysroot=.
LDFLAGS=-Wall -Werror -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs
NASMFLAGS=-f elf -g -w+orphan-labels -Werror -Wall
LINKSCRIPT=linker.ld

.PHONY : clean all

all : $(TARGET)

#$(IMAGE) : $(TARGET)
#	dd if=/dev/zero of=pad bs=1 count=750
#	cat stage1 stage2 pad $(TARGET) > $(IMAGE)

$(TARGET) : $(OBJS) $(LINKSCRIPT)
	$(CC) -T $(LINKSCRIPT) $(LDFLAGS) -o $(TARGET) $(OBJS)

-include $(OBJS:.o=.d)

%.o : %.asm
	nasm $(NASMFLAGS) $*.asm -o $*.o

%.o : %.cpp
	$(CC) -c $(CPPFLAGS) $*.cpp -o $*.o
	$(CC) -MM -MF $*.d $(CPPFLAGS) $*.cpp
#technique from http://scottmcpeak.com/autodepend/autodepend.html to handle renaming dependencies, copied directly
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -f $(TARGET) *.o *.d
	
mbchk/mbchk : mbchk/mbchk.c mbchk/multiboot.h
	cd mbchk; make all

