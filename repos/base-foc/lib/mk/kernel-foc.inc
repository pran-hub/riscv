FOC_BUILD_DIR  = $(shell pwd)/build
FOC            = $(FOC_BUILD_DIR)/foc
FOC_SRC       := $(call select_from_ports,foc)/src/kernel/foc/kernel/fiasco

KERNEL_BUILD_OUTPUT_FILTER = 2>&1 | sed "s/^/      [fiasco.oc]  /"

MAKEOVERRIDES := $(filter-out KERNEL=%,$(MAKEOVERRIDES))
unexport KERNEL

$(FOC_BUILD_DIR):
	$(VERBOSE_MK) $(MAKE) CROSS_COMPILE="$(CROSS_DEV_PREFIX)" \
	                      $(VERBOSE_DIR) -C $(FOC_SRC) BUILDDIR=$@ \
	                      $(KERNEL_BUILD_OUTPUT_FILTER)
	$(VERBOSE)cp $(KERNEL_CONFIG) $@/globalconfig.out

$(FOC): $(FOC_BUILD_DIR)
	$(VERBOSE_MK) $(MAKE) CROSS_COMPILE="$(CROSS_DEV_PREFIX)" \
	                      CC="$(CC)" CXX="$(CXX)" \
	                      $(VERBOSE_DIR) -C $(FOC_BUILD_DIR) \
	                      $(KERNEL_BUILD_OUTPUT_FILTER)
	$(VERBOSE)touch $@

#
# Sigma0 and bootstrap are provided as L4 packages
#
# We build them within the L4 build directory created by the 'syscall-fiasco'
# library.
#

LIBS += syscall-foc-$(BOARD)

PKGS := l4re-core/crtn \
        l4re-core/uclibc-headers \
        l4re-core/l4util \
        l4re-core/cxx \
        l4re-core/uclibc-minimal \
        l4re-core/libstdc++-headers \
        l4re-core/sigma0 \
        drivers-frst/include drivers-frst/of drivers-frst/uart \
        bootstrap

L4_BUILD_DIR := $(LIB_CACHE_DIR)/syscall-foc-$(BOARD)/build

#
# Supress several warnings especially of the uclibc-minimal
#
WARNINGS += -Wno-cpp -Wno-nonnull-compare -Wno-shift-overflow \
            -Wno-unused-but-set-variable -Wno-maybe-uninitialized \
            -Wno-unused-value -Wno-deprecated-declarations -Wno-attributes

include $(REP_DIR)/lib/mk/l4_pkg.inc

#
# Execute the rules in this file only at the second build stage when we know
# about the complete build settings, e.g., 'CROSS_DEV_PREFIX'.
#
ifeq ($(called_from_lib_mk),yes)
all: $(FOC)
endif
