# fastcompare

use `make build` to compile

## examples

	$ ./fastcompare big small verbose
	start generating 5 hashes for file big
	start comparing hashes with file small
	File big has 3 lines which are not in file small
	 Following lines of big are not included in small: 

	1
	5
	3
	
	$ ./fastcompare small big
	start generating 3 hashes for file small
	start comparing hashes with file big
	File small has 1 lines which are not in file big
