# Interbanqa

A P2P Banking system node.

# Configuration

In project root, create `config.json` with the following format:

```json
{
	"address": "[IPv4 Address Interbanqa will listen on]",
	"prefix": [IPv4 network prefix of the address above (between 0-32)],
	"min_port": [Minimum port Interbanqa will search],
	"max_port": [Maximum port Interbanqa will search],
	"port": [Port number Interbanqa will listen on (between min_port-max_port)],
	"timeout": [Timeout for requests, in seconds]
}
```

# Building

## Linux <a id='building-linux'></a>

+	Install dependencies: [Boost](https://www.boost.org/), A C compiler, A C++ compiler (I recommend using the package manager to do so).
+	Run `./configure`, `make`.

## Windows <a id='building-windows'></a>

+	Install [MSYS2](https://www.msys2.org/).
+	Open MSYS2 (I recommend using the MINGW64 variant).
+	Follow the [Linux steps](#building-linux) above.

# Usage

## Linux

+	Run `interbanqa`.
+	Connect to the node with any TCP client (e.g. PuTTY or `telnet`).

If `interbanqa` isn't present, see [Building instructions](#building-linux).

## Windows

+	Run `interbanqa.exe`.
+	Connect to the node with any TCP client (e.g. PuTTY or `telnet`).

If `interbanqa.exe` isn't present, see [Building instructions](#building-windows).

# Sources

See `sources.md`.