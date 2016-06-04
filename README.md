# fastcompare

use `make build` to compile

## examples

	$ ./fastcompare small big 
	start generating 3 hashes for file small
	start comparing hashes with file big
	summary:
	 File small: 3 lines
	 File big: 5 lines
	 File small has 1 lines which are not in file big


	$ ./fastcompare big small verbose
	start generating 5 hashes for file big
	start comparing hashes with file small
	summary:
	 File big: 5 lines
	 File small: 3 lines
	 File big has 3 lines which are not in file small
	 Following lines of big are not included in small: 

	1
	5
	3
