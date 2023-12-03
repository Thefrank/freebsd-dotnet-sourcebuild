## WIP for lang/dotnet8 port

TL;DR: "How not to Port 101". Tested working on 13.2-RELEASE-p4 and 14.0-RELEASE.
Notes including by "block" in `Makefile`, port stage, and some general thoughts.

### `PORTNAME`

Off to a bad start as we need a number of prebuild packages along with the offical tarball from Microsoft
I was not able to figure out a way to get the port system to handle the extraction of both the git tarball and the other prebuilts

### `MAINTAINER`

Filler for now so `portlint` stops complaining

### `LICENSE`

MIT? Yes, MIT. k.

### `BROKEN/IGNORE/DEPRECATED`

ARM64 support exists but I lack the hardware to test it so `ONLY_FOR_ARCHS` used

### Dependencies

Some of the `USES` might/should(?) be shifted into the `BUILD_DEPENDS`
`gnugrep` is used for exactly one statement during the build. A proper patch would be to fix the statement to work without `-P`

### `USES`

See above.
`ELF_FEATURES=  +noaslr:artifacts/x64/Release/${PORTNAME}/dotnet` or something like it works post-build but we also need it pre-build too

There are more files that could use shebang fixes but the ones listed are the only ones that directly break the build

### Options section: currently missing

I would like to add support for changing the `--clean-while-building` option and `STRIP` behavior for those that want it included
Options would require a change to how various stages work.
```
#OPTIONS_DEFINE=                CWB STRIP
#OPTIONS_DEFAULT=       CWB
#OPTIONS_DEFAULT=       STRIP

#CWB_DESC=      Clean while building
#STRIP_DESC=    Strip debug_info

#CWB_ON=                --clean-while-building
#STRIP_OFF=     ${STRIP}=
```

### Everything else

Variables provided to make it easier for me

### `fetch` stage

See `PORTNAME` notes.
The `prep.sh` would need to be run here but can't as it has not been extract from the tarball yet
The `extract` stage works well enough for unpacking the git tarball
This brings us to...

### `patch` stage

Work needs to be done on VMR's `eng/bootstrap/buildBootstrapPreviouslySB.csproj` to better support ports

We no longer have network access after the `fetch` stage. 
The unpacking, `awk` and repacking tries to do what `prep.sh` would be doing.

`elfctl` used as ASLR does not work here (yet?) and too early for `ELF_FEATURES` to work

gnugrep (`ggrep`) is needed for exactly one statement and should be fixed upstream. The sed line fix can be removed afterwards

### `build` stage

This uses its own build flow that includes cmake, clang, items it just built, node, and python

### `install` stage

Output is unpacked at the end of the build before installing
Unpacked output has `elfctl` and `strip` run over the items that need it

### `test` stage

Nothing yet. Only smoke tests are supported in VMR right now and require patching.
This would also need to be a custom `do-test`

### General thoughts and notes

This would be much easier if a depends for `security/opensslXXX` could be used but I was informed that doing so would kill the chances of the port being merged. 
I was under the general thought that items for ports should use other items made in ports and items found in base were generally for the OS and building the OS. This no longer is the case?
Hopefully this port's usage of ports `libunwind` and `uses=gssapi:mit` are acceptable as there are base variants of those. If not, some make changes will need to be merged into `runtime` to change how it uses those for FreeBSD.
VMR produces a NON-PORTABLE build. It can consume a PORTABLE or NON-PORTABLE build but it will never output a PORTABLE build.
I suggest using `make install-missing-packages` before running `make install` or `make package`. If you have a burning desire to build all of the depends I suggest `-DBATCH` or real port management tools.