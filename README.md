# fastcompare

use `make build` to compile

## usage

`fastcompare file1 file2`

## about

`fastcompare` is designed to compare line-based content (order doesn't matter) of very large ASCII files.

For the 1st file, it will generate `crc32` hashes for each line (so it is more memory efficient when you take the smaller file as the 1st file. But this has no affect on the speed).
Now it will iterate over the 2nd file, build a temporary `crc32` hash and do a binary search in the hash array.



## caution

still under construction

todo:

  * use struct array to carry line index after sorting
  * use optional other hashing algorithms to lower the risc of collisions
  
# restrictions

  * duplicates lines from file one, can be marked as "not included in" 2nd file (only when the 2nd one hasn't the equal number of this line).
