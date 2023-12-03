PORTNAME=	dotnet
DISTVERSIONPREFIX=	v
DISTVERSION=	8.0.0
CATEGORIES=	lang
MASTER_SITES+=	https://github.com/dotnet/dotnet/releases/download/${DISTVERSIONPREFIX}${DISTVERSION}/:dotnetmanifest \
		https://github.com/Thefrank/freebsd-dotnet-sourcebuild/releases/download/v8.0.100-FreeBSD14x64/:bootstrapartifacts \
		https://github.com/sec/dotnet-core-freebsd-source-build/releases/download/8.0.100-rc.2-x64-native/:bootstrapsdk \
		https://dotnetcli.azureedge.net/source-built-artifacts/assets/:msft
DISTFILES+=	release.json:dotnetmanifest \
		Microsoft.NETCore.App.Crossgen2.freebsd-x64.${DOTNET_BOOTSTRAPRUNTIME}.nupkg:bootstrapsdk \
		Microsoft.NETCore.App.Runtime.freebsd-x64.${DOTNET_BOOTSTRAPRUNTIME}.nupkg:bootstrapsdk \
		runtime.freebsd-x64.Microsoft.NETCore.ILAsm.${DOTNET_BOOTSTRAPRUNTIME}.nupkg:bootstrapsdk \
		runtime.freebsd-x64.Microsoft.NETCore.ILDAsm.${DOTNET_BOOTSTRAPRUNTIME}.nupkg:bootstrapsdk \
		Microsoft.NETCore.App.Host.freebsd-x64.8.0.0-rc.2.23479.6.nupkg:bootstrapsdk \
		Private.SourceBuilt.Artifacts.${DOTNET_BOOTSTRAPARTIFACTS}.centos.8-x64.tar.gz:msft \
		dotnet-sdk-${DOTNET_BOOTSTRAPSDK}-freebsd-x64.tar.gz:bootstrapsdk \
		Prep.Packages.tar.gz:bootstrapartifacts
DIST_SUBDIR=	${PORTNAME}
EXTRACT_ONLY=	${DISTNAME}.tar.gz

MAINTAINER=	me@hoem.com
COMMENT=	This is a port of .NET (dotNET) to FreeBSD
WWW=		https://dotnet.microsoft.com/en-us/

LICENSE=	MIT

ONLY_FOR_ARCHS=	amd64

DOTNET_DEPENDS=	bash>0:shells/bash icu>0:devel/icu krb5>0:security/krb5 \
		libinotify>0:devel/libinotify libunwind>0:devel/libunwind \
		terminfo-db>0:misc/terminfo-db
BUILD_DEPENDS=	${DOTNET_DEPENDS} \
		cmake>0:devel/cmake \
		gnugrep>0:textproc/gnugrep
RUN_DEPENDS=	${DOTNET_DEPENDS}

