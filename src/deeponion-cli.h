// Copyright (c) 20018 The DeepOnion developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef DEEPONION_CLI_H
#define DEEPONION_CLI_H

#include "uint256.h"
#include "version.h"
#include "sync.h"
#include "strlcpy.h"
#include <boost/signals2/last_value.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/foreach.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>
#include <stdint.h>
#include <stdarg.h>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/shared_ptr.hpp>
#include "json/json_spirit_writer_template.h"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace json_spirit;

#ifdef WIN32
#ifdef _MSC_VER
#pragma warning(disable : 4786)
#pragma warning(disable : 4804)
#pragma warning(disable : 4805)
#pragma warning(disable : 4717)
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501
#ifdef _WIN32_IE
#undef _WIN32_IE
#endif
#define _WIN32_IE 0x0501
#define WIN32_LEAN_AND_MEAN 1
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h> /* for _commit */
#include "shlobj.h"
#elif defined(__linux__)
#include <sys/prctl.h>
#endif

#ifndef WIN32
#include <execinfo.h>
#endif

using namespace std;

#define PAIRTYPE(t1, t2) std::pair<t1, t2>
static const unsigned int MAX_SIZE = 0x02000000;
extern bool fTestNet;
extern bool fPrintToDebugger;
extern std::string strMiscWarning;
extern bool fPrintToConsole;
extern bool fLogTimestamps;
/** Wrapped boost mutex: supports recursive locking, but no waiting  */
typedef boost::recursive_mutex CCriticalSection;
std::map<std::string, std::string> mapArgs;
std::map<std::string, std::vector<std::string> > mapMultiArgs;
bool fReopenDebugLog = false;
/* This GNU C extension enables the compiler to check the format string against the parameters provided.
 * X is the number of the "format string" parameter, and Y is the number of the first variadic parameter.
 * Parameters count from 1.
 */
#ifdef __GNUC__
#define ATTR_WARN_PRINTF(X, Y) __attribute__((format(printf, X, Y)))
#else
#define ATTR_WARN_PRINTF(X, Y)
#endif

// forwarder

int ATTR_WARN_PRINTF(1, 2) OutputDebugStringF(const char *pszFormat, ...);

#define printf OutputDebugStringF

void LogException(std::exception *pex, const char *pszThread);
void PrintException(std::exception *pex, const char *pszThread);
void PrintExceptionContinue(std::exception *pex, const char *pszThread);
void ParseString(const std::string &str, char c, std::vector<std::string> &v);
std::string FormatMoney(int64_t n, bool fPlus = false);
bool ParseMoney(const std::string &str, int64_t &nRet);
bool ParseMoney(const char *pszIn, int64_t &nRet);
std::vector<unsigned char> ParseHex(const char *psz);
std::vector<unsigned char> ParseHex(const std::string &str);
bool IsHex(const std::string &str);
std::vector<unsigned char> DecodeBase64(const char *p, bool *pfInvalid = NULL);
std::string DecodeBase64(const std::string &str);
std::string EncodeBase64(const unsigned char *pch, size_t len);
std::string EncodeBase64(const std::string &str);
std::vector<unsigned char> DecodeBase32(const char *p, bool *pfInvalid = NULL);
std::string DecodeBase32(const std::string &str);
std::string EncodeBase32(const unsigned char *pch, size_t len);
std::string EncodeBase32(const std::string &str);
void ParseParameters(int argc, const char *const argv[]);
bool WildcardMatch(const char *psz, const char *mask);
bool WildcardMatch(const std::string &str, const std::string &mask);
void FileCommit(FILE *fileout);
bool RenameOver(boost::filesystem::path src, boost::filesystem::path dest);
boost::filesystem::path GetDefaultDataDir();
const boost::filesystem::path &GetDataDir(bool fNetSpecific = true);
boost::filesystem::path GetConfigFile();
boost::filesystem::path GetPidFile();
#ifndef WIN32
void CreatePidFile(const boost::filesystem::path &path, pid_t pid);
#endif
void ReadConfigFile(std::map<std::string, std::string> &mapSettingsRet, std::map<std::string, std::vector<std::string> > &mapMultiSettingsRet);
#ifdef WIN32
boost::filesystem::path GetSpecialFolderPath(int nFolder, bool fCreate = true);
#endif
void ShrinkDebugFile();
int GetRandInt(int nMax);
uint64_t GetRand(uint64_t nMax);
uint256 GetRandHash();
int64_t GetTime();
void SetMockTime(int64_t nMockTimeIn);
int64_t GetAdjustedTime();
int64_t GetTimeOffset();
long hex2long(const char *hexString);
// std::string FormatFullVersion();
std::string FormatSubVersion(const std::string &name, int nClientVersion, const std::vector<std::string> &comments);
void runCommand(std::string strCommand);

