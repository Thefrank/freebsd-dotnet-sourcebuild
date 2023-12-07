# FreeBSD dotNET Sourcebuild

## Requirements
- FreeBSD 12.4+, 8GB RAM, 75GB free space
  - more threads/RAM/space the faster
- Make sure you understand "STABLE", "CURRENT", and "ABI" within the context of FreeBSD -> https://wiki.freebsd.org/Glossary
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
  - Only tested with OpenSSL 1.1.1 and LLVM15 
- optional: `ninja curl`
- if in a jail: `allow.mlock=true`

## Steps
Please check `./patches`
Some builds can use `prep.sh` some can not

## Issues
- `sysctl 1 failed with 3 error`
  - Unknown cause. Old MSBuild would hit this when trying to reuse nodes. Try `killall dotnet` and try again
- Random crash when building
  - check `dmesg` if something was killed because of memory issues try increasing RAM (e.g., building in a VM) or increasing swap
  - Something else? Let me know.
- Hangs / Fails to finish
  - Unknown cause. `CTRL+T` check for very long `[uwait]`. `crossgen2` will sometimes do this. `CTRL+C` and try again
  - If feeling brave, `truss` to help troubleshoot
 
## FreeBSD 14
- Compile errors. Forwards declartions that never get resolved.
  - See patch. Lazy and Ugly but works(tm)
    - Fix in runtime via OS Version based IFDEF?
- ASLR crashes
  - `elfctl -e +noaslr dotnet` OR disable globally
  - Help fix in runtime?
- OpenSSL3 vs OpenSSL1.1
  - ~~OpenSSL1.1 appears to be a hard requirement AFAIK~~
    - Somewhat fixed in ports
  - Help fix in runtime?
## Port?
 - See: https://github.com/Thefrank/freebsd-dotnet-sourcebuild/tree/main/PortWIP
