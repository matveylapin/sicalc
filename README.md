# SICALC - calculator library on C
## Build
### Linux / MacOS
```bash
git clone
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build
```
## Usage
```bash
./build/cli_sicalc "sin(pi/4)"
./build/cli_sicalc "sin(pi/x)" -k x=4 
```