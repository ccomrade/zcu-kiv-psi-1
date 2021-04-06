# zcu-kiv-psi-1

This is the first KIV/PSI project. It's a basic [SSDP](https://en.wikipedia.org/wiki/Simple_Service_Discovery_Protocol)
client for discovering devices and services on local network.

## Implementation

The client is implemented in C and uses only the standard library and POSIX sockets.
It should work on any POSIX-compatible platform.
Windows operating system is not supported.

## Building

First of all, make sure that `gcc` compiler and `make` tool are installed.
On Debian-based systems, you only need to install the `build-essential` package.

Next, clone this repository to some directory and switch to it.
Then run the following command to compile the client:

```
make
```

You should now see the resulting `ssdp_client` executable in the current directory.

## Usage

To obtain a list of local devices and all services provided by them, simply run the client:

```
./ssdp_client
```

Here's an example output:

```
HTTP/1.1 200 OK
CACHE-CONTROL: max-age=100
DATE: Sat, 03 Apr 2021 08:36:50 GMT
EXT:
LOCATION: http://192.168.8.2:1900/igd.xml
SERVER: vxWorks/5.5 UPnP/1.0 Archer C80/1.0
ST: upnp:rootdevice
USN: uuid:upnp-InternetGatewayDevice-71EE9B5FBEDC::upnp:rootdevice


HTTP/1.1 200 OK
CACHE-CONTROL: max-age=1800
DATE: Sat, 03 Apr 2021 08:36:50 GMT
ST: urn:schemas-upnp-org:device:MediaServer:1
USN: uuid:4d696e69-444c-164e-9d41-1c1b0d286a2b::urn:schemas-upnp-org:device:MediaServer:1
EXT:
SERVER: Debian DLNADOC/1.50 UPnP/1.0 MiniDLNA/1.2.1
LOCATION: http://192.168.8.1:8200/rootDesc.xml
Content-Length: 0


...
```
