# FreeBSD dotNET Sourcebuild

## Requirements
- FreeBSD 12.3+, 8GB RAM, 75GB free space
  - more threads/RAM/space the faster
  - OOM dmesg? More RAM or more swap
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
- optional: `ninja curl`
- if in a jail: `allow.mlock=true`

## Steps
Vary! Please check `./patches`
