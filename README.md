# This is a multithreading research

## To build a project, you should type
```
cmake -S ./ -B build/
```

## To build a project with sanitizers
```
cmake -S ./ -B build/ -DSANITIZERS=1
```

## To run every test
```
cd build
make run_every_test
```

## To run every benchmark
```
cd build
make run_every_bench
```

## To get research
```
chmod +x mprog.sh
./mgprog.sh
```