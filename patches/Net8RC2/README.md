# Baseline instructions

## Requirements
- FreeBSD 12.3+, 8GB RAM, 75GB free space
  - more threads/RAM/space the faster
  - OOM dmesg? More RAM or more swap
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
- optional: ninja curl
- if in a jail: `allow.mlock=true`

## Steps
- `git clone https://github.com/Thefrank/freebsd-dotnet-sourcebuild`
- `git clone https://github.com/dotnet/dotnet dotnet-dotnet`
- Apply patches and backports from ./patches/Net(VERSION) or Release using `patch` instead of `git apply`. Patch fuzzes by default and I am lazy
  - `patch -d dotnet-dotnet/src/REPO < freebsd-dotnet-sourcebuild/patches/Net(VERSION)/REPO_PRNUM.patch`
  - `patch -d dotnet-dotnet/src/REPO < freebsd-dotnet-sourcebuild/patches/Net(VERSION)/REPO_backportPRNUM.patch`
- SDK Bootstrap. ONE OF:
  - [ ] `tar xf PATH/TO/SDK/dotnet-sdk-8.0.100-VERSION.NUMBER-freebsd-x64.tar.gz -C dotnet-dotnet/.dotnet`
  - [ ] Place the Sdk into a path with this structure: `/PATH/Sdk/8.0.100-VERSION.NUMBER/dotnet-sdk-8.0.100-VERSION.NUMBER-freebsd-x64.tar.gz`
- `cd dotnet-dotnet`
- Lazy and dangerous shebang fix or lazy and linuxism symlink
  - [ ] `git grep -l "/bin/bash" | xargs sed -i '' -e 's/\/bin\/bash/\/usr\/bin\/env bash/g'`
  - [ ] `ln -s /usr/local/bin/bash /bin/bash`
- Prep
  - `./prep.sh` (If you used the second method add `--runtime-source-feed /PATH/`)
- Using your favorite downloading tool (e.g., `fetch`, `curl`, `wget`), add the required NuGet packages found in `patches/Net(VERSION)/NuGets.txt` to `prereqs/packages/prebuilt`
  - e.g. `cat PLACE/OF/freebsd-dotnet-sourcebuild/patches/Net8RC2/NuGets.txt | xargs -p -I {} fetch {} -o PLACE/OF/dotnet-dotnet/prereqs/packages/prebuilt/`
- `./build.sh --clean-while-building` 
