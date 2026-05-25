sh build.sh

if [ $? -eq 0 ]; then
	echo "Running target..."
	echo ""
	echo "Output"
	echo "================="
	./build/bin/run
else
	echo "Compilation failed!"
fi