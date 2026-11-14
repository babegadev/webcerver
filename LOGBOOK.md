# Webcerver logbook
A place where I log my progress throughout this project. I will be documenting my learning process, taking notes, explaining my thought process, etc.

## Part 1: Learning fundamentals

### 1.1: Strings in c
I had to implement my own version of `strlen` and `strncmp`. I had already done this before on my learn c excercises, but it was a nice refresher. It strengthened my understanding of strings in c by just imagining that everything is flat, and in the case of strings, the end is terminated by `\0`. I also learned the importance of typecasting as the std strncmp function casts the type before comparing to prevent weird arithmetic stuffs.

### 1.2: Request line splitter
This was a challenging one. Normally, I would just call string.split(" ") function in python/js and be done with it. But now, I had to really rethink the way I program. I initially started by using while loops and an index tracker, going through the code until I reaches the req_length or if the current character is a space. At first, I had some problem with the indexing and the length, but then I managed to get a basic version working. However, I tested my code with several bad inputs and it couldn't handle many edge cases. I then created a helper function to traverse the current string up to a certain character while passing the I pointer. I used dereferencing to pass I between. Then, I ran into several issues with double spaces and etc, but I settled on the solution of returning a number through the helper function to determine the current subset length. It was a game changer and after adding some more error checking, I managed to handle all of the edge cases successfully. Btw it's 3am and I have a stats exam at 10 today I'm so cooked lol.

### 1.3: Reading a whole file into heap buffer of exactly the right size
Okay this one's pretty tough. I'm currently able to get the file size using fseek() and ftell(), and I can already allocate memory in heap using malloc(size) using the size from earlier. However, I'm still unable to write the whole text file to the output. I think i need to use a loop, but reference the pointer at certain index. Okay turns out there is another method called fread() that can read the whole file at once. I also learned that in these cases, passing a double pointer might be necessary, or instead we can use the idiomatic convention of returning a char* pointer instead like waht malloc does. Final improvements include error checking and freeing memory.

### 1.4: Dynamic array of items that doubles capacity when full
This one I think I have done before in my learn-c, where I created an IntVec, I think the key is to track len and cap. Completed the function(s) from the given function headers. Added checks to prevent size from overflowing by comparing to MAX_SIZE. Split the grow function into its own static helper.

## Part 2 - Tooling: compiler, debugger, sanitizers, make

### 2.1: Catching erros with tools:
- Off by one: ASan caught this. Running the compiled binary with -Wall and -Wextra does not give the warning and it fails silently.
- Use after free: ASan caught this one, but compiling with no flag and running the binary still flags an error.
- Read uninitialized: this one got caught by the -Wall flag.

Further exploration:
- Turns out `gdb` is not recommended for use on mac due to Apple's security models. So we can use `lldb` instead or setup a linux dev env (which I'm guessing is why part 3 exist). 

ASan deep dive:
- used by adding `-fsanitize=address` when compiling / linking
- can be used to detect:
    - out of bounds to heap, stack, and globals
    - use after free
    - double-free, invalid free
    - use after return (with extra flags)
- ASan introduces a ~2x slowdown
- ASan exits on the first detected error, because a memory error causes an unstable program state, which makes the rest of the program inconsistent. This also speeds up compilation.

UndefinedBehaviorSanitizer (UBSan)
- used by adding `-fsanitize=undefined`
- can be used to detect
    - array subscript out of bounds (when it can be determined statically)
    - bitwise shifts out of bounds for the data type
    - dereferencing null/misalligned pointers
    - signed integer overflow
    - floating point conversions that would cause an overflow
- UBSan have a small runtime cost and no impact on address space layout

Valgrind
- debugging tool similar to ASan
- runs on compiled binary (unlike ASan where you have to recompile with a specific flag)
- works by emulating the cpu (whereas ASan manipulates the memory)
- significant slowdown (20-50x)
- run with `valgrind --leak-check=yes ./myprog arg1 arg2`

Summary of which tools to use:
| Error type        | ASan  | UBSan | Valgrind |
| ----------        | ----  | ----- | -------- |
| off-by-one        | X     |       | X        |
| use-after-free    | X     |       |          |
| uninitialized     |       |       | X        |

GDB
- just a debugger
- run with `gdb ./prog -tui` for gui
- commands `file, run, b (break), quit, file, next, step, bt`
- pair with ASan to catch bugs. compile with ASan then `ASAN_OPTIONS=abort_on_error=1 gdb ./prog -tui`
    - `(gdb) run`
    - `(gdb) bt`
    - `(gdb) frame #n`
    - `(gdb) info locals`

### 2.2: Makefile
I guess this is like a config file similar to a Dockerfile where you set the commands to run on build. Will be going through the https://makefiletutorial.com/ site.

Okay so i get it, kinda. Basically we can set a target (an output file). And if the output file doesn't exist yet, make will run the commands to build/compile that target file so that it exists.

Usage example
```make
blah: blah.o
	cc blah.o -o blah # Runs third

blah.o: blah.c
	cc -c blah.c -o blah.o # Runs second

blah.c:
	echo "int main() { return 0; }" > blah.c # Runs first
```
In `blah: blah.o`, `blah` is the target, `blah.o` is a requirement (dependency). So it searches first for blah.o. Since blah.o requires blah.c, it first searches for blah.c.

Other notable things: make clean is used to cleanup files. It's just like any other target. There are several targets & wildcards available, but we can always refer to the docs. For now I think I'm good and can continue.

### 2.3: Setting up a linux environment
This looks like a fun part lol, I miss homelabbing. So, after some research, I found that I can use OrbStack to create an Ubuntu VM (arm64). Since my pi that I'll use to host the final version will also be arm64, the behaviour should be the same. OrbStack also uses much less ram, so I guess it's a good choice.

Orbstack & Ubuntu VM:
- I have installed OrbStack, and created a default VM, called it (webcerver-vm)
- Installed the required packages `buld-essentials clang gdb valgrind curl netcat-openbsd make git`
- Figured out how to ssh, and how to use the `orb` command
- Can access files on webcerver folder from the VM
- Can compile files from the VM and can run `gdb` command

Netcat (nc):
- Successfully started a netcat listener in orb `nc -l 8080`
- Connected from mac `nc webcerver-vm.orb.local 8080`
- Could send messages between the 2 of them

## Part 3 - Files

### 3.1 - Cat:
It's been a while. I just finished 2 midterms this week, so time to get back to coding. Right so this was a fun one. I started by getting the file descriptor using the open() function and passing in the filename. Then, i tried reading the bytes and printing it through the buffer. But it had some weird issues like for example an error message printed just before the last line of my cat output. Turns out using printf caused some issues due to buffering. Instead, i was supposed to use write directly to the stdout. Then, I realized I needed to handle the ./cat command and just print back whatever comes in stdin to stdout. So, I extracted the copy from file to stdout into its own function and called it from the blank cat branch.