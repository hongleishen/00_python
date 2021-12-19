################# select platform #################
ifndef TARGET_BOARD
#TARGET_BOARD = 2280_sram
TARGET_BOARD = 1680_fpga
#TARGET_BOARD = 2280_ddr
endif

##################mem (512M) layout###########################
CONFIG_ENTRY_ADDR = 0x80000000
CONFIG_SP_START_ADDR = 0x82000000
CONFIG_DMA_START_ADDR = $(CONFIG_SP_START_ADDR)
CONFIG_INT_VECTOR_ADDR = 0x9FF00000
###############mem layout end##########################

################# Configuration #################
##CROSS_COMPILE = arm-none-eabi-
CROSS_COMPILE = arm-hjimi-linux-gnueabi-

ifndef DDR_WIDTH
DDR_WIDTH = 64
endif

DEBUG_OPEN = 0

ifeq ($(TARGET_BOARD), 1680_fpga)
	MACRO_IN_CODE   += -DSYNPSYS_DPHY_TESTCHIP=0
	MACRO_IN_CODE   += -DVERISLC_DPHY_TESTCHIP=1
	MACRO_IN_CODE   += -DVERISLC_DPHY_SIMULAT=0
	MACRO_IN_CODE   += -DCHIP_IMI_1680
	SELECT_PLATFORM += -DTARGET_FPGA=1
	ASM_PLATFORM    += -DTARGET_FPGA=1
endif

ifeq ($(TARGET_BOARD), 2280_ddr)
	MACRO_IN_CODE   += -DSYNPSYS_DPHY_TESTCHIP=1
	MACRO_IN_CODE   += -DVERISLC_DPHY_TESTCHIP=0
	MACRO_IN_CODE   += -DVERISLC_DPHY_SIMULAT=0
	MACRO_IN_CODE   += -DCHIP_IMI_2280
	SELECT_PLATFORM += -DTARGET_DDR=1
	ASM_PLATFORM    += -DTARGET_DDR=1
endif

################### Build Tool ##################
CC	:= ${CROSS_PREFIX}gcc
CXX	:= ${CC} -E
LD	:= ${CROSS_PREFIX}ld
CP	:= ${CROSS_PREFIX}objcopy
OD	:= ${CROSS_PREFIX}objdump

#################### variate ####################
srctree  = .
dirs-y  :=
dirs-n  :=
objs-y  :=
ifeq ($(DDR_WIDTH), 64)
libs-y  := $(srctree)/drivers/ddr/libddr_64.a
else
libs-y  := $(srctree)/drivers/ddr/libddr_32.a
endif

