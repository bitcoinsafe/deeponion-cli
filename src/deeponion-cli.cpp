// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Copyright (c) 2014-2018 The DeepOnion developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "rpcclient.h"
#include "rpcprotocol.h"
#include "deeponion-cli.h"

#include <boost/filesystem/operations.hpp>

extern std::map<std::string, std::string> mapArgs;
extern std::map<std::string, std::vector<std::string> > mapMultiArgs;

std::string strMiscWarning = "";
bool fTestNet = false;
bool fPrintToDebugger = false;
bool fPrintToConsole = false;
bool fLogTimestamps = false;
//////////////////////////////////////////////////////////////////////////////
//
// Start
//
static bool AppInitRPC(int argc, char* argv[])
{
    //
    // Parameters
    //
    ParseParameters(argc, argv);
    fTestNet = GetBoolArg("-testnet", true);
    fPrintToDebugger = GetBoolArg("-printtodebugger");
    fPrintToConsole = GetBoolArg("-printtoconsole");
    fLogTimestamps = GetBoolArg("-logtimestamps");
    if (!boost::filesystem::is_directory(GetDataDir(false)))
    {
        fprintf(stderr, "Error: Specified data directory \"%s\" does not exist.\n", mapArgs["-datadir"].c_str());
        return false;
    }
    try {
        ReadConfigFile(mapArgs, mapMultiArgs);
    } catch(std::exception &e) {
        fprintf(stderr,"Error reading configuration file: %s\n", e.what());
        return false;
    }
    // Check for -testnet or -regtest parameter (TestNet() calls are only valid after this clause)
    //TODO | currently no regtest available
    // if (!SelectParamsFromCommandLine()) {
    //     fprintf(stderr, "Error: Invalid combination of -regtest and -testnet.\n");
    //     return false;
    // }

    if (argc<2 || mapArgs.count("-?") || mapArgs.count("--help"))
    {
        // First part of help message is specific to RPC client
        std::string strUsage = _("DeepOnion RPC client version") + " " + FormatFullVersion() + "\n\n" +
            _("Usage:") + "\n" +
              "  deeponion-cli [options] <command> [params]  " + _("Send command to DeepOnion") + "\n" +
              "  deeponion-cli [options] help                " + _("List commands") + "\n" +
              "  deeponion-cli [options] help <command>      " + _("Get help for a command") + "\n";

        strUsage += "\n" + HelpMessageCli(true);

        fprintf(stdout, "%s", strUsage.c_str());
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    SetupEnvironment();

    try
    {
        if(!AppInitRPC(argc, argv))
            return abs(RPC_MISC_ERROR);
    }
    catch (std::exception& e) {
        PrintExceptionContinue(&e, "AppInitRPC()");
        return abs(RPC_MISC_ERROR);
    } catch (...) {
        PrintExceptionContinue(NULL, "AppInitRPC()");
        return abs(RPC_MISC_ERROR);
    }

    int ret = abs(RPC_MISC_ERROR);
    try
    {
        ret = CommandLineRPC(argc, argv);
    }
    catch (std::exception& e) {
        PrintExceptionContinue(&e, "CommandLineRPC()");
    } catch (...) {
        PrintExceptionContinue(NULL, "CommandLineRPC()");
    }
    return ret;
}
