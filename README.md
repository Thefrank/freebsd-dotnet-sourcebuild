# freebsd-dotnet-sourcebuild

## Requirement
- FreeBSD 12.3+, 8GB RAM, 75GB free space
  - more threads/RAM/space the faster
- `pkg install bash cmake icu libunwind krb5 openssl python libinotify llvm git terminfo-db node`
- optional: ninja
- if in a jail: `allow.mlock=true`

## Steps
- `git clone https://github.com/dotnet/dotnet
- Apply patches from ./patches or Release using `patch` instead of `git apply`
  - `patch -d dotnet/src/REPO < patches/REPO_PRNUM.patch`
- Unpack the bootstrap SDK found in Release into dotnet/.dotnet/
  - `tar xf PATH/TO/SDK/dotnet-sdk-8.0.100-VERSION.NUMBER-freebsd-x64.tar.gz -C dotnet/.dotnet`
- `cd dotnet`
- `./prep.sh`
- Using your favorite downloading tool (e.g., `fetch`, `curl`, `wget`), add the required NuGet packages to prereqs/packages/prebuilt
- Work around one issue:
  - `mv src/nuget-client/NuGet.Config src/nuget-client/NuGet.config`
  - `rm src/nuget-client/eng/source-build-patches/0001-Rename-NuGet.Config-to-NuGet.config-to-account-for-a.patch`
- `./build.sh --clean-while-building` 