//
// "Never go to sea with two chronometers; take one or three."
// Our three time sources are:
//  - System clock
//  - Median of other nodes clocks
//  - The user (asking the user to fix the system clock if the first two disagree)
//
static int64_t nMockTime = 0; // For unit testing

int64_t GetTime()
{
  if (nMockTime)
    return nMockTime;

  return time(NULL);
}

inline int64_t atoi64(const char *psz)
{
#ifdef _MSC_VER
  return _atoi64(psz);
#else
  return strtoll(psz, NULL, 10);
#endif
}

inline int64_t atoi64(const std::string &str)
{
#ifdef _MSC_VER
  return _atoi64(str.c_str());
#else
  return strtoll(str.c_str(), NULL, 10);
#endif
}

inline int atoi(const std::string &str)
{
  return atoi(str.c_str());
}

inline int roundint(double d)
{
  return (int)(d > 0 ? d + 0.5 : d - 0.5);
}

inline int64_t roundint64(double d)
{
  return (int64_t)(d > 0 ? d + 0.5 : d - 0.5);
}

inline int64_t abs64(int64_t n)
{
  return (n >= 0 ? n : -n);
}

inline std::string leftTrim(std::string src, char chr)
{
  std::string::size_type pos = src.find_first_not_of(chr, 0);

  if (pos > 0)
    src.erase(0, pos);

  return src;
}

template <typename T>
std::string HexStr(const T itbegin, const T itend, bool fSpaces = false)
{
  std::string rv;
  static const char hexmap[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  rv.reserve((itend - itbegin) * 3);
  for (T it = itbegin; it < itend; ++it)
  {
    unsigned char val = (unsigned char)(*it);
    if (fSpaces && it != itbegin)
      rv.push_back(' ');
    rv.push_back(hexmap[val >> 4]);
    rv.push_back(hexmap[val & 15]);
  }

  return rv;
}

inline std::string HexStr(const std::vector<unsigned char> &vch, bool fSpaces = false)
{
  return HexStr(vch.begin(), vch.end(), fSpaces);
}

template <typename T>
void PrintHex(const T pbegin, const T pend, const char *pszFormat = "%s", bool fSpaces = true)
{
  printf(pszFormat, HexStr(pbegin, pend, fSpaces).c_str());
}

inline void PrintHex(const std::vector<unsigned char> &vch, const char *pszFormat = "%s", bool fSpaces = true)
{
  printf(pszFormat, HexStr(vch, fSpaces).c_str());
}

inline int64_t GetPerformanceCounter()
{
  int64_t nCounter = 0;
#ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER *)&nCounter);
#else
  timeval t;
  gettimeofday(&t, NULL);
  nCounter = (int64_t)t.tv_sec * 1000000 + t.tv_usec;
#endif
  return nCounter;
}

inline int64_t GetTimeMillis()
{
  return (boost::posix_time::ptime(boost::posix_time::microsec_clock::universal_time()) -
          boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1)))
      .total_milliseconds();
}

inline std::string DateTimeStrFormat(const char *pszFormat, int64_t nTime)
{
  time_t n = nTime;
  struct tm *ptmTime = gmtime(&n);
  char pszTime[200];
  strftime(pszTime, sizeof(pszTime), pszFormat, ptmTime);
  return pszTime;
}

static const std::string strTimestampFormat = "%Y-%m-%d %H:%M:%S UTC";
inline std::string DateTimeStrFormat(int64_t nTime)
{
  return DateTimeStrFormat(strTimestampFormat.c_str(), nTime);
}

template <typename T>
void skipspaces(T &it)
{
  while (isspace(*it))
    ++it;
}

inline bool IsSwitchChar(char c)
{
#ifdef WIN32
  return c == '-' || c == '/';
#else
  return c == '-';
#endif
}

/**
 * Return string argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (e.g. "1")
 * @return command-line argument or default value
 */
std::string GetArg(const std::string &strArg, const std::string &strDefault);

/**
 * Return integer argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (e.g. 1)
 * @return command-line argument (0 if invalid number) or default value
 */
int64_t GetArg(const std::string &strArg, int64_t nDefault);

/**
 * Return boolean argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param default (true or false)
 * @return command-line argument or default value
 */
bool GetBoolArg(const std::string &strArg, bool fDefault = false);

/**
 * Set an argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param strValue Value (e.g. "1")
 * @return true if argument gets set, false if it already had a value
 */
bool SoftSetArg(const std::string &strArg, const std::string &strValue);