BOARD_CONFIG := $(wildcard $(srctree)/boards/$(TARGET_BOARD)/*_defconfig)
-include $(BOARD_CONFIG)

ifneq ($(O),)
BUILD_OUTPUT = $(O)
else
BUILD_OUTPUT = .
endif

BUILD_TARGET = imiTest_$(TARGET_BOARD)
FINAL_TARGET = $(srctree)/$(BUILD_OUTPUT)/$(BUILD_TARGET)

LDSCRIPT	= $(shell find ./ -name '*.lds')
LIBDIR		=

GLOBAL_CFLAGS ?= $(ALL_INCLUDES)
GLOBAL_CFLAGS += -Wall  -Wstrict-prototypes -O0 -g  -Werror
GLOBAL_CFLAGS += -c -mlittle-endian -march=armv5te
GLOBAL_CFLAGS += -DCONFIG_ENTRY_ADDR=$(CONFIG_ENTRY_ADDR) -DCONFIG_SP_START_ADDR=$(CONFIG_SP_START_ADDR) -DCONFIG_DMA_START_ADDR=$(CONFIG_DMA_START_ADDR) -DCONFIG_INT_VECTOR_ADDR=$(CONFIG_INT_VECTOR_ADDR)
GLOBAL_CFLAGS += $(SELECT_PLATFORM)
GLOBAL_CFLAGS += $(ASM_PLATFORM)
GLOBAL_CFLAGS += $(MACRO_IN_CODE)
GLOBAL_CFLAGS += -DCPUDIR=$(CPUDIR)
#GLOBAL_CFLAGS += -DQSPI_INST_TYPE_QUAD_MODE
GLOBAL_CFLAGS += -DQSPI_USE_DMA
ifeq ($(DEBUG_OPEN), 1)
       GLOBAL_CFLAGS += -DDEBUG_OPEN=1
endif

LDFLAGS = -march=armv5te -nostartfiles -pie -Bstatic -T $(LDSCRIPT) \
	$(ALL_LIBS) -Ttext ${CONFIG_ENTRY_ADDR} \
	-Wl,-Map=$(FINAL_TARGET).map,--cref,--no-warn-mismatch

#################### Build files ####################
DIR_SRCS_C    = $(if $(dirs-y), $(foreach dir, $(wildcard $(dirs-y)), $(shell find $(dir) -name '*.c')))
DIR_SRCS_S    = $(if $(dirs-y), $(foreach dir, $(wildcard $(dirs-y)), $(shell find $(dir) -name '*.S')))
DIR_SRCS_S   += $(if $(dirs-y), $(foreach dir, $(wildcard $(dirs-y)), $(shell find $(dir) -name '*.s')))
DIR_SRCS_CPP  = $(if $(dirs-y), $(foreach dir, $(wildcard $(dirs-y)), $(shell find $(dir) -name '*.cpp')))
DIR_RMVS_C    = $(if $(dirs-n), $(foreach dir, $(wildcard $(dirs-n)), $(shell find $(dir) -name '*.c')))
DIR_RMVS_S    = $(if $(dirs-n), $(foreach dir, $(wildcard $(dirs-n)), $(shell find $(dir) -name '*.S')))
DIR_RMVS_S   += $(if $(dirs-n), $(foreach dir, $(wildcard $(dirs-n)), $(shell find $(dir) -name '*.s')))
DIR_RMVS_CPP  = $(if $(dirs-n), $(foreach dir, $(wildcard $(dirs-n)), $(shell find $(dir) -name '*.cpp')))

ALL_SRCS_C    = $(filter-out $(DIR_RMVS_C), $(DIR_SRCS_C))
ALL_SRCS_C   += $(filter %.c, $(objs-y))
ALL_SRCS_CPP  = $(filter-out $(DIR_RMVS_CPP), $(DIR_SRCS_CPP))
ALL_SRCS_CPP += $(filter %.cpp, $(objs-y))
ALL_SRCS_S    = $(filter-out $(DIR_RMVS_S), $(DIR_SRCS_S))
ALL_SRCS_S   += $(filter %.S %.s, $(objs-y))
ALL_LIBS     := $(libs-y)

ifeq ($(O),)

ALL_OBJECTS  = $(ALL_SRCS_C:.c=.o)
vpath %.c $(sort $(dir $(ALL_SRCS_C)))
ALL_OBJECTS += $(ALL_SRCS_CPP:.cpp=.o)
vpath %.cpp $(sort $(dir $(ALL_SRCS_CPP)))
ALL_OBJECTS += $(ALL_SRCS_S:.S=.o)
vpath %.S $(sort $(dir $(ALL_SRCS_S)))
CPUDIR = boards/$(TARGET_BOARD)/start
else

ALL_OBJECTS  = $(addprefix $(BUILD_OUTPUT)/, $(ALL_SRCS_C:.c=.o))
vpath %.c $(sort $(dir $(ALL_SRCS_C)))
ALL_OBJECTS += $(addprefix $(BUILD_OUTPUT)/, $(ALL_SRCS_CPP:.cpp=.o))
vpath %.cpp $(sort $(dir $(ALL_SRCS_CPP)))
ALL_OBJECTS += $(addprefix $(BUILD_OUTPUT)/, $(ALL_SRCS_S:.S=.o))
vpath %.S $(sort $(dir $(ALL_SRCS_S)))
OUTPUT_DIR   = $(addprefix $(BUILD_OUTPUT)/, $(dir $(ALL_SRCS_C)))
CPUDIR = $(BUILD_OUTPUT)/boards/$(TARGET_BOARD)/start
endif

#################### Targets ####################
all: version imi_test.lds $(FINAL_TARGET).axf $(FINAL_TARGET).bin $(FINAL_TARGET).ds

	@echo "###############################################################################"
	@echo "##                                                                           ##"
	@echo "##                    successfully Build! (Little Endian)                    ##"
	@echo "##                                                                           ##"
	@echo "###############################################################################"

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	else echo sh; fi ; fi)

version:
	$(shell if [ -d $(srctree)/include/autogen ]; then rm -rf $(srctree)/include/autogen; fi)
	mkdir -p include/autogen
	@echo "#define LOCAL_VER \"$$($(CONFIG_SHELL) $(srctree)/script/setlocalversion $(srctree))\"" > include/autogen/local_version.h

imi_test.lds: boards/imi_test.lds.S
	$(CROSS_COMPILE)gcc -E -nostdinc $(GLOBAL_CFLAGS) -I$(ALL_INCLUDES) -D__ASSEMBLY__ -x assembler-with-cpp -P -o imi_test.lds boards/imi_test.lds.S

$(BUILD_OUTPUT)/%.o: %.c mk_outpu_dir
	$(CROSS_COMPILE)gcc $(GLOBAL_CFLAGS) $< -o $@

$(BUILD_OUTPUT)/%.o: %.cpp mk_outpu_dir
	$(CROSS_COMPILE)g++ $(GLOBAL_CFLAGS) $< -o $@

$(BUILD_OUTPUT)/%.o: %.S mk_outpu_dir
	$(CROSS_COMPILE)gcc -x assembler-with-cpp -D__ASSEMBLY__ $(GLOBAL_CFLAGS) $< -o $@

$(BUILD_OUTPUT)/%.o: %.s mk_outpu_dir
	$(CROSS_COMPILE)gcc -x assembler-with-cpp -D__ASSEMBLY__ $(GLOBAL_CFLAGS) $< -o $@

$(FINAL_TARGET).axf: $(ALL_OBJECTS)
	$(CROSS_COMPILE)gcc $(ALL_OBJECTS) $(LDFLAGS) -o $@

$(FINAL_TARGET).bin: $(FINAL_TARGET).axf | $(BUILD_OUTPUT)
	$(CROSS_COMPILE)objcopy -O binary $< $@
ifneq ($(O),)
	cp $@ ./$(BUILD_TARGET).bin
endif

$(FINAL_TARGET).ds: $(FINAL_TARGET).axf | $(BUILD_OUTPUT)
	$(CROSS_COMPILE)objdump -d    $< > $@

mk_outpu_dir:
ifneq ($(O),)
	echo $(BUILD_OUTPUT)
	echo $(OUTPUT_DIR)
	mkdir -p $(BUILD_OUTPUT)
	mkdir -p $(OUTPUT_DIR)
endif


debug: test
test: test_start all_c all_s all_lib test_end
config: test_start test_config test_end
allc: test_start all_c test_end
alls: test_start all_s test_end
allib: test_start all_lib test_end
test_start:
	@echo "###############################################################################"
test_config:
	@echo "-------------------------------------------------------------------------------"
	@echo "objs-y : "
	@$(if $(objs-y), \
		$(foreach obj, $(objs-y), printf "\\t %s \\n" $(obj);) \
		echo " ")
	@echo "-------------------------------------------------------------------------------"
	@echo "dirs-y : "
	@$(if $(dirs-y), \
		$(foreach dir, $(dirs-y), printf "\\t %s " $(dir);) \
		echo " ")
	@echo "-------------------------------------------------------------------------------"
	@echo "libs-y : "
	@$(if $(libs-y), \
		$(foreach lib, $(libs-y), printf "\\t %s \\n" $(lib);) \
		echo " ")
all_c:
	@echo "-------------------------------------------------------------------------------"
	@echo "ALL C CODE FILES : "
	@$(if $(ALL_SRCS_C), \
		$(foreach src, $(ALL_SRCS_C), printf "\\t %s \\n" $(src);) \
		echo " ")
all_s:
	@echo "-------------------------------------------------------------------------------"
	@echo "ALL S CODE FILES : "
	@$(if $(ALL_SRCS_S), \
		$(foreach src, $(ALL_SRCS_S), printf "\\t %s \\n" $(src);) \
		echo " ")
all_lib:
	@echo "-------------------------------------------------------------------------------"
	@echo "ALL LBRARY FILES : "
	@$(if $(ALL_LIBS), \
		$(foreach lib, $(ALL_LIBS), printf "\\t %s \\n" $(lib);) \
		echo " ")
all_obj:
	@echo "-------------------------------------------------------------------------------"
	@echo "ALL OUTPUT FILES : "
	@$(if $(ALL_OBJECTS), \
		$(foreach obj, $(ALL_OBJECTS), printf "\\t %s \\n" $(obj);) \
		echo " ")
test_end:
	@echo "-------------------------------------------------------------------------------"
	@echo "###############################################################################"


RCS_FIND_IGNORE := \( -name .svn -o  -name .git \) -prune -o
clean:
	@echo "Clean output dir ..."
	rm -rf $(srctree)/include/autogen
	@echo "Clean obj files ..."
	@find . $(RCS_FIND_IGNORE) \
		\( -name '*.[od]'  \
		-o -name '*.bak'   \
		-o -name '*.lst'   \
		-o -name '*.su'    \
		-o -name '.*.tmp'  \
		-o -name '*.map'   \
		-o -name '*.axf'   \
		-o -name '*.bin'   \
		-o -name '*.ds'    \
		-o -name '*.lds'   \
		-o -name 'output'  \) -print | xargs rm -rf

