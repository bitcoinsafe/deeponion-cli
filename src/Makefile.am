AM_CPPFLAGS =  $(INCLUDES) 
AM_LDFLAGS = $(PTHREAD_CFLAGS)

EXTRA_LIBRARIES =

BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS)

bin_PROGRAMS =
TESTS =

if BUILD_BITCOIN_CLI
  bin_PROGRAMS += DeepOnion-cli
endif

.PHONY: FORCE
# DeepOnion CLI core #
BITCOIN_CORE_H = \
  rpcclient.h \
  rpcprotocol.h \
  sync.h \
  uint256.h \
  util.h \
  strlcpy.h \
  version.h
  
JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbitcoin_util_a-version.$(OBJEXT): obj/build.h

# deeponion-cli binary #
DeepOnion_cli_LDADD = \
  $(BOOST_LIBS)
DeepOnion_cli_SOURCES = deeponion-cli.cpp \
			version.cpp
#

if TARGET_WINDOWS
DeepOnion_cli_SOURCES += deeponion-cli-res.rc
endif

CLEANFILES = *.gcda *.gcno $(EXTRA_LIBRARIES)

DISTCLEANFILES = obj/build.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)