/**
 * Set a boolean argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param fValue Value (e.g. false)
 * @return true if argument gets set, false if it already had a value
 */
bool SoftSetBoolArg(const std::string &strArg, bool fValue);

//*******************************************************************

std::string FormatFullVersion()
{
  return CLIENT_BUILD;
}

#ifdef WIN32
boost::filesystem::path GetSpecialFolderPath(int nFolder, bool fCreate)
{
  namespace fs = boost::filesystem;

  char pszPath[MAX_PATH] = "";

  if (SHGetSpecialFolderPathA(NULL, pszPath, nFolder, fCreate))
  {
    return fs::path(pszPath);
  }

  printf("SHGetSpecialFolderPathA() failed, could not obtain requested path.\n");
  return fs::path("");
}
#endif

boost::filesystem::path GetDefaultDataDir()
{
  namespace fs = boost::filesystem;
  // Windows < Vista: C:\Documents and Settings\Username\Application Data\DeepOnion
  // Windows >= Vista: C:\Users\Username\AppData\Roaming\DeepOnion
  // Mac: ~/Library/Application Support/DeepOnion
  // Unix: ~/.DeepOnion
#ifdef WIN32
  // Windows
  return GetSpecialFolderPath(CSIDL_APPDATA) / "DeepOnion";
#else
  fs::path pathRet;
  char *pszHome = getenv("HOME");
  if (pszHome == NULL || strlen(pszHome) == 0)
    pathRet = fs::path("/");
  else
    pathRet = fs::path(pszHome);
#ifdef MAC_OSX
  // Mac
  pathRet /= "Library/Application Support";
  fs::create_directory(pathRet);
  return pathRet / "DeepOnion";
#else
  // Unix
  return pathRet / ".DeepOnion";
#endif
#endif
}

const boost::filesystem::path &GetDataDir(bool fNetSpecific)
{
  namespace fs = boost::filesystem;

  static fs::path pathCached[2];
  static CCriticalSection csPathCached;
  static bool cachedPath[2] = {false, false};

  fs::path &path = pathCached[fNetSpecific];

  // This can be called during exceptions by printf, so we cache the
  // value so we don't have to do memory allocations after that.
  if (cachedPath[fNetSpecific])
    return path;

  LOCK(csPathCached);

  if (mapArgs.count("-datadir"))
  {
    path = fs::system_complete(mapArgs["-datadir"]);
    if (!fs::is_directory(path))
    {
      path = "";
      return path;
    }
  }
  else
  {
    path = GetDefaultDataDir();
  }
  if (fNetSpecific && fTestNet)
    path /= "testnet";

  fs::create_directory(path);

  cachedPath[fNetSpecific] = true;
  return path;
}


/** Overload strprintf for std::string, to be able to use it with _ (translation).
 * This will not support GCC format type warnings (-Wformat) so be careful.
 */

std::string vstrprintf(const char *format, va_list ap)
{
  char buffer[50000];
  char *p = buffer;
  int limit = sizeof(buffer);
  int ret;
  while (true)
  {
    va_list arg_ptr;
    va_copy(arg_ptr, ap);
#ifdef WIN32
    ret = _vsnprintf(p, limit, format, arg_ptr);
#else
    ret = vsnprintf(p, limit, format, arg_ptr);
#endif
    va_end(arg_ptr);
    if (ret >= 0 && ret < limit)
      break;
    if (p != buffer)
      delete[] p;
    limit *= 2;
    p = new char[limit];
    if (p == NULL)
      throw std::bad_alloc();
  }
  std::string str(p, p + ret);
  if (p != buffer)
    delete[] p;
  return str;
}

std::string real_strprintf(const char *format, int dummy, ...)
{
  va_list arg_ptr;
  va_start(arg_ptr, dummy);
  std::string str = vstrprintf(format, arg_ptr);
  va_end(arg_ptr);
  return str;
}

std::string real_strprintf(const std::string &format, int dummy, ...)
{
  va_list arg_ptr;
  va_start(arg_ptr, dummy);
  std::string str = vstrprintf(format.c_str(), arg_ptr);
  va_end(arg_ptr);
  return str;
}

#define strprintf(format, ...) real_strprintf(format, 0, __VA_ARGS__)

inline std::string itostr(int n)
{
  return strprintf("%d", n);
}

bool ATTR_WARN_PRINTF(1, 2) error(const char *format, ...)
{
  va_list arg_ptr;
  va_start(arg_ptr, format);
  std::string str = vstrprintf(format, arg_ptr);
  va_end(arg_ptr);
  printf("ERROR: %s\n", str.c_str());
  return false;
}

