#! /bin/bash

cp filestocopy/cache0.txt file.txt

mkdir forimages
mkdir build ; cd "$_"
make -j8
make run_editor_main

for FILE in editor/mprog/plot; do
    gnuplot <<- EOF
        set xlabel "nthreads"
        set ylabel "milliseconds"
        set title "Graph title"   
        set term png
        set output "${FILE}.png"
        plot "${FILE}.txt" with lines
EOF

mv editor/mprog/plot.png ../forimages/cache0.png
done

cd ..
cp filestocopy/cache1.txt file.txt
cd build
make run_editor_main

for FILE in editor/mprog/plot; do
    gnuplot <<- EOF
        set xlabel "nthreads"
        set ylabel "milliseconds"
        set title "Graph title"   
        set term png
        set output "${FILE}.png"
        plot "${FILE}.txt" with lines
EOF

mv editor/mprog/plot.png ../forimages/cache1.png
done

cd ..
cp filestocopy/cache2.txt file.txt
cd build
make run_editor_main

for FILE in editor/mprog/plot; do
    gnuplot <<- EOF
        set xlabel "nthreads"
        set ylabel "milliseconds"
        set title "Graph title"   
        set term png
        set output "${FILE}.png"
        plot "${FILE}.txt" with lines
EOF

mv editor/mprog/plot.png ../forimages/cache2.png
done

cd ..
cp filestocopy/cache3.txt file.txt
cd build
make run_editor_main

for FILE in editor/mprog/plot; do
    gnuplot <<- EOF
        set xlabel "nthreads"
        set ylabel "milliseconds"
        set title "Graph title"   
        set term png
        set output "${FILE}.png"
        plot "${FILE}.txt" with lines
EOF

mv editor/mprog/plot.png ../forimages/cache3.png
done