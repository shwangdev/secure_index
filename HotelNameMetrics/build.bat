if exist bin rm -rf bin
md bin
javac -d bin src/*.java src/metrics/*.java src/metrics/costfunctions/*.java src/metrics/math/*.java src/metrics/tokenisers/*.java src/metrics/wordhandlers/*.java
cp Distribute*.txt bin