static FILE *fileout = NULL;
inline int ATTR_WARN_PRINTF(1, 2) OutputDebugStringF(const char *pszFormat, ...)
{
  int ret = 0;
  if (fPrintToConsole)
  {
    // print to console
    va_list arg_ptr;
    va_start(arg_ptr, pszFormat);
    ret = vprintf(pszFormat, arg_ptr);
    va_end(arg_ptr);
  }
  else if (!fPrintToDebugger)
  {
    // print to debug.log

    if (!fileout)
    {
      boost::filesystem::path pathDebug = GetDataDir() / "debug.log";
      fileout = fopen(pathDebug.string().c_str(), "a");
      if (fileout)
        setbuf(fileout, NULL); // unbuffered
    }
    if (fileout)
    {
      static bool fStartedNewLine = true;

      // This routine may be called by global destructors during shutdown.
      // Since the order of destruction of static/global objects is undefined,
      // allocate mutexDebugLog on the heap the first time this routine
      // is called to avoid crashes during shutdown.
      static boost::mutex *mutexDebugLog = NULL;
      if (mutexDebugLog == NULL)
        mutexDebugLog = new boost::mutex();
      boost::mutex::scoped_lock scoped_lock(*mutexDebugLog);

      // reopen the log file, if requested
      if (fReopenDebugLog)
      {
        fReopenDebugLog = false;
        boost::filesystem::path pathDebug = GetDataDir() / "debug.log";
        if (freopen(pathDebug.string().c_str(), "a", fileout) != NULL)
          setbuf(fileout, NULL); // unbuffered
      }

      // Debug print useful for profiling
      if (fLogTimestamps && fStartedNewLine)
        fprintf(fileout, "%s ", DateTimeStrFormat("%x %H:%M:%S", GetTime()).c_str());
      if (pszFormat[strlen(pszFormat) - 1] == '\n')
        fStartedNewLine = true;
      else
        fStartedNewLine = false;

      va_list arg_ptr;
      va_start(arg_ptr, pszFormat);
      ret = vfprintf(fileout, pszFormat, arg_ptr);
      va_end(arg_ptr);
    }
  }

#ifdef WIN32
  if (fPrintToDebugger)
  {
    static CCriticalSection cs_OutputDebugStringF;

    // accumulate and output a line at a time
    {
      LOCK(cs_OutputDebugStringF);
      static std::string buffer;

      va_list arg_ptr;
      va_start(arg_ptr, pszFormat);
      buffer += vstrprintf(pszFormat, arg_ptr);
      va_end(arg_ptr);

      int line_start = 0, line_end;
      while ((line_end = buffer.find('\n', line_start)) != -1)
      {
        OutputDebugStringA(buffer.substr(line_start, line_end - line_start).c_str());
        line_start = line_end + 1;
      }
      buffer.erase(0, line_start);
    }
  }
#endif
  return ret;
}

boost::filesystem::path GetConfigFile()
{
  boost::filesystem::path pathConfigFile(GetArg("-conf", "DeepOnion.conf"));
  if (!pathConfigFile.is_complete())
    pathConfigFile = GetDataDir(false) / pathConfigFile;
  return pathConfigFile;
}

// Args handling (from util.cpp)

std::string GetArg(const std::string &strArg, const std::string &strDefault)
{
  if (mapArgs.count(strArg))
    return mapArgs[strArg];
  return strDefault;
}

int64_t GetArg(const std::string &strArg, int64_t nDefault)
{
  if (mapArgs.count(strArg))
    return atoi64(mapArgs[strArg]);
  return nDefault;
}

bool GetBoolArg(const std::string &strArg, bool fDefault)
{
  if (mapArgs.count(strArg))
  {
    if (mapArgs[strArg].empty())
      return true;
    return (atoi(mapArgs[strArg]) != 0);
  }
  return fDefault;
}

// dummy function to avoid linking with uiInterface
inline std::string _(const char *psz)
{
  boost::optional<std::string> rv = std::string(psz);
  return rv ? (*rv) : psz;
}

static void InterpretNegativeSetting(std::string name, std::map<std::string, std::string> &mapSettingsRet)
{
  // interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
  if (name.find("-no") == 0)
  {
    std::string positive("-");
    positive.append(name.begin() + 3, name.end());
    if (mapSettingsRet.count(positive) == 0)
    {
      bool value = !GetBoolArg(name);
      mapSettingsRet[positive] = (value ? "1" : "0");
    }
  }
}

