## Command Line Interface for DeepOnion

![dologo](https://camo.githubusercontent.com/b3793963a4860e775c6180b1cdbf7480e469f874/68747470733a2f2f696d6167652e6962622e636f2f6d66523452362f444f2e706e67)

This a console-based CLI for RPC (*remote procedure call*) access of local or remote Wallets and/or Daemons.

This is how it looks like in action (Windows, Mac, and Linux):

**Get Blockchain Info**

![rpcaccess](https://img2.picload.org/image/daillwol/cli.png)

**Get Blockchain Info on Mac**

![rpcaccessmac](https://img3.picload.org/image/dailwida/mac_cli.png)

**Get Blockchain Info on Linux**

![rpcaccessunix](https://img3.picload.org/image/daicdial/cli.png)

**Get Transaction**

![rpcaccess2](https://img2.picload.org/image/daillwoi/cli2.png)

**Usage Info**

![rpchelp](https://img2.picload.org/image/daillwlr/cli3.png)
=====================================

## Compilation

To compile the CLI under Windows use the **DeepOnion-cli.pro** QT project file.

```
qmake DeepOnion-cli.pro
make
```

Unix & Mac will generate a separate CLI binary by executing the default *autotools*.

```
./autogen.sh
./configure --disable-dependency-tracking
make
```

The CLI-binary will be located at `src/DeepOnion-cli`.

