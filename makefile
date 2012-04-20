OBJS=loader.o kmain.o
TARGET=sandwich
IMAGE=floppy.img

CPPFLAGS=-g -Wall -Werror -std=gnu++0x -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -m32 \
-fno-exceptions -fno-rtti -fno-stack-protector
LDFLAGS=-Wall -Werror -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs
NASMFLAGS=-f elf
LINKSCRIPT=linker.ld

.PHONY : clean all

all : $(TARGET)

#$(IMAGE) : $(TARGET)
#	dd if=/dev/zero of=pad bs=1 count=750
#	cat stage1 stage2 pad $(TARGET) > $(IMAGE)

$(TARGET) : $(OBJS) $(LINKSCRIPT)
	$(CC) -T $(LINKSCRIPT) $(LDFLAGS) -o $(TARGET) $(OBJS)

-include $(OBJS:.o=.d)

%.o : %.s
	nasm $(NASMFLAGS) $*.s -o $*.o

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