void ReadConfigFile(std::map<std::string, std::string> &mapSettingsRet,
                    std::map<std::string, std::vector<std::string> > &mapMultiSettingsRet)
{
  boost::filesystem::ifstream streamConfig(GetConfigFile());
  if (!streamConfig.good())
    return; // No bitcoin.conf file is OK

  std::set<std::string> setOptions;
  setOptions.insert("*");

  for (boost::program_options::detail::config_file_iterator it(streamConfig, setOptions), end; it != end; ++it)
  {
    // Don't overwrite existing settings so command line settings override bitcoin.conf
    std::string strKey = std::string("-") + it->string_key;
    if (mapSettingsRet.count(strKey) == 0)
    {
      mapSettingsRet[strKey] = it->value[0];
      // interpret nofoo=1 as foo=0 (and nofoo=0 as foo=1) as long as foo not set)
      InterpretNegativeSetting(strKey, mapSettingsRet);
    }
    mapMultiSettingsRet[strKey].push_back(it->value[0]);
  }
}

// copied from util.h
void SetupEnvironment()
{
#ifndef WIN32
  try
  {
#if BOOST_FILESYSTEM_VERSION == 3
    boost::filesystem::path::codecvt(); // Raises runtime error if current locale is invalid
#else                                   // boost filesystem v2
    std::locale(); // Raises runtime error if current locale is invalid
#endif
  }
  catch (std::runtime_error &e)
  {
    setenv("LC_ALL", "C", 1); // Force C locale
  }
#endif
}

void ParseParameters(int argc, const char *const argv[])
{
  mapArgs.clear();
  mapMultiArgs.clear();
  for (int i = 1; i < argc; i++)
  {
    char psz[10000];
    strlcpy(psz, argv[i], sizeof(psz));
    char *pszValue = (char *)"";
    if (strchr(psz, '='))
    {
      pszValue = strchr(psz, '=');
      *pszValue++ = '\0';
    }
#ifdef WIN32
    _strlwr(psz);
    if (psz[0] == '/')
      psz[0] = '-';
#endif
    if (psz[0] != '-')
      break;
    mapArgs[psz] = pszValue;
    mapMultiArgs[psz].push_back(pszValue);
  }

  // New 0.6 features:
  BOOST_FOREACH (const PAIRTYPE(std::string, std::string) & entry, mapArgs)
  {
    std::string name = entry.first;

    //  interpret --foo as -foo (as long as both are not set)
    if (name.find("--") == 0)
    {
      std::string singleDash(name.begin() + 1, name.end());
      if (mapArgs.count(singleDash) == 0)
        mapArgs[singleDash] = entry.second;
      name = singleDash;
    }

    // interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
    InterpretNegativeSetting(name, mapArgs);
  }
}

static std::string FormatException(std::exception *pex, const char *pszThread)
{
#ifdef WIN32
  char pszModule[MAX_PATH] = "";
  GetModuleFileNameA(NULL, pszModule, sizeof(pszModule));
#else
  const char *pszModule = "DeepOnion";
#endif
  if (pex)
    return strprintf(
        "EXCEPTION: %s       \n%s       \n%s in %s       \n", typeid(*pex).name(), pex->what(), pszModule, pszThread);
  else
    return strprintf(
        "UNKNOWN EXCEPTION       \n%s in %s       \n", pszModule, pszThread);
}

void PrintExceptionContinue(std::exception *pex, const char *pszThread)
{
  std::string message = FormatException(pex, pszThread);
  printf("\n\n************************\n%s\n", message.c_str());
  fprintf(stderr, "\n\n************************\n%s\n", message.c_str());
  strMiscWarning = message;
}

template <typename T>
void ConvertTo(Value &value, bool fAllowNull = false)
{
  if (fAllowNull && value.type() == null_type)
    return;
  if (value.type() == str_type)
  {
    // reinterpret string as unquoted json value
    Value value2;
    string strJSON = value.get_str();
    if (!read_string(strJSON, value2))
      throw runtime_error(string("Error parsing JSON:") + strJSON);
    ConvertTo<T>(value2, fAllowNull);
    value = value2;
  }
  else
  {
    value = value.get_value<T>();
  }
}

