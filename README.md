# superOS
a rudementary operating system written in C. Uses GRUB bootloader
## Before
1. instal qemu and GNU GAS binutils
## How to?
### made for linux
1. `chmod u+x run.sh` <- grant permissions to run
2. `./run.sh`<- compiles and runs on qemu

## Commands
1. `echo whatever` prints
2. `create name` creates a a file with a name and returns the file id
3. `open fileid` opens a file using its file id
4. `append fileid data` adds data to the end of a file
5. `overlay fileid data` writes over a file from the beggining of it
6. `edit fileid` WIP, opens the file and allwos you to "edit" it
7. `dump`dumps file ram
