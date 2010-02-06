#Windows compilation?
#make CYGWIN=1
# :)

     CC := gcc
  STRIP := strip
     CP := cp

 SOURCE := wwscan.c
   OBJS := wwscan.o
DISTDIR := ../dist

ifndef CYGWIN
   CFLAGS := -Wtraditional -W -Wall -Wshadow -Wundef
   CFLAGS += -O9 -funroll-loops -fomit-frame-pointer -ffast-math -pipe
   CFLAGS += $(shell if $(CC) -S -march=`uname -m` -o /dev/null -xc /dev/null >/dev/null 2>&1; then echo "-march=`uname -m`" ; fi)
   CFLAGS += $(shell if $(CC) -S -fstrict-aliasing -o /dev/null -xc /dev/null >/dev/null 2>&1; then echo "-fstrict-aliasing" ; fi)
  LDFLAGS := -s  -lmysqlclient -lz -lping
  TARGETS := wwscan
else
   CFLAGS := -Wtraditional -W -Wall -Wshadow -Wundef
   CFLAGS += -O9 -funroll-loops -fomit-frame-pointer -ffast-math -pipe
   CFLAGS += -DCYGWIN
   CFLAGS += $(shell if $(CC) -S -march=`uname -m` -o /dev/null -xc /dev/null >/dev/null 2>&1; then echo "-march=`uname -m`" ; fi)
   CFLAGS += $(shell if $(CC) -S -fstrict-aliasing -o /dev/null -xc /dev/null >/dev/null 2>&1; then echo "-fstrict-aliasing" ; fi)
  LDFLAGS :=  -lmysqlclient -lz -lping
  TARGETS := wwscan.exe
endif

# For glibc 2.x.x memory tracing (leak finder)
ifdef MTRACE
   CFLAGS += -DMTRACE -g
   # mtrace (start) muntrace(returns)
endif

DEPS := $(OBJS:%.o=.deps/%.d)
 FOO := $(shell mkdir .deps >/dev/null 2>&1 || :)

all:
	$(MAKE) all-targets

all-targets: $(TARGETS)


.SUFFIXES: .o .a
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFINES)
	

$(TARGETS): $(OBJS)
	$(CC) -o $@ $< $(LDFLAGS)
	$(STRIP) $@
	$(CP) $@ $(DISTDIR)

clean:
	rm -rf $(TARGETS) *~ .deps mtrace.log *.o $(DISTDIR)/$(TARGETS)

checkout:
	cvs checkout $(TARGETS)

commit:
	cvs commit

update:
	cvs update


-include $(DEPS)

$(DEPS):;

# Enhanced compiling ruleset that also generate dependencies. It's quite
# ugly, like anything borrowed from automake/libtool :)

%.o: %.c
	@echo '$(CC) $(CFLAGS) -c $< -o $@'; \
	$(CC) $(CFLAGS) -Wp,-MD,.deps/$(*F).pp -c $< -o $@
	@-cp .deps/$(*F).pp .deps/$(*F).d; \
	tr ' ' '\012' <.deps/$(*F).pp \
	| sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	>> .deps/$(*F).d; \
	rm .deps/$(*F).pp

.PHONY: clean all all-targets commit update mtrace