// Convert strings to command-specific RPC representation
Array RPCConvertValues(const std::string &strMethod, const std::vector<std::string> &strParams)
{
  Array params;
  BOOST_FOREACH (const std::string &param, strParams)
    params.push_back(param);

  int n = params.size();

  //
  // Special case non-string parameter types
  //
  if (strMethod == "stop" && n > 0)
    ConvertTo<bool>(params[0]);
  if (strMethod == "sendtoaddress" && n > 1)
    ConvertTo<double>(params[1]);
  if (strMethod == "sendtostealthaddress" && n > 1)
    ConvertTo<double>(params[1]);
  if (strMethod == "settxfee" && n > 0)
    ConvertTo<double>(params[0]);
  if (strMethod == "getreceivedbyaddress" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "getreceivedbyaccount" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "listreceivedbyaddress" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "listreceivedbyaddress" && n > 1)
    ConvertTo<bool>(params[1]);
  if (strMethod == "listreceivedbyaccount" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "listreceivedbyaccount" && n > 1)
    ConvertTo<bool>(params[1]);
  if (strMethod == "getbalance" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "getblock" && n > 1)
    ConvertTo<bool>(params[1]);
  if (strMethod == "getblockbynumber" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "getblockbynumber" && n > 1)
    ConvertTo<bool>(params[1]);
  if (strMethod == "getblockhash" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "getinfo" && n > 0)
    ConvertTo<bool>(params[0]);
  if (strMethod == "move" && n > 2)
    ConvertTo<double>(params[2]);
  if (strMethod == "move" && n > 3)
    ConvertTo<boost::int64_t>(params[3]);
  if (strMethod == "sendfrom" && n > 2)
    ConvertTo<double>(params[2]);
  if (strMethod == "sendfrom" && n > 3)
    ConvertTo<boost::int64_t>(params[3]);
  if (strMethod == "listtransactions" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "listtransactions" && n > 2)
    ConvertTo<boost::int64_t>(params[2]);
  if (strMethod == "listaccounts" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "walletpassphrase" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "walletpassphrase" && n > 2)
    ConvertTo<bool>(params[2]);
  if (strMethod == "getblocktemplate" && n > 0)
    ConvertTo<Object>(params[0]);
  if (strMethod == "listsinceblock" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);

  if (strMethod == "sendalert" && n > 2)
    ConvertTo<boost::int64_t>(params[2]);
  if (strMethod == "sendalert" && n > 3)
    ConvertTo<boost::int64_t>(params[3]);
  if (strMethod == "sendalert" && n > 4)
    ConvertTo<boost::int64_t>(params[4]);
  if (strMethod == "sendalert" && n > 5)
    ConvertTo<boost::int64_t>(params[5]);
  if (strMethod == "sendalert" && n > 6)
    ConvertTo<boost::int64_t>(params[6]);

  if (strMethod == "sendmany" && n > 1)
    ConvertTo<Object>(params[1]);
  if (strMethod == "sendmany" && n > 2)
    ConvertTo<boost::int64_t>(params[2]);
  if (strMethod == "reservebalance" && n > 0)
    ConvertTo<bool>(params[0]);
  if (strMethod == "reservebalance" && n > 1)
    ConvertTo<double>(params[1]);
  if (strMethod == "addmultisigaddress" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "addmultisigaddress" && n > 1)
    ConvertTo<Array>(params[1]);
  if (strMethod == "listunspent" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "listunspent" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "listunspent" && n > 2)
    ConvertTo<Array>(params[2]);
  if (strMethod == "getrawtransaction" && n > 1)
    ConvertTo<boost::int64_t>(params[1]);
  if (strMethod == "createrawtransaction" && n > 0)
    ConvertTo<Array>(params[0]);
  if (strMethod == "createrawtransaction" && n > 1)
    ConvertTo<Object>(params[1]);
  if (strMethod == "signrawtransaction" && n > 1)
    ConvertTo<Array>(params[1], true);
  if (strMethod == "signrawtransaction" && n > 2)
    ConvertTo<Array>(params[2], true);
  if (strMethod == "scanforalltxns" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "scanforstealthtxns" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);
  if (strMethod == "keypoolrefill" && n > 0)
    ConvertTo<boost::int64_t>(params[0]);

  return params;
}

inline void MilliSleep(int64_t n)
{
#if BOOST_VERSION >= 105000
  boost::this_thread::sleep_for(boost::chrono::milliseconds(n));
#else
  boost::this_thread::sleep(boost::posix_time::milliseconds(n));
#endif
}

string HTTPPost(const string &strMsg, const map<string, string> &mapRequestHeaders)
{
  ostringstream s;
  s << "POST / HTTP/1.1\r\n"
    << "User-Agent: bitcoin-json-rpc/" << FormatFullVersion() << "\r\n"
    << "Host: 127.0.0.1\r\n"
    << "Content-Type: application/json\r\n"
    << "Content-Length: " << strMsg.size() << "\r\n"
    << "Connection: close\r\n"
    << "Accept: application/json\r\n";
  BOOST_FOREACH (const PAIRTYPE(string, string) & item, mapRequestHeaders)
    s << item.first << ": " << item.second << "\r\n";
  s << "\r\n"
    << strMsg;

  return s.str();
}

