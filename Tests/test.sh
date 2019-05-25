CLANG="clang"
OPT="opt"

BUILD_DIR="../build"
DCE_LIB="$BUILD_DIR/DeadCodeElimination/LLVMDeadCodeElimination.so"
VSSA_LIB="$BUILD_DIR/VSSA/LLVMVSSA.so"

tests=('Bubblesort' 'IntMM' 'Oscar' 'Perm' 'Puzzle' 'Queens' 'Quicksort' 'RealMM' 'Towers' 'Treesort' 'switch' 'Example1' 'Example2')

for ((i=0; i<${#tests[@]}; i++)) do
    file=${tests[${i}]}

    ${CLANG} ${file}.c -o ${file}.bc -c -emit-llvm -O0

    ${OPT} -instnamer -mem2reg -break-crit-edges ${file}.bc -o ${file}.rbc
    ${OPT} -load $VSSA_LIB -vssa ${file}.rbc -o ${file}.vssa.rbc 
    ${OPT} -load $DCE_LIB -ra-dce ${file}.vssa.rbc -o ${file}.dce.rbc

    ${OPT} -dot-cfg ${file}.dce.rbc -disable-output

    rm -f ${file}.bc
    rm -f ${file}.rbc
    rm -f ${file}.vssa.rbc

done
