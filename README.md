# FreeBSD dotNET Sourcebuild

## Requirements
- FreeBSD 12.4+, 8GB RAM, 75GB free space
  - more threads/RAM/space the faster
  - ABI difference might make this less portable than you want
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
- optional: `ninja curl`
- if in a jail: `allow.mlock=true`

## Steps
Please check `./patches`

## Issues
- `sysctl 1 failed with 3 error`
  - Unknown cause. Old MSBuild would hit this when trying to reuse nodes. Try `killall dotnet` and try again
- Random crash when building
  - check `dmesg` if something was killed because of memory issues try increasing RAM (e.g., building in a VM) or increasing swap
  - Something else? Let me know.
- Hangs / Fails to finish
  - Break and try again
