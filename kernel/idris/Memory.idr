import Effects
%access public

--------------------------------------------------------------------------------
-- Pointers and Sizes
--------------------------------------------------------------------------------

||| Size in bytes of a data structure in memory.
Size : Type
Size = Bits64

||| Offset in bytes.
Offset : Type
Offset = Bits64

||| Pointer into virtual memory.
data VirtPtr a = MkVirtPtr Bits64

instance Cast (VirtPtr a) (VirtPtr b) where
  cast (MkVirtPtr x) = MkVirtPtr x

data PhysPtr = MkPhysPtr Bits64

addOffset : VirtPtr a -> Offset -> VirtPtr a
addOffset (MkVirtPtr v) o = MkVirtPtr (v + o)

--------------------------------------------------------------------------------
-- Memory Effect
--------------------------------------------------------------------------------

||| Internal representation of Memory effects
data Memory : Effect where
  MkMemory  : IO a -> { () } Memory a

instance Handler Memory IO where
  handle () (MkMemory go) k = do x <- go; k x ()

MEMORY : EFFECT
MEMORY = MkEff () Memory

private
callMemory : IO a -> { [MEMORY] } Eff a
callMemory go = call (MkMemory go) 

--------------------------------------------------------------------------------
-- Storable
--------------------------------------------------------------------------------

||| Class for types that can be written to - and read from - virtual memory.
class Storable a where
  readMem  : VirtPtr a -> { [MEMORY] } Eff a 
  writeMem : VirtPtr a -> a -> { [MEMORY] } Eff ()

--------------------------------------------------------------------------------
-- Primitive Storable Instances
--------------------------------------------------------------------------------

instance Storable Bits8 where
  readMem  (MkVirtPtr ptr)   = callMemory $ mkForeign (FFun "GETMEM8" [FBits64] FBits8) ptr 
  writeMem (MkVirtPtr ptr) x = callMemory $ mkForeign (FFun "SETMEM8" [FBits64, FBits8] FUnit) ptr x

instance Storable Bits16 where
  readMem  (MkVirtPtr ptr)   = callMemory $ mkForeign (FFun "GETMEM16" [FBits64] FBits16) ptr 
  writeMem (MkVirtPtr ptr) x = callMemory $ mkForeign (FFun "SETMEM16" [FBits64, FBits16] FUnit) ptr x

instance Storable Bits32 where
  readMem  (MkVirtPtr ptr)   = callMemory $ mkForeign (FFun "GETMEM32" [FBits64] FBits32) ptr 
  writeMem (MkVirtPtr ptr) x = callMemory $ mkForeign (FFun "SETMEM32" [FBits64, FBits32] FUnit) ptr x

instance Storable Bits64 where
  readMem  (MkVirtPtr ptr)   = callMemory $ mkForeign (FFun "GETMEM64" [FBits64] FBits64) ptr 
  writeMem (MkVirtPtr ptr) x = callMemory $ mkForeign (FFun "SETMEM64" [FBits64, FBits64] FUnit) ptr x

--------------------------------------------------------------------------------
-- Operations
--------------------------------------------------------------------------------

||| Copies a given number of bytes.
memcpy : Size -> VirtPtr a -> VirtPtr a -> { [MEMORY] } Eff ()
memcpy sz (MkVirtPtr from) (MkVirtPtr to) = callMemory go where
  go = mkForeign (FFun "memcpy" [FBits64, FBits64, FBits64] FUnit) to from sz