int ReadHTTPStatus(std::basic_istream<char> &stream, int &proto)
{
  string str;
  getline(stream, str);
  vector<string> vWords;
  boost::split(vWords, str, boost::is_any_of(" "));
  if (vWords.size() < 2)
    return HTTP_INTERNAL_SERVER_ERROR;
  proto = 0;
  const char *ver = strstr(str.c_str(), "HTTP/1.");
  if (ver != NULL)
    proto = atoi(ver + 7);
  return atoi(vWords[1].c_str());
}

int ReadHTTPHeaders(std::basic_istream<char> &stream, map<string, string> &mapHeadersRet)
{
  int nLen = 0;
  while (true)
  {
    string str;
    std::getline(stream, str);
    if (str.empty() || str == "\r")
      break;
    string::size_type nColon = str.find(":");
    if (nColon != string::npos)
    {
      string strHeader = str.substr(0, nColon);
      boost::trim(strHeader);
      boost::to_lower(strHeader);
      string strValue = str.substr(nColon + 1);
      boost::trim(strValue);
      mapHeadersRet[strHeader] = strValue;
      if (strHeader == "content-length")
        nLen = atoi(strValue.c_str());
    }
  }
  return nLen;
}

int ReadHTTPMessage(std::basic_istream<char> &stream, map<string, string> &mapHeadersRet, string &strMessageRet,
                    int nProto)
{
  mapHeadersRet.clear();
  strMessageRet = "";

  // Read header
  int nLen = ReadHTTPHeaders(stream, mapHeadersRet);
  if (nLen < 0 || nLen > (int)MAX_SIZE)
    return HTTP_INTERNAL_SERVER_ERROR;

  // Read message
  if (nLen > 0)
  {
    vector<char> vch(nLen);
    stream.read(&vch[0], nLen);
    strMessageRet = string(vch.begin(), vch.end());
  }

  string sConHdr = mapHeadersRet["connection"];

  if ((sConHdr != "close") && (sConHdr != "keep-alive"))
  {
    if (nProto >= 1)
      mapHeadersRet["connection"] = "keep-alive";
    else
      mapHeadersRet["connection"] = "close";
  }

  return HTTP_OK;
}

string JSONRPCRequest(const string &strMethod, const Array &params, const Value &id)
{
  Object request;
  request.push_back(Pair("method", strMethod));
  request.push_back(Pair("params", params));
  request.push_back(Pair("id", id));
  return write_string(Value(request), false) + "\n";
}

string EncodeBase64(const unsigned char *pch, size_t len)
{
  static const char *pbase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  string strRet = "";
  strRet.reserve((len + 2) / 3 * 4);

  int mode = 0, left = 0;
  const unsigned char *pchEnd = pch + len;

  while (pch < pchEnd)
  {
    int enc = *(pch++);
    switch (mode)
    {
    case 0: // we have no bits
      strRet += pbase64[enc >> 2];
      left = (enc & 3) << 4;
      mode = 1;
      break;

    case 1: // we have two bits
      strRet += pbase64[left | (enc >> 4)];
      left = (enc & 15) << 2;
      mode = 2;
      break;

    case 2: // we have four bits
      strRet += pbase64[left | (enc >> 6)];
      strRet += pbase64[enc & 63];
      mode = 0;
      break;
    }
  }

  if (mode)
  {
    strRet += pbase64[left];
    strRet += '=';
    if (mode == 1)
      strRet += '=';
  }

  return strRet;
}

string EncodeBase64(const string &str)
{
  return EncodeBase64((const unsigned char *)str.c_str(), str.size());
}

