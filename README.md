# cpp-compile-calc

C++ Calculator at compile-time, with template meta-programming.
Features:
- Operators +,-,*,/
- parentheseses
- integers


## Build

```shell
mkdir _build
cd _build
cmake ..
echo "\"12+1"\" > ../src/input.txt
make
./calc #13
```

## Tests

```shell
cd _build
python ../check.py
```
