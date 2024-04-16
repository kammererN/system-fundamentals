// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-04-15

Create a file called data by running this command:
head –c 10000 /dev/random > data
Create a program that uses mmap to map the above “data” file into memory
It should then create a shared memory area and uses memcpy to copy the data from the
mmap’ed memory into the shared memory area.
Create a program that attaches to the shared memory from above.
It should mmap a new file and memcpy the data from shared memory into the new file.
Use cmp to ensure that the two files are the same. If they are the same, you will see no output.
If they are different, you will see the first difference.