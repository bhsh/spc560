# e200z common makefile scripts and rules.

##############################################################################
# Processing options coming from the upper Makefile.
#

# Compiler options
OPT = $(USE_OPT)
COPT = $(USE_COPT)
CPPOPT = $(USE_CPPOPT)

# Garbage collection
ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections -fno-common
  LDOPT := --gc-sections
else
  LDOPT := --no-gc-sections
endif

# Linker extra options
ifneq ($(USE_LDOPT),)
  LDOPT := $(LDOPT),$(USE_LDOPT)
endif

# Link time optimizations
ifeq ($(USE_LTO),yes)
  OPT += -flto
endif

# VLE option handling.
ifeq ($(USE_VLE),yes)
  DDEFS += -DPPC_USE_VLE=1
  DADEFS += -DPPC_USE_VLE=1
  MCU += -mvle
else
  DDEFS += -DPPC_USE_VLE=0
  DADEFS += -DPPC_USE_VLE=0
endif


# Output directory and files
ifeq ($(BUILDDIR),)
  BUILDDIR = build
endif
ifeq ($(BUILDDIR),.)
  BUILDDIR = build
endif
OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).hex \
           $(BUILDDIR)/$(PROJECT).mot $(BUILDDIR)/$(PROJECT).bin \
           $(BUILDDIR)/$(PROJECT).dmp

# Source files groups and paths
SRC	      = $(CSRC)$(CPPSRC)
SRCPATHS  = $(sort $(dir $(ASMXSRC)) $(dir $(ASMSRC)) $(dir $(SRC)))

# Various directories
OBJDIR    = $(BUILDDIR)/obj
LSTDIR    = $(BUILDDIR)/lst

# Object files groups
COBJS     = $(addprefix $(OBJDIR)/, $(notdir $(CSRC:.c=.o)))
CPPOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(CPPSRC:.cpp=.o)))
ASMOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(ASMSRC:.s=.o)))
ASMXOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(ASMXSRC:.S=.o)))
OBJS	  = $(ASMXOBJS) $(ASMOBJS) $(COBJS) $(CPPOBJS)

# Paths
IINCDIR   = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR   = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

# Macros
DEFS      = $(DDEFS) $(UDEFS)
ADEFS 	  = $(DADEFS) $(UADEFS)

# Libs
LIBS      = $(DLIBS) $(ULIBS)

# Various settings
MCFLAGS   = -mcpu=$(MCU)
ODFLAGS	  = -x --syms
ASFLAGS   = $(MCFLAGS) -Wa,-amhls=$(LSTDIR)/$(notdir $(<:.s=.lst)) $(ADEFS)
ASXFLAGS  = $(MCFLAGS) -Wa,-amhls=$(LSTDIR)/$(notdir $(<:.S=.lst)) $(ADEFS)
CFLAGS    = $(MCFLAGS) $(OPT) $(COPT) $(CWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.c=.lst)) $(DEFS)
CPPFLAGS  = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.cpp=.lst)) $(DEFS)
LDFLAGS   = $(MCFLAGS) $(OPT) -nostartfiles $(LLIBDIR) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch,--library-path=$(RULESPATH),$(LDOPT),--script=$(LDSCRIPT)

# Generate dependency information
ASFLAGS  += -MD -MP -MF .dep/$(@F).d
CFLAGS   += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

# Paths where to search for sources
VPATH     = $(SRCPATHS)

#
# Makefile rules
#
#all: $(OBJS) $(OUTFILES) MAKE_ALL_RULE_HOOK
all: $(OBJS) $(OUTFILES)
#all: $(OBJS)

MAKE_ALL_RULE_HOOK:

$(OBJS): | $(BUILDDIR)

$(BUILDDIR) $(OBJDIR) $(LSTDIR):
ifneq ($(USE_VERBOSE_COMPILE),yes)
	@echo Compiler Options
	@echo $(CC) -c $(CFLAGS) -I. $(IINCDIR) main.c -o main.o
#	@echo
endif
	mkdir .\build
	mkdir .\build\obj
	mkdir .\build\lst

$(CPPOBJS) : $(OBJDIR)/%.o : %.cpp Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CPPC) -c $(CPPFLAGS) -I. $(IINCDIR) $< -o $@
endif

$(COBJS) : $(OBJDIR)/%.o : %.c Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CC) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@
endif

$(ASMOBJS) : $(OBJDIR)/%.o : %.s Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
endif

$(ASMXOBJS) : $(OBJDIR)/%.o : %.S Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(ASXFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CC) -c $(ASXFLAGS) -I. $(IINCDIR) $< -o $@
endif

%.elf: $(OBJS) $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
else
	@echo Linking $@
	@$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
endif

%.hex: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(HEX) $< $@
else
	@echo Creating $@
	@$(HEX) $< $@
endif

%.mot: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(MOT) $< $@
else
	@echo Creating $@
	@$(MOT) $< $@
endif

%.bin: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(BIN) $< $@
else
	@echo Creating $@
	@$(BIN) $< $@
endif

%.dmp: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(OD) $(ODFLAGS) $< > $@
else
	@echo Creating $@
	@$(OD) $(ODFLAGS) $< > $@
#	@echo
	@$(SZ) $<
#	@echo
	@echo Done
endif

clean:
	@echo Cleaning
	-rm -fR .dep $(BUILDDIR)
	@echo Done
#	@echo Done

# lint-nt must be accessible from your PATH
LINT_TOOL = lint-nt
LINT_OPTIONS = pclint/co-gcc.lnt pclint/au-misra3.lnt pclint/waivers.lnt

define AWK_CMD =
/search starts/{inc=1; next}/End of search list./{inc=0; next}{if (inc>0) { a = $$1; printf(gensub(/(.*)$$/, "--i\\\\1", "g", a)) }}
endef
define GET_INCLUDE_PATHS_CMD = 
$(shell $(TRGT)cpp -v 2>&1 | gawk '$(AWK_CMD)' > pclint/gcc-include-path.lnt)
endef

misra: pclint/gcc-include-path.lnt pclint/lint_cmac.h pclint/lint_cppmac.h
	@echo
	@echo Start MISRA Check...
	@echo ${LINT_TOOL} ${LINT_OPTIONS} $(subst \,/,$(IINCDIR)) $(subst \,/,$(SRC)) > misra.txt
	${LINT_TOOL} ${LINT_OPTIONS} $(subst \,/,$(IINCDIR)) $(subst \,/,$(SRC)) >> misra.txt
	@echo 
	@echo See results in misra.txt

.PHONY: pclint/gcc-include-path.lnt
pclint/gcc-include-path.lnt:
	@echo
	@echo Generate pclint/gcc-include-path.lnt for current compiler 
	@$(GET_INCLUDE_PATHS_CMD)

.PHONY: pclint/lint_cmac.h
pclint/lint_cmac.h:
	@echo
	@echo Generate pclint/lint_cmac.h for current compiler 
	@touch t.c 
	@$(TRGT)gcc -E -dM t.c > pclint/lint_cmac.h
	@rm t.c

.PHONY: pclint/lint_cppmac.h
pclint/lint_cppmac.h:
	@echo
	@echo Generate pclint/lint_cppmac.h for current compiler
	@touch t.cpp 
	@$(TRGT)g++ -E -dM t.cpp > pclint/lint_cppmac.h
	@rm t.cpp

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>null) $(wildcard .dep/*)

# *** EOF ***
