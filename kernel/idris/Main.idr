-- %include "base/panic.h"
import Effects
import Memory
import VGA

panic : String -> IO ()
panic str = mkForeign (FFun "kernel_panic" [FString] FUnit) str

wasteMem : Bits64 -> String
wasteMem n = if n == 0
  then "Consider this memory wasted. No, really, this memory does not contain any useful information. Just pass by, nothing to see here."
  else wasteMem (n-1) ++ wasteMem (n-1)

----

main : IO ()
main = run $ return ()