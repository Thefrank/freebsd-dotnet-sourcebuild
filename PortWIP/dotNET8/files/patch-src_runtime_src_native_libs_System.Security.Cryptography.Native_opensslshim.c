--- src/runtime/src/native/libs/System.Security.Cryptography.Native/opensslshim.c.orig	2023-11-01 21:55:24 UTC
+++ src/runtime/src/native/libs/System.Security.Cryptography.Native/opensslshim.c
@@ -120,6 +120,11 @@ static void OpenLibraryOnce(void)
     // Current supported FreeBSD releases should use the order .11 -> .111
     if (libssl == NULL)
     {
+        DlOpen(MAKELIB("12"));
+    }
+
+    if (libssl == NULL)
+    {
         DlOpen(MAKELIB("11"));
     }
 
