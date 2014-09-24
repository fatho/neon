import Effects

Size : Type
Size = Bits64

data VirtPtr a = MkVirtPtr Bits64

coerceVirt : VirtPtr a -> VirtPtr b
coerceVirt (MkVirtPtr x) = MkVirtPtr x

data Memory : Effect where
  MkMemory  : IO a -> { () } Memory a

instance Handler Memory IO where
  handle () (MkMemory go) k = do x <- go; k x ()

MEMORY : EFFECT
MEMORY = MkEff () Memory

class Storable a where
  readMem  : VirtPtr a -> { [MEMORY] } Eff a 
  writeMem : VirtPtr a -> a -> { [MEMORY] } Eff ()

memcpy' : Bits64 -> Bits64 -> Bits64 -> IO ()
memcpy' to from sz = mkForeign (FFun "memcpy" [FBits64, FBits64, FBits64] FUnit) to from sz

memcpy : Size -> VirtPtr a -> VirtPtr a -> { [MEMORY] } Eff ()
memcpy sz (MkVirtPtr from) (MkVirtPtr to) = call $ MkMemory $ memcpy' to from sz