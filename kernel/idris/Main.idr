-- %include "base/panic.h"

panic : String -> IO ()
panic str = mkForeign (FFun "kernel_panic" [FString] FUnit) str

wasteMem : Int -> String
wasteMem 0 = "Consider this memory wasted. No, really, this memory does not contain any useful information. Just pass by, nothing to see here."
wasteMem n = wasteMem (n-1) ++ wasteMem (n-1)

main : IO ()
main = panic (wasteMem 20)
