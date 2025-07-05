SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..
ALGO_NAME=$1
# ================================================================================

sed "s/ALGORITHM/$ALGO_NAME/g" CMakeLists_template.txt > CMakeLists.txt

cmake -DCMAKE_BUILD_TYPE:STRING=Release \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	--no-warn-unused-cli \
	-S . \
	-B ./build \
	-G "Unix Makefiles" \
	-DCMAKE_C_COMPILER:FILEPATH=/opt/homebrew/bin/aarch64-apple-darwin24-gcc-14 \
	-DCMAKE_CXX_COMPILER:FILEPATH=/opt/homebrew/bin/aarch64-apple-darwin24-g++-14 \

cmake --build ./build --config Release --target all -j 10 --

python3 experiments/$ALGO_NAME/run.py