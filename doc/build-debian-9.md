### DEBIAN 9 STRETCH BUILD NOTES


#### Disclaimer

This guide has it's a modified version of this original guide: [DeepOnion in Tails persistent volume](https://gist.github.com/dragononcrypto/d3df54d72b23f7c1023876cc7264707e)


The following are commands needed to build on Debian from scratch (use sudo if you are not root). It has been verified with Debian 9 Stretch

#### Prepare the system

First of all we need to get some dependencies:

``` 
sudo apt update && sudo apt upgrade -y
sudo apt install build-essential libtool autotools-dev autoconf pkg-config libssl-dev libboost-all-dev
sudo apt install libqrencode-dev libminiupnpc-dev libevent-dev libcap-dev libseccomp-dev git
sudo apt install bsdmainutils libqrencode-dev libminiupnpc-dev libevent-dev libcap-dev libseccomp-dev git 
sudo apt install software-properties-common libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler
```

Debian repository doesn’t contain libdb4.8. So you need to get add the additional APT repositories. Create the file than will contain the repositories:

``` 
sudo nano /etc/apt/sources.list.d/deeponionAdd.list
 ``` 
 
Add the additional repositories:

```
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu xenial main
deb http://ftp.de.debian.org/debian jessie-backports main
```    

Exit and save the changes (Ctrl+x).

Now go to [Bitcoin repository key](https://keyserver.ubuntu.com/pks/lookup?op=get&search=0xD46F45428842CE5E) ([ref](https://launchpad.net/~bitcoin/+archive/ubuntu/bitcoin)) copy the text, paste in a file and save as BTC.gpg in ~/Downloads, add to keyring and update the system:

```
sudo apt-key add ~/Downloads/BTC.gpg
sudo apt update
```
Install additional dependencies Berkeley 4.8++ and OpenSSL 1.0

```
sudo apt install libdb4.8-dev libdb4.8++-dev libssl1.0 libssl1.0-dev
```

#### Clone and compile DeepOnion

Clone deeponion repository:

```
cd 
git clone --recursive https://github.com/deeponion/deeponion.git
```

Build with autotools:

```
cd deeponion
./autogen.sh
./configure --with-gui=qt5
make
```

Now the system is compiled. You can copy DeepOnion-qt file to the place that you want. Go to the desired folder and do:

```
cp ~/deeponion/src/qt/DeepOnion-qt .
```



