export LLVM_DIR=/home/lionswrath/llvm/build/9.0.0/

tests=('Example')

for ((i=0; i<${#tests[@]}; i++)) do
    file=${tests[${i}]}

    clang ${file}.c -o ${file}.bc -c -emit-llvm -O0

    opt -instnamer -mem2reg -break-crit-edges ${file}.bc -o ${file}.rbc
    opt -load ../build/VSSA/LLVMVSSA.so -vssa ${file}.rbc -o ${file}.vssa.rbc 
    opt -load ../build/DeadCodeElimination/LLVMDeadCodeElimination.so -ra-dce ${file}.vssa.rbc -o ${file}.dce.rbc

    #opt -dot-cfg ${file}.vssa.rbc -disable-output
    opt -dot-cfg ${file}.dce.rbc -disable-output

    rm -f ${file}.bc
    rm -f ${file}.rbc
    rm -f ${file}.vssa.rbc

done
