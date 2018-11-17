# These definitions are suitable for efivar and libsmbios_c,
# since they only use -I and -l.
CFLAGS += $(shell pkg-config --cflags libsmbios_c efivar)
LDLIBS += $(shell pkg-config --libs   libsmbios_c efivar)

.PHONY: all clean

all: force_dell_tbt

clean:
	$(RM) force_dell_tbt