Object CallRPC(const string &strMethod, const Array &params)
{
  if (mapArgs["-rpcuser"] == "" && mapArgs["-rpcpassword"] == "")
    throw runtime_error(strprintf(
        _("You must set rpcpassword=<password> in the configuration file:\n%s\n"
          "If the file does not exist, create it with owner-readable-only file permissions."),
        GetConfigFile().string().c_str()));

  // Connect to localhost
  bool fUseSSL = GetBoolArg("-rpcssl", false);
  asio::io_service io_service;
  ssl::context context(io_service, ssl::context::sslv23);
  context.set_options(ssl::context::no_sslv2 | ssl::context::no_sslv3);
  asio::ssl::stream<asio::ip::tcp::socket> sslStream(io_service, context);
  SSLIOStreamDevice<asio::ip::tcp> d(sslStream, fUseSSL);
  iostreams::stream<SSLIOStreamDevice<asio::ip::tcp> > stream(d);

  bool fWait = GetBoolArg("-rpcwait", false); // -rpcwait means try until server has started
  do
  {
    bool fConnected = d.connect(GetArg("-rpcconnect", "127.0.0.1"), GetArg("-rpcport", itostr(fTestNet ? 28580 : 18580)));
    if (fConnected)
      break;
    if (fWait)
      MilliSleep(1000);
    else
      throw runtime_error("couldn't connect to server");
  } while (fWait);

  // HTTP basic authentication
  string strUserPass64 = EncodeBase64(mapArgs["-rpcuser"] + ":" + mapArgs["-rpcpassword"]);
  map<string, string> mapRequestHeaders;
  mapRequestHeaders["Authorization"] = string("Basic ") + strUserPass64;

  // Send request
  string strRequest = JSONRPCRequest(strMethod, params, 1);
  string strPost = HTTPPost(strRequest, mapRequestHeaders);
  stream << strPost << std::flush;

  // Receive HTTP reply status
  int nProto = 0;
  int nStatus = ReadHTTPStatus(stream, nProto);

  // Receive HTTP reply message headers and body
  map<string, string> mapHeaders;
  string strReply;
  ReadHTTPMessage(stream, mapHeaders, strReply, nProto);

  if (nStatus == HTTP_UNAUTHORIZED)
    throw runtime_error("incorrect rpcuser or rpcpassword (authorization failed)");
  else if (nStatus >= 400 && nStatus != HTTP_BAD_REQUEST && nStatus != HTTP_NOT_FOUND && nStatus != HTTP_INTERNAL_SERVER_ERROR)
    throw runtime_error(strprintf("server returned HTTP error %d", nStatus));
  else if (strReply.empty())
    throw runtime_error("no response from server");

  // Parse reply
  Value valReply;
  if (!read_string(strReply, valReply))
    throw runtime_error("couldn't parse reply from server");
  const Object &reply = valReply.get_obj();
  if (reply.empty())
    throw runtime_error("expected reply to have result, error and id properties");

  return reply;
}

int CommandLineRPC(int argc, char *argv[])
{
  string strPrint;
  int nRet = 0;
  try
  {
    // Skip switches
    while (argc > 1 && IsSwitchChar(argv[1][0]))
    {
      argc--;
      argv++;
    }

    // Method
    if (argc < 2)
      throw runtime_error("too few parameters");
    string strMethod = argv[1];

    // Parameters default to strings
    std::vector<std::string> strParams(&argv[2], &argv[argc]);
    Array params = RPCConvertValues(strMethod, strParams);

    // Execute
    Object reply = CallRPC(strMethod, params);

    // Parse reply
    const Value &result = find_value(reply, "result");
    const Value &error = find_value(reply, "error");

    if (error.type() != null_type)
    {
      // Error
      strPrint = "error: " + write_string(error, false);
      int code = find_value(error.get_obj(), "code").get_int();
      nRet = abs(code);
    }
    else
    {
      // Result
      if (result.type() == null_type)
        strPrint = "";
      else if (result.type() == str_type)
        strPrint = result.get_str();
      else
        strPrint = write_string(result, true);
    }
  }
  catch (boost::thread_interrupted)
  {
    throw;
  }
  catch (std::exception &e)
  {
    strPrint = string("error: ") + e.what();
    nRet = abs(RPC_MISC_ERROR);
  }
  catch (...)
  {
    PrintExceptionContinue(NULL, "CommandLineRPC()");
    throw;
  }

  if (strPrint != "")
  {
    fprintf((nRet == 0 ? stdout : stderr), "%s\n", strPrint.c_str());
  }
  return nRet;
}

std::string HelpMessageCli(bool mainProgram)
{
  string strUsage;
  if (mainProgram)
  {
    strUsage += _("Options:") + "\n";
    strUsage += "  -?                     " + _("This help message") + "\n";
    strUsage += "  -conf=<file>           " + _("Specify configuration file (default: DeepOnion.conf)") + "\n";
    strUsage += "  -datadir=<dir>         " + _("Specify data directory") + "\n";
    strUsage += "  -testnet               " + _("Use the test network") + "\n" +
                "\n";
  }
  else
  {
    strUsage += _("RPC client options:") + "\n";
  }

  strUsage += "  -rpcconnect=<ip>       " + _("Send commands to node running on <ip> (default: 127.0.0.1)") + "\n";
  strUsage += "  -rpcport=<port>        " + _("Connect to JSON-RPC on <port> (default: 18580 or testnet: 28580)") + "\n";
  strUsage += "  -rpcwait               " + _("Wait for RPC server to start") + "\n";

  if (mainProgram)
  {
    strUsage += "  -rpcuser=<user>        " + _("Username for JSON-RPC connections") + "\n";
    strUsage += "  -rpcpassword=<pw>      " + _("Password for JSON-RPC connections") + "\n";
    strUsage += "  -rpcssl                " + _("Use OpenSSL (https) for JSON-RPC connections") + "\n";
  }

  return strUsage;
}

#endif //DEEPONION_CLI_H