USES=		elfctl gssapi:mit llvm:min=15 nodejs python:build shebangfix ssl
SHEBANG_FILES=	src/diagnostics/src/Tools/dotnet-trace/*.sh \
		src/source-build-externals/src/azure-activedirectory-identitymodel-extensions-for-dotnet/updateAssemblyInfo.sh

DOTNET_BOOTSTRAPSDK=		8.0.100-rc.2.23502.2
DOTNET_BOOTSTRAPARTIFACTS=	8.0.100-rc.2.23502.1
DOTNET_BOOTSTRAPRUNTIME=	8.0.0-rc.2.23479.6
ARTIFACTS_PATH=			${WRKSRC}/artifacts/x64/Release/
DOTNET_ENV=	CLR_OPENSSL_VERSION_OVERRIDE=30
USE_GITHUB=			yes

.include <bsd.port.options.mk>

pre-patch:
	${MKDIR} ${WRKSRC}/artifacts/unpacked ${WRKSRC}/prereqs/packages/archive/ ${WRKSRC}/.dotnet;\
	${CP} ${_DISTDIR}/release.json ${WRKSRC}/; \
	${CP} ${_DISTDIR}/*.nupkg ${WRKSRC}/artifacts/unpacked ;\
	${EXTRACT_CMD} ${EXTRACT_BEFORE_ARGS} ${_DISTDIR}/Private.SourceBuilt.Artifacts.${DOTNET_BOOTSTRAPARTIFACTS}.centos.8-x64.tar.gz -C ${WRKSRC}/artifacts/unpacked ${EXTRACT_AFTER_ARGS};\
	${EXTRACT_CMD} ${EXTRACT_BEFORE_ARGS} ${_DISTDIR}/dotnet-sdk-${DOTNET_BOOTSTRAPSDK}-freebsd-x64.tar.gz -C ${WRKSRC}/.dotnet ${EXTRACT_AFTER_ARGS};\
	${EXTRACT_CMD} ${EXTRACT_BEFORE_ARGS} ${_DISTDIR}/Prep.Packages.tar.gz -C ${WRKSRC}/artifacts/unpacked ${EXTRACT_AFTER_ARGS} ;\
	${ELFCTL} -e +noaslr ${WRKSRC}/.dotnet/dotnet; \
	cd ${WRKSRC};\
	${AWK} -vmax="$$(${GREP} -c '' eng/bootstrap/OverrideBootstrapVersions.props)" 'NR>1 && NR<max' eng/bootstrap/OverrideBootstrapVersions.props > artifacts/TrimOverrideBootstrapVersions.props ;\
	${AWK} '/<\/Project>/{while(getline line<"artifacts/TrimOverrideBootstrapVersions.props"){print line}} //' artifacts/unpacked/PackageVersions.props > artifacts/UpdatedPackageVersions.props ;\
	${MV} artifacts/UpdatedPackageVersions.props artifacts/unpacked/PackageVersions.props ;\
	cd ${WRKSRC}/artifacts/unpacked ;\
	${TAR} --numeric-owner -cf ${WRKSRC}/prereqs/packages/archive/Private.SourceBuilt.Artifacts.Bootstrap.tar.gz *.nupkg *.props SourceBuildReferencePackages/ ;\
	${SED} -i '' -E 's/grep\ \-oP/ggrep\ \-oP/' ${WRKSRC}/build.sh
do-build:
	cd ${WRKSRC};\
	${DOTNET_ENV} ./build.sh --release-manifest release.json --clean-while-building
pre-install:
	${MKDIR} ${ARTIFACTS_PATH}/${PORTNAME} && (${EXTRACT_CMD} ${EXTRACT_BEFORE_ARGS} ${ARTIFACTS_PATH}/dotnet-sdk* -C ${ARTIFACTS_PATH}/${PORTNAME} ${EXTRACT_AFTER_ARGS})
	${ELFCTL} -i -e +noaslr ${ARTIFACTS_PATH}/${PORTNAME}/${PORTNAME}
	${FIND} ${ARTIFACTS_PATH}/${PORTNAME} '(' -name "*.so" -or -name "apphost" -or -name "singlefilehost" -or -type f -name "dotnet" ')' -exec ${STRIP_CMD} -s "{}" ";"
do-install:
	cd ${ARTIFACTS_PATH}/${PORTNAME} && ${COPYTREE_SHARE} . ${STAGEDIR}/${LOCALBASE}/${PORTNAME}/ "! (  -name "*.so" -or -name "apphost" -or -name "singlefilehost" -or -name "dotnet" )"
	cd ${ARTIFACTS_PATH}/${PORTNAME} && ${COPYTREE_BIN} . ${STAGEDIR}/${LOCALBASE}/${PORTNAME}/ " -name "*.so" -or -name "apphost" -or -name "singlefilehost" -or -name "dotnet" "
	${RLN} ${STAGEDIR}/${LOCALBASE}/${PORTNAME}/${PORTNAME} ${STAGEDIR}/${LOCALBASE}/bin/${PORTNAME}

.include <bsd.port.mk>
