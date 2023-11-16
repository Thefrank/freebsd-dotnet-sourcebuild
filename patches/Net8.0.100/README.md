# Baseline instructions

## Requirements
- FreeBSD 12.4+, 8GB RAM, 75GB free space
- Make sure you understand "STABLE", "CURRENT", and "ABI" within the context of FreeBSD -> https://wiki.freebsd.org/Glossary
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
  - Only tested with OpenSSL 1.1.1 and LLVM15 under FreeBSD 12.4
- optional: `ninja curl`
- if in a jail: `allow.mlock=true`
- Sourcebuilt Bootstrap SDK
- Sourcebuilt Bootstrap Package

## Suggestions
- Make a jail of this build environment and do not update it
- SSD/NVME

## Steps
- Clone repos (This should also work from tarball but I have not tested it)
  - `git clone https://github.com/Thefrank/freebsd-dotnet-sourcebuild`
  - `git clone https://github.com/dotnet/dotnet dotnet-dotnet`
  - `git -C dotnet-dotnet checkout tags/v8.0.100`
- Apply patches and backports from ./patches/Net(VERSION) or Release using `patch` instead of `git apply`. Patch fuzzes by default and I am lazy
  - `patch -d dotnet-dotnet/src/REPO < freebsd-dotnet-sourcebuild/patches/Net(VERSION)/REPO_PRNUM.patch`
  - `patch -d dotnet-dotnet/src/REPO < freebsd-dotnet-sourcebuild/patches/Net(VERSION)/REPO_backportPRNUM.patch`
- Download and unpack SDK and Artifacts from Release
  - `tar xf Private.SourceBuilt.Artifacts.(VERSION).freebsd.(MAJOR)-(ARCH).tar.gz -C /PATH/TO/Artifacts/`
  - `tar xf dotnet-sdk-(VERSION)-freebsd-(ARCH).tar.gz -C /PATH/TO/Sdk/`
- Almost time to build
 - `cd dotnet-dotnet`
- Lazy and dangerous shebang fix OR lazy and linuxism symlink
  - [ ] `git grep -l "/bin/bash" | xargs sed -i '' -e 's/\/bin\/bash/\/usr\/bin\/env bash/g'`
  - [ ] `ln -s /usr/local/bin/bash /bin/bash`
- Build
- `./build.sh --with-packages /PATH/TO/Artifacts/ --with-sdk ~/PATH/TO/SDK/ --clean-while-building` 

## Smoke Tests
These require some additional patching to make sure they a) are not skipped and b) run correctly.
Expect failures to be: related to the build system assuming it is on linux, not being able to restore packages for FreeBSD via official NuGet feeds, and tests expecting linuxism outputs.