{-# LANGUAGE OverloadedStrings #-}
import Development.Shake hiding ((*>))
import Development.Shake.FilePath
import Control.Applicative ((<$))

-- FILES -----------------------------------------------------------------------

idrisC, idrisO, neonSys :: FilePath
idrisC  = "build/kernel/idris/idris.c"
idrisO  = "build/kernel/idris/idris.o"
neonSys = "build/neon.sys"

gcc, ld, idris :: String
gcc   = "x86_64-elf-gcc"
ld    = "x86_64-elf-ld"
idris = "idris"
nasm  = "nasm"

-- RULES -----------------------------------------------------------------------

main :: IO ()
main = shakeArgs shakeOptions $ do
  phony "install" $ do
    prefix <- getEnvWithDefault "build" "PREFIX"
    liftIO $ print prefix
    copyFile' "build/neon.sys" (prefix </> "boot/neon.sys")
    copyFile' "grub/grub.cfg.in" (prefix </> "boot/grub/grub.cfg")
    return ()

  phony "build" $ need [neonSys]

  phony "clean" $ do
    () <- cmd ["rm", "-Rf", "build" :: String]
    return ()

  neonSys %> \x -> do
    () <- cmd ["mkdir", "-p", "build/kernel" :: String]
    need [idrisO]
    cs <- needCompiled "c"
    ss <- needCompiled "asm"
    cmd $ [ld] ++ ldflags ++ ["-T", "kernel/link.ld", "-o", x, idrisO] ++ cs ++ ss

  idrisO %> \x -> do
    need [idrisC]
    buildC idrisC x

  idrisC %> \x -> do
    is <- getDirectoryFiles "" ["kernel/idris//*.idr"]
    need is
    buildIdr "kernel/idris/Main.idr" x
    return ()

  buildFiles "asm" buildS
  buildFiles "c"   buildC

buildIdr :: FilePath -> FilePath -> Action ()
buildIdr s o = command_ [EchoStdout True] idris $ idrflags ++ ["-o", o, s]

buildS :: FilePath -> FilePath -> Action ()
buildS s o = cmd $ [nasm] ++ asflags ++ [s, "-o", o]

buildC :: FilePath -> FilePath -> Action ()
buildC s o = cmd $ [gcc, "-c", s, "-o", o] ++ cflags

-- FLAGS -----------------------------------------------------------------------

cflags :: [String]
cflags =
  [ "-m64"
  , "-ffreestanding"
  , "-Ikernel"
  , "-Ikernel/idrisrts"
  , "-mcmodel=large"
  , "-std=c11"
  , "-DIDRIS_TARGET_OS=\"neon\""
  , "-DIDRIS_TARGET_TRIPLE=\"x86_64-neon-elf\""
  ]

ldflags :: [String]
ldflags = ["-z", "max-page-size=0x1000"]

asflags :: [String]
asflags = ["-f elf64"]

idrflags :: [String]
idrflags =
  [ "-S", "-p", "effects"
  , "-i", "kernel/idris"
  ]

-- HELPER ----------------------------------------------------------------------

dropDirs :: Int -> FilePath -> FilePath
dropDirs n file = iterate dropDirectory1 file !! n

needCompiled :: String -> Action [FilePath]
needCompiled ext = do
  ss <- getDirectoryFiles "" ["kernel//*." ++ ext]
  let os = fmap (\p -> "build" </> (p -<.> ('o' : ext))) ss
  os <$ need os

buildFiles :: String -> (FilePath -> FilePath -> Action ()) -> Rules ()
buildFiles ext go = "build/kernel//*.o" ++ ext %> \o -> do
  let s = dropDirs 1 (o -<.> ext)
  need [s]
  go s o