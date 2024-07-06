# SICALC - calculator library on C
## Build
### Linux / MacOS
```bash
git clone https://github.com/matveylapin/sicalc.git
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build
```
## Usage
### CLI tool:
```bash
./build/cli_sicalc "sin(pi/4)"
./build/cli_sicalc "sin(pi/x)" -k x=4 
```
---
### Add new functions:
File [operators](./src/operators.h#L141):
```c
{"exp", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_exp}
```
ID|Flags|Direct value|Function
:---:|:---:|:---:|:---:
Name of action|Count of arguments & Action type|If args count = 0|If args count > 0
---
### Function format:
```c
typedef sicalc_real (*sicalc_action_fn)(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret);
```
In ret you may set action error field, for example [SICALC_STATUS_ZERO_DEVISION](./include/sicalc/sicalc.h:L35)